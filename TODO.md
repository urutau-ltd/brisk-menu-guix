# TODO

Ideas for later releases. The 0.6.x series is maintenance-only: bug fixes and
quality-of-life changes, not new features.

Issue numbers below refer to the open issues at
<https://github.com/getsolus/brisk-menu/issues>.

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
- Add visibility/sensitivity options for the lock, logout, and shutdown controls
  (`#38`).
- Keep the existing `gsettings` keys working so current configurations are not
  lost.
- Add a “Reset to defaults” action.
- Validate custom icon names and fall back cleanly when an icon is missing.

## Styling and visual polish

- Improve the menu window edge, background, border, and shadow.
- Add clear focus styling for the search entry.
- Check the result with light and dark GTK 3.24 themes.
- Check HiDPI scaling and vertical-panel layouts.
- Keep the classic and dash layouts consistent.
- Review hover, checked, disabled, and selected states for the sidebar, category
  buttons, session controls, and app entries.
- Add an option to centre the category row in the dash layout (`#7`).

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

## Menu window and panel behaviour

- Investigate the menu closing by itself after a few seconds (`#42`).
- Fix mate-panel not auto-hiding after the menu closes via a desktop click
  (`#51`).
- Investigate the applet disappearing after panel layout changes or at startup
  (`#52`).
- Make sure screenshot tools and global shortcuts still work while the menu is
  open (`#54`).
- Verify two applets on separate panels both survive logout/restart and remain
  populated (`#45`).

## Deferred technical work from the 0.6.5 proposal

### Launcher and startup notification

- Replace the remaining `GdkScreen`-based app launch context setup with
  display-based context handling.
- Review launch timestamps and startup-notification IDs.
- Verify startup-notification cleanup when an application fails to launch.

### Hotkeys and input

- Cache the window-manager name instead of querying X11 on every Super+click.
- Review X11 grab edge cases: no modifier, multiple applet instances, hotkey
  changes, and failed grabs.
- Fix the conflict with IBus Super+Space when Brisk is bound to Super (`#47`).
- Verify Super+click window actions still work when the menu is bound to Super_L
  (`#34`).
- Show a clear message when global hotkeys are unavailable on non-X11 displays
  or when the grab fails.

### Context menus and backends

- Ensure applications listed in multiple categories appear consistently
  everywhere they are expected (`#46`).
- Investigate system-wide `.desktop` installs that do not appear in the menu,
  including duplicate desktop IDs and menu cache reloads (`#58`).
- Hold a reference to the active `BriskItem` while its context menu is open.
- Make sure context menus close and rebuild safely when a backend reloads.
- Review the remaining context-menu ownership and lifecycle paths.
- Expand tests for `.desktop` actions and favourites pin/unpin.

### Accessibility and keyboard use

- Make the context-menu key open item context menus like the right mouse button
  (`#44`).
- Improve keyboard focus and navigation between the search entry, categories,
  and application list.
- Highlight the first search result so Enter launches the expected item (`#35`).
- Audit labels and tooltips for screen readers.
- Announce empty search results and state changes.
- Investigate interaction with Onboard and other virtual keyboards, including
  input-layer/window positioning (`#15`).

### Memory and performance

- Profile long-running memory use and fix leaks in backends, context menus, and
  icon handling (`#56`).
- Add a repeatable stress test that opens/closes the menu and reloads `.desktop`
  entries many times.

## Packaging and release hygiene

- Publish a signed release tarball and release notes for future releases.
- Keep `guix.scm` and the `nyctibius` channel package updated after tags.
- Verify packages with `guix build --no-grafts -f guix.scm`.
- Keep `CHANGELOG.md`, `README.md`, and this file in sync.
- Keep the `.pot` file and translation workflow current.
- Review and refresh machine translations; the vendored upstream translation
  repository has been dormant since 2021 (`#55`).

## Testing and CI

- Add a CI build against a current MATE release.
- Add an ASan/UBSan test job.
- Add a clang-format/whitespace check.
- Add tests for favourites pinning and `.desktop` action handling.
- Add a smoke test for applet startup with `mate-panel-test-applets`.
- Add regression coverage for multi-panel startup, menu auto-close, panel
  auto-hide, and system-wide `.desktop` discovery.

## Maintenance

- Keep the minimum GTK, GLib, and MATE versions aligned with current MATE.
- Audit deprecated GTK and MATE APIs when MATE releases new versions.
- Review translations after adding or changing user-visible strings.

## Discarded upstream issues

These were reviewed and intentionally not added to the active TODO:

- `#60` Building for MATE 1.28 — resolved by the 0.6.5 compatibility work.
- `#59` Fix call to `g_source_remove()` — already merged into this fork.
- `#57` Status of this project — covered by the README/fork status.
- `#53` Alpine build failure — user environment is missing `gcc-multilib`; not a
  Brisk code bug.
- `#50` Move the search bar into the application list — layout customization,
  out of maintenance scope.
- `#43` Directly edit a menu entry from the right-click menu — new feature, out
  of scope.
- `#37` Most-used items — new feature, out of scope.
- `#22` Drag/reorder favourites — new feature, out of scope.
- `#11` Add a “Places” category — new feature, out of scope.
- `#33` Customize the main icon — implemented via `icon-name` and
  `icon-symbolic`.
- `#14` Keyword search — implemented through `.desktop` keywords.
- `#32` Removed libsaver-glue patch — obsolete; the glue is generated and the
  current build works.
- `#18` journald spam — the example logs come from Thunderbird/Grammalecte under
  the applet’s journal unit, not from Brisk itself; no actionable code change.
