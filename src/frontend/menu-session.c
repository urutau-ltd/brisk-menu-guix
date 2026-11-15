/*
 * This file is part of brisk-menu.
 *
 * Copyright © 2016-2020 Brisk Menu Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define _GNU_SOURCE

#include "util.h"

BRISK_BEGIN_PEDANTIC
#include "menu-private.h"
#include <glib/gi18n.h>
#include <gtk/gtk.h>
BRISK_END_PEDANTIC

static void brisk_menu_window_logout_cb(__brisk_unused__ GObject *obj, GAsyncResult *res,
                                        gpointer v)
{
        autofree(GError) *error = NULL;
        BriskMenuWindow *self = v;

        gnome_session_manager_call_logout_finish(self->session, res, &error);
        if (error) {
                g_warning("Error logging out: %s", error->message);
        }
}

static gboolean brisk_menu_window_logout_real(BriskMenuWindow *self)
{
        if (!self->session) {
                return FALSE;
        }

        gnome_session_manager_call_logout(self->session,
                                          0,
                                          NULL,
                                          brisk_menu_window_logout_cb,
                                          self);
        return FALSE;
}

/**
 * Handle log out
 */
void brisk_menu_window_logout(BriskMenuWindow *self, __brisk_unused__ gpointer v)
{
        gtk_widget_hide(GTK_WIDGET(self));

        g_idle_add_full(G_PRIORITY_DEFAULT_IDLE,
                        (GSourceFunc)brisk_menu_window_logout_real,
                        g_object_ref(self),
                        g_object_unref);
}

static void brisk_menu_window_shutdown_cb(__brisk_unused__ GObject *obj, GAsyncResult *res,
                                          gpointer v)
{
        autofree(GError) *error = NULL;
        BriskMenuWindow *self = v;

        gnome_session_manager_call_shutdown_finish(self->session, res, &error);
        if (error) {
                g_warning("Error shutting down: %s", error->message);
        }
}

static inline gboolean brisk_menu_window_shutdown_real(BriskMenuWindow *self)
{
        if (!self->session) {
                return FALSE;
        }

        gnome_session_manager_call_shutdown(self->session,
                                            NULL,
                                            brisk_menu_window_shutdown_cb,
                                            self);
        return FALSE;
}

/**
 * Handle shut down
 */
void brisk_menu_window_shutdown(BriskMenuWindow *self, __brisk_unused__ gpointer v)
{
        gtk_widget_hide(GTK_WIDGET(self));
        g_idle_add_full(G_PRIORITY_DEFAULT_IDLE,
                        (GSourceFunc)brisk_menu_window_shutdown_real,
                        g_object_ref(self),
                        g_object_unref);
}

static void brisk_menu_window_lock_cb(__brisk_unused__ GObject *obj, GAsyncResult *res, gpointer v)
{
        autofree(GError) *error = NULL;
        BriskMenuWindow *self = v;

        mate_screen_saver_call_lock_finish(self->saver, res, &error);
        if (error) {
                g_warning("Error locking screen: %s", error->message);
        }
}

static inline gboolean brisk_menu_window_lock_real(BriskMenuWindow *self)
{
        if (!self->saver) {
                return FALSE;
        }
        mate_screen_saver_call_lock(self->saver, NULL, brisk_menu_window_lock_cb, self);
        return FALSE;
}

/**
 * Handle lock
 */
void brisk_menu_window_lock(BriskMenuWindow *self, __brisk_unused__ gpointer v)
{
        gtk_widget_hide(GTK_WIDGET(self));
        g_idle_add_full(G_PRIORITY_DEFAULT_IDLE,
                        (GSourceFunc)brisk_menu_window_lock_real,
                        g_object_ref(self),
                        g_object_unref);
}

gboolean brisk_menu_window_setup_session_idle(gpointer v)
{
        BriskMenuWindow *self = v;

        self->session_source_id = 0;
        brisk_menu_window_setup_session(self);
        return G_SOURCE_REMOVE;
}

gboolean brisk_menu_window_setup_session(BriskMenuWindow *self)
{
        autofree(GError) *error = NULL;
        gboolean can_shutdown = TRUE;
        __brisk_unused__ gboolean is_active = FALSE;

        /* Sort out gnome-session dbus */
        self->session = gnome_session_manager_proxy_new_for_bus_sync(
            G_BUS_TYPE_SESSION,
            G_DBUS_PROXY_FLAGS_DO_NOT_AUTO_START | G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES,
            "org.gnome.SessionManager",
            "/org/gnome/SessionManager",
            NULL,
            &error);
        if (error) {
                g_warning("Failed to contact org.gnome.SessionManager: %s\n", error->message);
                g_clear_error(&error);
                g_clear_object(&self->session);
                self->can_shutdown = FALSE;
        } else {
                /* Set sensitive according to policy */
                if (!gnome_session_manager_call_can_shutdown_sync(
                        self->session, &can_shutdown, NULL, &error)) {
                        g_warning("Failed to query org.gnome.SessionManager: %s\n",
                                  error->message);
                        g_clear_error(&error);
                        can_shutdown = TRUE;
                }
                self->can_shutdown = can_shutdown;
        }

        self->saver =
            mate_screen_saver_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION,
                                                     G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES,
                                                     "org.mate.ScreenSaver",
                                                     "/org/mate/ScreenSaver",
                                                     NULL,
                                                     &error);
        if (error) {
                g_warning("Failed to contact org.mate.ScreenSaver: %s\n", error->message);
                g_clear_error(&error);
                g_clear_object(&self->saver);
        } else {
                /* Check the screensaver is *really* running */
                if (!mate_screen_saver_call_get_active_sync(self->saver,
                                                            &is_active,
                                                            NULL,
                                                            &error)) {
                        g_warning("org.mate.ScreenSaver not running: %s\n", error->message);
                        g_clear_error(&error);
                }
        }

        if (self->shutdown_widget) {
                gtk_widget_set_sensitive(self->shutdown_widget, self->can_shutdown);
        }

        return self->session != NULL;
}

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */
