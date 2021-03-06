/*
 * purple-events - libpurple events handling plugin and library
 *
 * Copyright © 2011-2012 Quentin "Sardem FF7" Glidic
 *
 * This file is part of purple-events.
 *
 * purple-events is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * purple-events is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with purple-events. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <config.h>
#include <glib.h>
#include <purple.h>

#include "context.h"
#include "events.h"

#include "utils.h"

static gint
_purple_events_utils_find_just_signed_on_account(gconstpointer a, gconstpointer b)
{
    const PurpleEventsJustSignedOnAccount *just_signed_on_account = a;
    return ( just_signed_on_account->account == b ) ? 0 : 1;
}

static gint
_purple_events_utils_check_global_event_dispatch(PurpleEventsContext *context, PurpleAccount *account, PurpleConversation *conv, const gchar *event)
{
    if ( purple_prefs_get_bool("/plugins/core/events/restrictions/only-available") && ( ! purple_status_is_available(purple_account_get_active_status(account)) ) )
        return -1;

    if ( ( conv != NULL ) && purple_conversation_has_focus(conv) )
        return -1;

    if ( g_list_find_custom(context->just_signed_on_accounts, account, _purple_events_utils_find_just_signed_on_account) != NULL )
        return -1;

    gchar *event_name;
    gboolean r;

    event_name = g_strconcat("/plugins/core/events/events/", event, NULL);
    r = purple_prefs_get_bool(event_name);
    g_free(event_name);

    return r;
}

gboolean
purple_events_utils_check_event_dispatch(PurpleEventsContext *context, PurpleAccount *account, PurpleConversation *conv, const gchar *event)
{
    return ( _purple_events_utils_check_global_event_dispatch(context, account, conv, event) > 0 );
}

gboolean
purple_events_utils_check_buddy_event_dispatch(PurpleEventsContext *context, PurpleBuddy *buddy, const gchar *event)
{
    const gchar *name = purple_buddy_get_name(buddy);
    PurpleAccount *account = purple_buddy_get_account(buddy);

    if ( ( ! purple_privacy_check(account, name) ) && purple_prefs_get_bool("/plugins/core/events/restrictions/blocked") )
        return FALSE;

    PurpleConversation *conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_ANY, name, account);

    gint set;

    set = _purple_events_utils_check_global_event_dispatch(context, account, conv, event);
    if ( set < 0 )
        return FALSE;

    PurpleContact *contact = purple_buddy_get_contact(buddy);
    PurpleGroup *group = purple_buddy_get_group(buddy);

    gchar *event_name;
    gboolean r;

    event_name = g_strconcat("events/", event, NULL);

    r = set;
    set = purple_blist_node_get_int(PURPLE_BLIST_NODE(contact), "events");
    if ( set < 0 )
        r = FALSE;
    else if ( set > 0 )
        r = ( purple_blist_node_get_int(PURPLE_BLIST_NODE(contact), event_name) == 1 );
    else
    {
        set = purple_blist_node_get_int(PURPLE_BLIST_NODE(group), "events");
        if ( set < 0 )
            r = FALSE;
        else if ( set > 0 )
            r = ( purple_blist_node_get_int(PURPLE_BLIST_NODE(group), event_name) == 1 );
    }
    g_free(event_name);

    return r;
}
