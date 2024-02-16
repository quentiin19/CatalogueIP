# Version minimum de cmake
cmake_minimum_required(VERSION 3.22)

# Nom et language du projet
project(gtk_tp C)

set(CMAKE_C_STANDARD 99)

# On utilise le module PkgConfig pour détecter la librairie GTK+ sur le système
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

# On utilise PkgConfig pour détecter et configurer la librairie SQLite
PKG_CHECK_MODULES(SQLite3 REQUIRED sqlite3)

# On dit à CMake d'utiliser GTK+ et SQLite, on indique au compilateur où trouver les fichiers headers
# Et au linker où trouver les librairies
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS} ${SQLite3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS} ${SQLite3_LIBRARY_DIRS})

# Instructions pour le compilateur
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

#Pour linux, va servir à correctement lier les signaux de notre application au code
if (UNIX)
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,-export-dynamic")
endif (UNIX)

# Ajoute un éxecutable à partir du main.c
add_executable(start process.h
        main.c
        fonction/function.h
        fonction/function.c
        fonction/cli_display.h
        fonction/cli_display.c
        fonction/gtk_add_ip.h)  # Assurez-vous que process.c est inclus dans la liste des fichiers source


        # fonction/gtk_search_all.h
        # fonction/gtk_search.h
        # fonction/gtk_delete.h)  # Assurez-vous que process.c est inclus dans la liste des fichiers source

# Lie à l'éxecutable les librairies GTK+ et SQLite
TARGET_LINK_LIBRARIES(start ${GTK3_LIBRARIES} ${SQLite3_LIBRARIES})
