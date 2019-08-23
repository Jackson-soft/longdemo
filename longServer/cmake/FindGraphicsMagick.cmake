# -*-cmake-*-
#
# Test for GraphicsMagick libraries, unlike CMake's FindGraphicsMagick.cmake
# which tests for GraphicsMagick's binary utilities
#
# Once loaded this will define MAGICK_FOUND        - system has GraphicsMagick
# MAGICK_INCLUDE_DIR  - include directory for GraphicsMagick MAGICK_LIBRARY_DIR
# - library directory for GraphicsMagick MAGICK_LIBRARIES    - libraries you
# need to link to
#
# MAGICK++_FOUND        - system has GraphicsMagick MAGICK++_INCLUDE_DIR  -
# include directory for GraphicsMagick MAGICK++_LIBRARY_DIR  - library directory
# for GraphicsMagick MAGICK++_LIBRARIES    - libraries you need to link to
#
# MAGICKWAND_FOUND        - system has GraphicsMagick MAGICKWAND_INCLUDE_DIR  -
# include directory for GraphicsMagick MAGICKWAND_LIBRARY_DIR  - library
# directory for GraphicsMagick MAGICKWAND_LIBRARIES    - libraries you need to
# link to
#

set(MAGICK_FOUND "NO")
set(MAGICK++_FOUND "NO")
set(MAGICKWAND_FOUND "NO")

find_path(
    MAGICK_INCLUDE_DIR
    NAMES "magick/magick.h"
    PATHS
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GraphicsMagick\\Current;BinPath]/include"
        "$ENV{MAGICK_LOCATION}"
        "$ENV{MAGICK_LOCATION}/include"
        "$ENV{MAGICK_LOCATION}/include/GraphicsMagick"
        "$ENV{MAGICK_HOME}/include"
        /usr/include/
        /usr/include/GraphicsMagick
        /usr/local/include
        /usr/local/include/GraphicsMagick
        /opt/local/include/GraphicsMagick)

find_path(
    MAGICK++_INCLUDE_DIR
    NAMES "Magick++/Magick++.h"
    PATHS
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GraphicsMagick\\Current;BinPath]/include"
        "$ENV{MAGICK++_LOCATION}"
        "$ENV{MAGICK++_LOCATION}/include"
        "$ENV{MAGICK_LOCATION}"
        "$ENV{MAGICK_LOCATION}/include"
        "$ENV{MAGICK_LOCATION}/include/GraphicsMagick"
        "$ENV{MAGICK_HOME}/include"
        /usr/include/
        /usr/include/GraphicsMagick
        /usr/local/include
        /usr/local/include/GraphicsMagick
        /opt/local/include/GraphicsMagick)

find_path(
    MAGICKWAND_INCLUDE_DIR
    NAMES "wand/magick_wand.h"
    PATHS
        "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GraphicsMagick\\Current;BinPath]/include"
        "$ENV{MAGICKWAND_LOCATION}"
        "$ENV{MAGICKWAND_LOCATION}/include"
        "$ENV{MAGICK_LOCATION}"
        "$ENV{MAGICK_LOCATION}/include"
        "$ENV{MAGICK_LOCATION}/include/GraphicsMagick"
        "$ENV{MAGICK_HOME}/include"
        /usr/include/
        /usr/include/GraphicsMagick
        /usr/local/include
        /usr/local/include/GraphicsMagick
        /opt/local/include/GraphicsMagick)

find_library(
    Magick
    NAMES GraphicsMagick CORE_RL_magick_
    PATHS "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GraphicsMagick\\Current;BinPath]/lib"
          "$ENV{MAGICK_LOCATION}/magick/.libs"
          "$ENV{MAGICK_LOCATION}/lib"
          "$ENV{MAGICK_HOME}/lib"
          /usr/lib64
          /usr/local/lib64
          /opt/local/lib64
          /usr/lib
          /usr/local/lib
          /opt/local/lib
    DOC "GraphicsMagick magic library")

find_library(
    Magick++
    NAMES GraphicsMagick++ CORE_RL_Magick++_
    PATHS "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GraphicsMagick\\Current;BinPath]/lib"
          "$ENV{MAGICK++_LOCATION}/.libs"
          "$ENV{MAGICK_LOCATION}/.libs"
          "$ENV{MAGICK++_LOCATION}/lib"
          "$ENV{MAGICK_LOCATION}/lib"
          "$ENV{MAGICK_HOME}/lib"
          /opt/local/lib64
          /usr/lib64
          /usr/local/lib64
          /opt/local/lib
          /usr/lib
          /usr/local/lib
    DOC "GraphicsMagick Magick++ library")

find_library(
    MagickWand
    NAMES GraphicsMagickWand CORE_RL_MagickWand_
    PATHS "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GraphicsMagick\\Current;BinPath]/lib"
          "$ENV{MAGICKWAND_LOCATION}/.libs"
          "$ENV{MAGICK_LOCATION}/.libs"
          "$ENV{MAGICKWAND_LOCATION}/lib"
          "$ENV{MAGICK_LOCATION}/lib"
          "$ENV{MAGICK_HOME}/lib"
          /opt/local/lib64
          /usr/lib64
          /usr/local/lib64
          /opt/local/lib
          /usr/lib
          /usr/local/lib
    DOC "GraphicsMagick MagickWand library")

set(MAGICK_LIBRARIES ${Magick})
set(MAGICK++_LIBRARIES ${Magick++})
set(MAGICKWAND_LIBRARIES ${MagickWand})

if(MAGICK_INCLUDE_DIR)
    if(MAGICK_LIBRARIES)
        set(MAGICK_FOUND "YES")
        get_filename_component(MAGICK_LIBRARY_DIR ${Magick} PATH)
    endif(MAGICK_LIBRARIES)
endif(MAGICK_INCLUDE_DIR)

if(MAGICK++_INCLUDE_DIR)
    if(MAGICK++_LIBRARIES)
        set(MAGICK++_FOUND "YES")
        get_filename_component(MAGICK++_LIBRARY_DIR ${Magick++} PATH)
    endif(MAGICK++_LIBRARIES)
endif(MAGICK++_INCLUDE_DIR)

if(MAGICKWAND_INCLUDE_DIR)
    if(MAGICKWAND_LIBRARIES)
        set(MAGICKWAND_FOUND "YES")
        get_filename_component(MAGICKWAND_LIBRARY_DIR ${MagickWand} PATH)
    endif(MAGICKWAND_LIBRARIES)
endif(MAGICKWAND_INCLUDE_DIR)

if(NOT MAGICK_FOUND)
    # make FIND_PACKAGE friendly
    if(NOT Magick_FIND_QUIETLY)
        if(Magick_FIND_REQUIRED)
            message(
                FATAL_ERROR
                    "GraphicsMagick required, please specify it's location with MAGICK_HOME, MAGICK_LOCATION or MAGICK++_LOCATION"
                )
        else(Magick_FIND_REQUIRED)
            message(STATUS "GraphicsMagick was not found.")
        endif(Magick_FIND_REQUIRED)
    endif(NOT Magick_FIND_QUIETLY)
endif(NOT MAGICK_FOUND)

#
