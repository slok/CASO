#
# CASO cmake build script
# (C) 2011 - Xabier (sLoK) Larrakoetxea <slok69@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or 
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
# 


cmake_minimum_required(VERSION 2.8)

# project name
project(CASO)

#if you want to patch the sources (only the first time)
#call cmake like this: cmake -DPATCHSRC=ON ..
#patch our sources?? (if no flag was provided then disable for security)
if(NOT PATCHSRC)
    set(PATCHSRC OFF)
else()
    set(GNULINUX_PATCH_FILE  ${PROJECT_SOURCE_DIR}/patches/gnuLinux.patch)
    set(NAMESPACE_PATCH_FILE  ${PROJECT_SOURCE_DIR}/patches/namespace.patch)
endif()

# define the path of our additional CMake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-modules)

# define the build type
set(CMAKE_BUILD_TYPE Release)

# add the header path
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

#install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/src/box2dsfml DESTINATION ${PROJECT_SOURCE_DIR})

if(PATCHSRC)
    if(UNIX)
        #Apply linux patch
        execute_process(COMMAND patch -p1
                        INPUT_FILE ${GNULINUX_PATCH_FILE}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
        message("-- Patch ${GNULINUX_PATCH_FILE} - done")
    endif()
    
    #Apply global patch
    execute_process(COMMAND patch -p1
                        INPUT_FILE ${NAMESPACE_PATCH_FILE}
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
    message("-- Patch ${NAMESPACE_PATCH_FILE} - done")
endif()

# add the subdirectories
add_subdirectory(util)
add_subdirectory(dns)
add_subdirectory(mdns)
add_subdirectory(dsm)
