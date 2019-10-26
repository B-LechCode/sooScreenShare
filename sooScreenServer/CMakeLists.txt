# Project name
set(PROJECT_NAME screenServer)
project( ${PROJECT_NAME} )


cmake_minimum_required(VERSION 3.7)

#options
option(USE_SDL2 "Check this if you want to use the SDL backend" true)

if ( UNIX )
option(USE_X11 "Check this if you want to use the X11 backend" true)
ENDIF ( UNIX )
if ( WIN32 )
option(USE_WINAPI "Check this if you want to use the WINAPI backend" true)
ENDIF ( WIN32 )
# Build-Typ
set(CMAKE_BUILD_TYPE Debug CACHE TYPE STRING FORCE )

#Build flags
#if ( UNIX )
#        # UNIX-like OS's, including Apple OS X and CygWin
#
#        #MESSAGE( STATUS "UNIX: " ${UNIX} )
#        # Gnu specific compiler flags
#        SET ( CMAKE_CXX_FLAGS
#             "${CMAKE_CXX_FLAGS} -Wall -O2 -march=native"
#        )
#ENDIF ( UNIX )


#OpenCV
FIND_PACKAGE(OpenCV REQUIRED )

#QT
find_package(Qt5 COMPONENTS  Core Network Widgets Gui REQUIRED)

#SDL
IF (USE_SDL2)
FIND_PACKAGE(SDL2 REQUIRED )
add_compile_definitions(WITH_SDL)
ENDIF(USE_SDL2)

#X11
IF ( USE_X11 )
    add_compile_definitions(WITH_X11)
    FIND_PACKAGE(X11 REQUIRED)

    find_library(X11_Xrandr_LIB Xrandr)
    find_library(X11_Xfixes_LIB Xfixes)
    message(${X11_Xfixes_LIB})
ENDIF ( USE_X11 )

if ( USE_WINAPI )
    add_compile_definitions(WITH_WINAPI)
ENDIF ( USE_WINAPI )

#LZ4
find_library (lz4_lib lz4)
find_path (lz4_path /lz4.h)
message(${lz4_lib})
set(lz4_include_path /usr/local/include)

# Include paths
include_directories (
        ${QT_INCLUDES}
        ${SDL2_INCLUDES}
        ${OpenCV_INCLUDES}
        ${OpenCV_INCLUDE_DIRS}
        ${lz4_include_path}
        ${lz4_path}
)

# Use all .cpp and .h files
FILE ( GLOB SRC_FILES "*.cpp")
FILE ( GLOB HEADER_FILES "*.h")

#Add specific files
LIST(APPEND HEADER_FILES "./../header.h")
LIST(APPEND HEADER_FILES "./../parameter.h")
LIST(APPEND HEADER_FILES "./../iparameter.h")
LIST(APPEND HEADER_FILES "./../parameterserialization.h")
LIST(APPEND HEADER_FILES "./../guihelpers.h")
LIST(APPEND HEADER_FILES "./../iparameter.cpp")
LIST(APPEND SRC_FILES "./../parameterserialization.cpp")
LIST(APPEND SRC_FILES "./../guihelpers.cpp")

#Add all .ui files
FILE ( GLOB UI_FILES "*.ui")




ADD_DEFINITIONS (
                ${QT_DEFINITIONS}
                )


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

#Create the executable
ADD_EXECUTABLE ( ${PROJECT_NAME}
        ${SRC_FILES}
        ${HEADER_FILES}
        ${UI_FILES}
)

#Linker settings
TARGET_LINK_LIBRARIES ( ${PROJECT_NAME}
        ${OpenCV_LIBS}
        ${X11_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${QT_LIBRARIES}                
                ${Qt5Widgets_LIBRARIES}
                ${Qt5Network_LIBRARIES}
                ${Qt5Core_LIBRARIES}
                ${Qt5Gui_LIBRARIES}
                ${lz4_lib}
                # ${X11_Xrandr_LIB}
                # ${X11_Xfixes_LIB}

)


