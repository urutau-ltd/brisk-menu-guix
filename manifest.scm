;; Development environment for Brisk Menu.
;;
;; Use it with:
;;   guix shell -m manifest.scm

(specifications->manifest
 (list "git"
       "emacs"
       "emacs-evil"
       "gcc-toolchain"
       "dbus-glib"
       "libnotify"
       "gobject-introspection"
       "itstool"
       "desktop-file-utils"
       "gspell"
       "gtk:bin"
       "glib:bin"
       "gtk-doc"
       "intltool"
       "libtool"
       "meson"
       "ninja"
       "pkg-config"
       "cairo"
       "gtk+"
       "libx11"
       "pango"
       "yelp-tools"
       "mate-desktop"
       "mate-panel"
       "mate-menus"
       "guile"
       "ccls"))
