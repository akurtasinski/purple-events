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

#ifndef __PURPLE_EVENTS_UTILS_H__
#define __PURPLE_EVENTS_UTILS_H__

#include <purple-events.h>

G_BEGIN_DECLS


gboolean purple_events_utils_check_event_dispatch(PurpleEventsContext *context, PurpleAccount *account, PurpleConversation *conv, const gchar *event);
gboolean purple_events_utils_check_buddy_event_dispatch(PurpleEventsContext *context, PurpleBuddy *buddy, const gchar *event);


G_END_DECLS

#endif /* __PURPLE_EVENTS_UTILS_H__ */
