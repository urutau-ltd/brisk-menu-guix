# AGENTS.md

Instructions for coding agents working in this repository.

## Project status

- `brisk-menu-guix` is a maintenance fork of Brisk Menu for the MATE desktop.
- The original project is no longer maintained by Solus or Ubuntu MATE.
- The 0.6.x series is **maintenance-only**: bug fixes, compatibility work, and
  quality-of-life changes. Do not add new features to 0.6.x.
- Longer-term feature ideas belong in `TODO.md`, not in a new branch unless the
  user explicitly asks for one.
- The two planned README items are intentional and must stay there:
  - settings UI
  - improved window-edge/search-entry styling
- The applet is currently **X11-only**. Do not advertise or claim Wayland
  support.

## Environment

- Tested with MATE 1.28, GTK 3.24, and GLib 2.86.
- Minimum supported versions:
  - GTK 3.22
  - GLib 2.50
  - MATE 1.24
- The user builds and runs this on Guix.
- If Meson picks up a nonexistent system compiler inside `guix shell`, unset
  `CC` and `CXX` before configuring.

## Repository layout

- `src/mate-applet/` — MATE panel applet, factory, applet menu actions.
- `src/frontend/` — menu windows and shared UI:
  - `classic/` — classic two-pane menu
  - `dash/` — full-screen dash menu
  - context menus, grabs, keyboard handling, search, settings, session
- `src/backend/` — backend/item/section interfaces plus the apps, all-items, and
  favourites backends.
- `src/lib/` — X11 hotkey binder, style names, utility macros.
- `data/` — GSettings schema, GResource bundle, applet metadata, D-Bus service,
  CSS, icons.
- `subprojects/translations/` — vendored translations and translation build.
- `guix.scm` — Guix package definition in this repository.
- `manifest.scm` — Guix development environment.
- `README.md`, `CHANGELOG.md`, `TODO.md` — user-facing documentation.

## Build

Using the Guix manifest:

```bash
guix shell -m manifest.scm -- bash -c 'unset CC CXX; meson setup build --buildtype=debugoptimized && ninja -C build'
```

Or without the manifest:

```bash
guix shell --no-grafts meson ninja pkg-config gcc-toolchain 'gtk+:bin' glib \
  mate-panel mate-menus mate-desktop libnotify intltool itstool \
  gobject-introspection libxml2 desktop-file-utils yelp-tools dconf libsm \
  libx11 pango -- bash -c 'unset CC CXX; meson setup build --buildtype=debugoptimized && ninja -C build'
```

Release-style build:

```bash
meson setup build --buildtype=plain --prefix=/usr
ninja -C build
sudo ninja -C build install
```

The build must finish without new warnings.

## Testing the applet

Install to a temporary prefix so the MATE factory and GSettings schema can be
found:

```bash
meson setup /tmp/brisk-build --buildtype=debugoptimized \
  --prefix=/tmp/brisk-prefix --libdir=lib --libexecdir=libexec
ninja -C /tmp/brisk-build install

XDG_DATA_DIRS=/tmp/brisk-prefix/share:/run/current-system/profile/share:$HOME/.local/share \
GSETTINGS_SCHEMA_DIR=/tmp/brisk-prefix/share/glib-2.0/schemas \
timeout 10 mate-panel-test-applets --iid BriskMenuFactory::BriskMenuApplet
```

`./test-brisk-menu.sh` also runs the test applet, but it starts with
`killall -q brisk-menu`, so do not use it while the user is actively using the
installed applet unless they agree.

Known test-harness noise on MATE 1.28 / GLib 2.86:

```text
GLib-CRITICAL: g_variant_new_string: assertion 'string != NULL' failed
```

These come from `mate-panel-test-applets`, not from Brisk. Reproduce with a
stock applet such as `SensorsApplet::SensorsApplet` before blaming this project.

Manual areas worth testing after UI/backend changes:

- classic, dash, and automatic window types
- horizontal and vertical panels
- label visible/hidden, custom icon, symbolic icon
- search and category filters
- favourites pin/unpin and desktop pin/unpin
- `.desktop` add/remove while the menu is open
- item context menus and `.desktop` actions
- hotkey bind/unbind, Escape, click-outside close
- repeated window-type switching
- multi-monitor work-area placement

An ASan/UBSan build is useful for lifetime changes:

```bash
meson setup build-asan --buildtype=debug -Db_sanitize=address,undefined
ninja -C build-asan
```

The install post-install script may fail in a minimal shell when
`glib-compile-schemas` or `gtk-update-icon-cache` are missing; build and run the
binary from the build tree or install with a fuller environment.

