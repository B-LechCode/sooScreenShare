# Name des Projekts
set(PROJECT_NAME screenClient)
project( ${PROJECT_NAME} )

# Altersbegrenzung fuer CMAKE: mindestens Version 2.8
cmake_minimum_required(VERSION 3.6)

# Build-Typ
set(CMAKE_BUILD_TYPE Debug CACHE TYPE STRING FORCE )



# OpenCV suchen
FIND_PACKAGE(OpenCV REQUIRED )

#QT suchen
#FIND_PACKAGE( Qt5 5.11 CONFIG REQUIRED  Core Network Widgets Gui )
find_package(Qt5 COMPONENTS  Core Network Widgets Gui OpenGL REQUIRED)

#X11 suchen
FIND_PACKAGE(X11 REQUIRED)

#OpenGL suchen
find_package(OpenGL REQUIRED)


find_library (lz4_lib lz4)
find_path (lz4_path /lz4.h)
message(${lz4_lib})
set(lz4_include_path /usr/local/include)
message(${lz4_include_path})



# Pfade für Include-Dateien
include_directories (
        ${QT_INCLUDES}
        ${OpenCV_INCLUDES}
        ${OpenCV_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${lz4_include_path}
)

# Fuege alle .cpp und alle .h Dateien aus dem aktuellen Ordner
# in das Projekt ein.
FILE ( GLOB SRC_FILES "*.cpp")
FILE ( GLOB HEADER_FILES "*.h")
LIST(APPEND HEADER_FILES "./../header.h")
LIST(APPEND HEADER_FILES "./../parameter.h")
LIST(APPEND HEADER_FILES "./../parameterserialization.h")
LIST(APPEND HEADER_FILES "./../guihelpers.h")
LIST(APPEND SRC_FILES "./../parameterserialization.cpp")
LIST(APPEND SRC_FILES "./../guihelpers.cpp")

FILE ( GLOB UI_FILES "*.ui")




ADD_DEFINITIONS (
                ${QT_DEFINITIONS}
                )


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Definition der ausfuehrbaren Datei (executable) die der
# Compiler erzeugen soll
ADD_EXECUTABLE ( ${PROJECT_NAME}
        ${SRC_FILES}
        ${HEADER_FILES}
        ${UI_FILES}
)

# Definiere die Bibliotheken, die der C++-Linker
# linken soll. Hier: OpenCV
TARGET_LINK_LIBRARIES ( ${PROJECT_NAME}
        ${OpenCV_LIBS}
        ${X11_LIBRARIES}
        ${QT_LIBRARIES}                
                ${Qt5Widgets_LIBRARIES}
                ${Qt5Network_LIBRARIES}
                ${Qt5Core_LIBRARIES}
                ${Qt5Gui_LIBRARIES}
                ${OPENGL_LIBRARIES}
                ${lz4_lib}
)




