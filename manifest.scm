;; Lo que sigue es un "manifest" equivalente a la línea de comando que
;; introdujo. Puede almacenarlo dentro de un archivo que pudiese pasar a
;; cualquier comando 'guix' que acepte una opción '--manifest' (o -m).

(concatenate-manifests
  (list (specifications->manifest
          (list "git"
                "emacs"
                "emacs-evil"
                "ccls"
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
                "pango"))
        (package->development-manifest
          (specification->package "guile"))))