## Code conventions

- C11, GTK3, GPL-2.0-or-later headers.
- Eight-space indentation, no tabs, existing project style.
- Wrap GTK/GLib includes with `BRISK_BEGIN_PEDANTIC` / `BRISK_END_PEDANTIC`.
- Use `__brisk_unused__` on callback parameters that must stay in the signature.
- Use `autofree` only for owned values. Do not wrap borrowed values from
  `g_object_get_data()` or similar APIs.
- Prefer `g_clear_error()`, `g_clear_object()`, and `g_clear_pointer()` for
  cleanup.
- Do not reintroduce global singleton pointers. Pass state through signal user
  data or object fields.
- Any idle/timeout callback using a raw object pointer must either hold a
  reference or have its source ID removed during `dispose()`.
- The MATE applet API still requires `GtkAction` / `GtkActionGroup`. Keep those
  uses wrapped in `G_GNUC_BEGIN_IGNORE_DEPRECATIONS`; do not migrate that API
  unless libmatepanelapplet changes.
- Do not reintroduce deprecated `gdk_screen_get_monitor_*()`,
  `gtk_menu_popup()`, `gdk_error_trap_*()`, `gdk_flush()`, or
  `gtk_misc_set_alignment()` calls.
- `./update_format.sh` runs `clang-format` over all C/H files and optionally
  `misspell`. Avoid running it on unrelated files if it creates a huge diff.

## Important invariants

- GSettings schema ID stays `com.solus-project.brisk-menu`; existing keys must
  remain compatible.
- Applet factory ID stays `BriskMenuFactory`; applet IID is
  `BriskMenuFactory::BriskMenuApplet`.
- The applet metadata must keep `Platforms=X11;` until real Wayland support
  exists.
- The applet right-click menu is loaded from the GResource path
  `/com/solus-project/brisk/menu/applet-menu.ui`.
- Global hotkeys are X11-only. The key binder must fail cleanly on non-X11
  displays.
- The session code talks to `org.gnome.SessionManager` and
  `org.mate.ScreenSaver`; keep `CanShutdown` handling.
- Translations are vendored in `subprojects/translations/`. Do not re-add
  `.gitmodules` or a translations submodule.

## Release process

Use lightweight tags, matching existing tags such as `v0.6.4`.

1. Update the version in `meson.build` and
   `subprojects/translations/meson.build`.
2. Update `CHANGELOG.md`.
3. Build and test.
4. Commit the release work and merge it into `master` (fast-forward is fine).
5. Tag the release commit from `master`:

   ```bash
   git tag vX.Y.Z
   ```

6. Push `master` and the tag to both Brisk mirrors.
7. Compute the Guix source hash from a clean checkout of the tag:

   ```bash
   git clone --no-local . /tmp/brisk-tag
   git -C /tmp/brisk-tag checkout vX.Y.Z
   rm -rf /tmp/brisk-tag/.git
   guix hash -rx /tmp/brisk-tag
   ```

8. Update `guix.scm` and the user's `nyctibius` channel package with the tag and
   hash, then commit the package update **after** the tag.
9. Verify the package builds:

   ```bash
   guix build --no-grafts -f guix.scm
   ```

The tag cannot contain its own source hash. That is why the package-update
commit is necessarily one commit ahead of the tag on `master`. This was the
v0.6.5 release pattern.

## Mirrors and external writes

Brisk Menu mirrors:

- `origin` — `git@codeberg.org:urutau-ltd/brisk-menu-guix.git`
- `ro-mirror` — `git@github.com:urutau-ltd/brisk-menu-guix.git`

Do not use `guix-mate/brisk-menu-guix`; that Codeberg path does not exist.

The user's Guix channel repository is separate:

- `~/src/nyctibius/channel`
- branch `stable`
- remotes `origin` (`sl.urutau-ltd.org`) and `ro-mirror2` (GitHub)

**Do not modify, commit, or push the channel repository unless the user
explicitly asks for it.** The same applies to `~/.config/guix/channels.scm`.
Updating a package file is not permission to commit or push the channel.

Do not push branches, tags, or commits to any remote without explicit user
instruction. Do not create extra branches for future ideas; use `TODO.md`.

## Current release state at the time this file was written

- Released tag: `v0.6.5`
- Tag commit: `846b05c` (release code and documentation)
- `master` contains the post-tag Guix package hash update and follow-up
  documentation commits.
- The remote `maint/v0.6.5` branch was deleted after the release merge.
- Do not recreate or push that branch unless asked.
