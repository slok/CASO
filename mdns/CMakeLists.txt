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

#A variable of the folder for clearness
set(SRC_ROOT_MDNS ${PROJECT_SOURCE_DIR}/mdns)

#Set all the sources to build
set(SRC_MNAMESERVER
    ${SRC_ROOT_MDNS}/mNameServer.cc
    ${SRC_ROOT_MDNS}/mNameServer.h
)

#Set our dependency libs
set(DEPEND_LIBS
    ${PROJECT_BINARY_DIR}/util/libesidesocket.so 
    ${PROJECT_BINARY_DIR}/util/libesidethread.so
    ${PROJECT_BINARY_DIR}/util/libesidesqlite.so
)

#set the needed headers
set(INCLUDE_DIRS
    ${SRC_ROOT_MDNS}
    ${PROJECT_SOURCE_DIR}/util
)

include_directories(${INCLUDE_DIRS})

#build our executables

add_executable(mNameServer ${SRC_MNAMESERVER})

#link our executables
target_link_libraries(mNameServer ${DEPEND_LIBS})

#copy the name-mappings-files to the build directory
file(COPY ${SRC_ROOT_MDNS}/es  DESTINATION ${PROJECT_BINARY_DIR}/mdns)
file(COPY ${SRC_ROOT_MDNS}/deusto.es  DESTINATION ${PROJECT_BINARY_DIR}/mdns)
file(COPY ${SRC_ROOT_MDNS}/yahoo.es  DESTINATION ${PROJECT_BINARY_DIR}/mdns)
file(COPY ${SRC_ROOT_MDNS}/ehu.es  DESTINATION ${PROJECT_BINARY_DIR}/mdns)
file(COPY ${SRC_ROOT_MDNS}/upv.es  DESTINATION ${PROJECT_BINARY_DIR}/mdns)
file(COPY ${SRC_ROOT_MDNS}/tecnologico.deusto.es  DESTINATION ${PROJECT_BINARY_DIR}/mdns)
file(COPY ${SRC_ROOT_MDNS}/eside.deusto.es  DESTINATION ${PROJECT_BINARY_DIR}/mdns)
file(COPY ${SRC_ROOT_MDNS}/com  DESTINATION ${PROJECT_BINARY_DIR}/mdns)
file(COPY ${SRC_ROOT_MDNS}/upv.com  DESTINATION ${PROJECT_BINARY_DIR}/mdns)

