# TODO

Ideas for later releases. The 0.6.x series is maintenance-only: bug fixes and
quality-of-life changes, not new features.

## Settings UI

- Add a Preferences dialog, reachable from the applet right-click menu.
- Bind it to the existing `com.solus-project.brisk-menu` schema:
  - window type
  - search position
  - label text and visibility
  - icon name and symbolic icon
  - hotkey
  - dark theme
  - rollover activation
- Keep the existing `gsettings` keys working so current configurations are not
  lost.

## Styling

- Improve the menu window edge, background, border, and shadow.
- Add clear focus styling for the search entry.
- Check the result with light and dark GTK 3.24 themes.
- Keep the classic and dash layouts consistent.

## Wayland and in-process applet

Not a small change. Brisk currently relies on X11 for:

- global hotkey registration
- popup placement relative to the panel
- window type and grab behaviour

To support Wayland properly, we would need to:

- add a Meson option for building an in-process applet module
- install it with `InProcess=true` and `Platforms=X11;Wayland;` once it works
- replace the custom popup placement with a Wayland-aware popup or layer-shell
  approach
- replace the global X11 hotkey with a portal/desktop-specific mechanism, or
  disable it cleanly under Wayland
- test with the experimental MATE Wayland session

Until then, the applet metadata intentionally declares `Platforms=X11;`.

## Maintenance

- Keep the `.pot` file and translation workflow up to date.
- Add a small CI build against a current MATE release.
- Add tests for favourites pinning and `.desktop` action handling.
