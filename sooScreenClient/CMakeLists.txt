# Project name
set(PROJECT_NAME screenClient)
project( ${PROJECT_NAME} )


cmake_minimum_required(VERSION 3.7)

# Build type
set(CMAKE_BUILD_TYPE Debug CACHE TYPE STRING FORCE )



#OpenCV
FIND_PACKAGE(OpenCV REQUIRED )

#QT
find_package(Qt5 COMPONENTS  Core Network Widgets Gui OpenGL REQUIRED)

#X11
FIND_PACKAGE(X11 REQUIRED)

#OpenGL
set(OPENGL_GL_PREFERENCE "LEGACY" CACHE STRING
        "Set OpenGL preference")
set(OpenGL_GL_PREFERENCE ${OPENGL_GL_PREFERENCE})
find_package(OpenGL REQUIRED)

#LZ4
find_library (lz4_lib lz4)
find_path (lz4_path /lz4.h)
message(${lz4_lib})
set(lz4_include_path /usr/local/include)
message(${lz4_include_path})



# Include paths
include_directories (
        ${QT_INCLUDES}
        ${OpenCV_INCLUDES}
        ${OpenCV_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
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
        ${QT_LIBRARIES}                
                ${Qt5Widgets_LIBRARIES}
                ${Qt5Network_LIBRARIES}
                ${Qt5Core_LIBRARIES}
                ${Qt5Gui_LIBRARIES}
                ${OPENGL_LIBRARIES}
                ${lz4_lib}
)




