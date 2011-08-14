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
set(SRC_ROOT_DNS ${PROJECT_SOURCE_DIR}/dns)

#Set all the sources to build
set(SRC_NAMESERVER
    ${SRC_ROOT_DNS}/NameServer.cc 
    ${SRC_ROOT_DNS}/NameServer.h
)

set(SRC_NAMECLIENT
    ${SRC_ROOT_DNS}/NameClient.cc 
)

set(SRC_ECHOTCPLISTENER
    ${SRC_ROOT_DNS}/EchoTcpListener.cc
    ${SRC_ROOT_DNS}/EchoServerThread.cc 
    ${SRC_ROOT_DNS}/EchoServerThread.h
)

set(SRC_ECHOTCPCLIENT
    ${SRC_ROOT_DNS}/EchoTcpClient.cc
)

set(SRC_PROPOSALDNSCLIENT
    ${SRC_ROOT_DNS}/ProposalDnsClient.cpp 
)

set(SRC_PROPOSALDNSERVER
    ${SRC_ROOT_DNS}/ProposalDnsServerThread.cpp
    ${SRC_ROOT_DNS}/ProposalDnsServerThread.h
    ${SRC_ROOT_DNS}/ProposalDnsServer.cpp
)

#Set our dependency libs
set(DEPEND_LIBS
    ${PROJECT_BINARY_DIR}/util/libesidesocket.so 
    ${PROJECT_BINARY_DIR}/util/libesidethread.so
    ${PROJECT_BINARY_DIR}/util/libesidesqlite.so
)
set(DEPEND_AES_LIBS
    ${PROJECT_BINARY_DIR}/util/libcasoaes.so 
)

#set the needed headers
set(INCLUDE_DIRS
    ${SRC_ROOT_DNS}
    ${PROJECT_SOURCE_DIR}/util
)

include_directories(${INCLUDE_DIRS})

#build our executables
add_executable(EchoTcpListener ${SRC_ECHOTCPLISTENER})
add_executable(EchoTcpClient ${SRC_ECHOTCPCLIENT})
add_executable(NameServer ${SRC_NAMESERVER})
add_executable(NameClient ${SRC_NAMECLIENT})
add_executable(ProposalDnsClient ${SRC_PROPOSALDNSCLIENT})
add_executable(ProposalDnsServer ${SRC_PROPOSALDNSERVER})

#link our executables
target_link_libraries(NameServer ${DEPEND_LIBS})
target_link_libraries(EchoTcpListener ${DEPEND_LIBS})
target_link_libraries(EchoTcpClient ${DEPEND_LIBS})
target_link_libraries(NameClient ${DEPEND_LIBS})
target_link_libraries(ProposalDnsClient ${DEPEND_LIBS} ${DEPEND_AES_LIBS})
target_link_libraries(ProposalDnsServer ${DEPEND_LIBS} ${DEPEND_AES_LIBS})

#copy the name-mappings-files to the build directory
file(COPY ${SRC_ROOT_DNS}/es  DESTINATION ${PROJECT_BINARY_DIR}/dns)
file(COPY ${SRC_ROOT_DNS}/deusto.es  DESTINATION ${PROJECT_BINARY_DIR}/dns)
file(COPY ${SRC_ROOT_DNS}/yahoo.es  DESTINATION ${PROJECT_BINARY_DIR}/dns)
file(COPY ${SRC_ROOT_DNS}/ehu.es  DESTINATION ${PROJECT_BINARY_DIR}/dns)
file(COPY ${SRC_ROOT_DNS}/tecnologico.deusto.es  DESTINATION ${PROJECT_BINARY_DIR}/dns)
file(COPY ${SRC_ROOT_DNS}/eside.deusto.es  DESTINATION ${PROJECT_BINARY_DIR}/dns)

