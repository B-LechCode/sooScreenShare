# Name des Projekts
set(PROJECT_NAME screenServer)
project( ${PROJECT_NAME} )

# Altersbegrenzung fuer CMAKE: mindestens Version 2.8
cmake_minimum_required(VERSION 3.6)

if ( UNIX )
option(USE_X11 "Check this if you want to use the X11 backend" true)
ENDIF ( UNIX )
# Build-Typ
set(CMAKE_BUILD_TYPE Debug CACHE TYPE STRING FORCE )

#if ( UNIX )
#        # UNIX-like OS's, including Apple OS X and CygWin
#
#        #MESSAGE( STATUS "UNIX: " ${UNIX} )
#        # Gnu specific compiler flags
#        SET ( CMAKE_CXX_FLAGS
#             "${CMAKE_CXX_FLAGS} -Wall -O2 -march=native"
#        )
#ENDIF ( UNIX )


# OpenCV suchen
FIND_PACKAGE(OpenCV REQUIRED )

#QT suchen
find_package(Qt5 COMPONENTS  Core Network Widgets Gui REQUIRED)

#X11 suchen
if ( USE_X11 )
    add_compile_definitions(WITH_X11)
    FIND_PACKAGE(X11 REQUIRED)

    find_library(X11_Xrandr_LIB Xrandr)
    find_library(X11_Xfixes_LIB Xfixes)
    message(${X11_Xfixes_LIB})    
ENDIF ( USE_X11 )

find_library (lz4_lib lz4)
find_path (lz4_path /lz4.h)
message(${lz4_lib})
set(lz4_include_path /usr/local/include)

# Pfade für Include-Dateien
include_directories (
        ${QT_INCLUDES}
        ${OpenCV_INCLUDES}
        ${OpenCV_INCLUDE_DIRS}
        ${lz4_include_path}
)

# Fuege alle .cpp und alle .h Dateien aus dem aktuellen Ordner
# in das Projekt ein.
FILE ( GLOB SRC_FILES "*.cpp")
FILE ( GLOB HEADER_FILES "*.h")
LIST(APPEND HEADER_FILES "./../header.h")
LIST(APPEND HEADER_FILES "./../parameter.h")
LIST(APPEND HEADER_FILES "./../parameterserialization.h")
LIST(APPEND SRC_FILES "./../parameterserialization.cpp")

FILE ( GLOB UI_FILES "*.ui")
MESSAGE(${HEADER_FILES})



ADD_DEFINITIONS (
                ${QT_DEFINITIONS}
                )


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

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
                ${lz4_lib}
                ${X11_Xrandr_LIB}
                ${X11_Xfixes_LIB}

)


