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
set(SRC_ROOT_DSM ${PROJECT_SOURCE_DIR}/dsm)
set(SRC_ROOT_UTIL ${PROJECT_SOURCE_DIR}/util)
 
#Set all the sources to build

set(SRC_DSMSERVER
    ${SRC_ROOT_DSM}/DsmServer.cc 
    ${SRC_ROOT_DSM}/DsmServer.h 
    ${SRC_ROOT_UTIL}/DsmTypes.h
)

set(SRC_DSMTEST
    ${SRC_ROOT_DSM}/DsmTest.cc
)

set(SRC_MATRIXEVENADDER
    ${SRC_ROOT_DSM}/MatrixEvenAdder.cc
)

set(SRC_MATRIXODDADDER
    ${SRC_ROOT_DSM}/MatrixOddAdder.cc
)

set(SRC_DSMTIMESERVER
    ${SRC_ROOT_DSM}/DsmTimeServer.cc
)

set(SRC_DSMTIMECLIENT
    ${SRC_ROOT_DSM}/DsmTimeClient.cc

)

set(SRC_PROPOSALDSMCLIENT
    ${SRC_ROOT_DSM}/ProposalDsmClient.cpp
)

set(SRC_PROPOSALDSMREFEREE
    ${SRC_ROOT_DSM}/ProposalDsmReferee.cpp
)


#Set our dependency libs
set(DEPEND_LIBS
    ${PROJECT_BINARY_DIR}/util/libesidesocket.so 
    ${PROJECT_BINARY_DIR}/util/libesidethread.so
    ${PROJECT_BINARY_DIR}/util/libesidedsm.so
)
set(DEPEND_PROPOSAL_TTT
    ${PROJECT_BINARY_DIR}/util/libcasotictactoe.so
)

set(DEPEND_PROPOSAL_SQLITE
    ${PROJECT_BINARY_DIR}/util/libesidesqlite.so
)


#set the needed headers
set(INCLUDE_DIRS
    ${SRC_ROOT_DSM}
    ${PROJECT_SOURCE_DIR}/util
)

include_directories(${INCLUDE_DIRS})


#build our executables
add_executable(DsmServer ${SRC_DSMSERVER})
add_executable(DsmTest ${SRC_DSMTEST})
add_executable(MatrixEvenAdder ${SRC_MATRIXEVENADDER})
add_executable(MatrixOddAdder ${SRC_MATRIXODDADDER})  
add_executable(DsmTimeServer ${SRC_DSMTIMESERVER})
add_executable(DsmTimeClient ${SRC_DSMTIMECLIENT}) 
add_executable(ProposalDsmClient ${SRC_PROPOSALDSMCLIENT}) 
add_executable(ProposalDsmReferee ${SRC_PROPOSALDSMREFEREE})  

#link our executables
target_link_libraries(DsmServer ${DEPEND_LIBS})
target_link_libraries(DsmTest ${DEPEND_LIBS})
target_link_libraries(MatrixEvenAdder ${DEPEND_LIBS})
target_link_libraries(MatrixOddAdder ${DEPEND_LIBS})
target_link_libraries(DsmTimeServer ${DEPEND_LIBS})
target_link_libraries(DsmTimeClient ${DEPEND_LIBS})
target_link_libraries(ProposalDsmClient ${DEPEND_LIBS} ${DEPEND_PROPOSAL_TTT})
target_link_libraries(ProposalDsmReferee ${DEPEND_LIBS} ${DEPEND_PROPOSAL_TTT} ${DEPEND_PROPOSAL_SQLITE})

#copy dsm lib to util directory (The file isn't generated after comiling, so we need to copy like a command)
#NOT NECCESARY, NOW IS COMPILED IN UTIL!
#add_custom_command( TARGET esidedsm
#                    POST_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E
#                    copy   ${PROJECT_BINARY_DIR}/dsm/libesidedsm.so  
#                            ${PROJECT_BINARY_DIR}/util
#)

