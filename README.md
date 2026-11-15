# brisk-menu-guix

[![License](https://img.shields.io/badge/License-GPL%202.0-blue.svg)](https://opensource.org/licenses/GPL-2.0)

> [!NOTE]
> Brisk Menu is no longer maintained by Solus or Ubuntu MATE. This community
> fork keeps it working on current MATE releases and collects bug fixes,
> compatibility work, and quality-of-life improvements.
>
> The project is considered feature-complete. No new features are planned beyond
> bug fixes and quality-of-life improvements.
>
> Current release: **0.6.5**, tested with **MATE 1.28**. The applet is currently
> X11-only; Wayland is not supported.

brisk-menu is a modern and efficient menu designed to improve the MATE Desktop
Environment with modern, first-class options.

The purpose of this project is to provide a usable menu as seen in other
desktops without the bloat and performance issues.

brisk-menu is **distro-agnostic** and the reporting of portability issues is
encouraged.

![screenshot](https://raw.githubusercontent.com/getsolus/brisk-menu/master/.github/main.png)

brisk-menu was originally a **collaborative** project between
[Solus](https://getsol.us/) and [Ubuntu MATE](https://ubuntu-mate.org/).

## Features

- Keyboard centric (mice welcome too, of course)
- Customizable menu icon via `gsettings`.
- Hotkey support (defaults to <kbd>Super</kbd>, configurable in gsettings)
- Stupid-fast
- Efficient, useful searching with prioritised listings
- Modular backend design split from the frontend, allowing new backends in
  future (hint: the frontend is not tied to `.desktop` files)
- Context menus for `.desktop` actions (incognito mode, etc.)
- Pin shortcuts to the `Favourites` backend and directly to the desktop using
  the context menu (unpin too!)
- Configurable label (hide/text)
- Automatically adapt to vertical panels
- Automatically reload
- Filter via categories
- Session/screensaver controls - Drag & drop support for launchers
- Sidebar launcher support
- GTK3 + CSS styling options
- Fully correct `X11` WM integration (grab policy and window types)
- Not Python.

**Future (v0.7.0)**

- Settings UI to control further visual aspects (labels/icons/options)
- Improved styling for the window edge + search entry

No other features are planned. Wayland support would require replacing the
X11-specific popup placement and global-hotkey code, so it is not part of the
current maintenance series.

![screenshot_context](https://raw.githubusercontent.com/getsolus/brisk-menu/master/.github/context.png)

## Building Brisk Menu

**Requirements:**

- GTK 3.22 or greater
- GLib 2.50 or greater
- MATE 1.24 or greater (tested with MATE 1.28)
- `meson` and `ninja`
- `pkg-config`
- libnotify

**Build Process:**

```bash
meson setup build --buildtype=plain --prefix=/usr
ninja -C build -j$(($(getconf _NPROCESSORS_ONLN)+1))
sudo ninja -C build install
```

**Development with Guix:**

```bash
guix shell -m manifest.scm -- meson setup build --buildtype=debugoptimized
guix shell -m manifest.scm -- ninja -C build
```

To test the applet without a system install, install it to a temporary prefix
and point `XDG_DATA_DIRS` and `GSETTINGS_SCHEMA_DIR` at that prefix before
running `./test-brisk-menu.sh`.

## License

Copyright © 2016-2026 Brisk Menu Developers

`brisk-menu` is available under the terms of the `GPL-2.0` license.

The translations in `subprojects/translations` come from the former
[`getsolus/brisk-menu-translations`](https://github.com/getsolus/brisk-menu-translations)
repository and are distributed under the same `GPL-2.0` license.

The `brisk_system-log-out-symbolic.svg` icon is a copy of
`application-exit-symbolic.svg` to use within Brisk.

This icon is copyright © Sam Hewitt, from the
[Paper Icons](https://github.com/snwh/paper-icon-theme) theme, available under
the terms of the
[CC-BY-SA-4.0 license](https://creativecommons.org/licenses/by-sa/4.0/).
