# Changelog

## 0.6.5

Maintenance and MATE 1.28 compatibility release.

- Require GTK 3.22, GLib 2.50, and MATE 1.24; remove the old GTK 3.18 code
  paths.
- Use `mate_panel_applet_setup_menu_from_resource()` for the applet context
  menu.
- Mark the applet as X11-only in the MATE panel metadata.
- Use `GdkMonitor` work areas for menu placement, keeping the menu clear of
  panels and docks.
- Replace deprecated GTK/GDK menu-popup, monitor, error-trap, and flush calls.
- Fix global hotkey handling when the hotkey changes or is removed, and skip X11
  setup on non-X11 displays.
- Fix a `GError` double-free in session setup and honour `CanShutdown`.
- Remove global singleton state from the window, apps, and favourites backends;
  fixes multiple applet instances and window-type switching.
- Fix `.desktop` action reference handling and missing-file checks.
- Fix the entry button release handler.
- Vendor the latest translations from the dormant
  `getsolus/brisk-menu-translations` repository.
- Fix `manifest.scm`, refresh the README, and update packaging for 0.6.5.
