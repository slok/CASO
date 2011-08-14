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
set(SRC_ROOT_UTIL ${PROJECT_SOURCE_DIR}/util)


#Set all the sources to build
set(SRC_SH
    ${SRC_ROOT_UTIL}/popen.cc
    ${SRC_ROOT_UTIL}/popen.h
    ${SRC_ROOT_UTIL}/MySh.cc
    ${SRC_ROOT_UTIL}/MySh.h
)

set(SRC_THREAD
    ${SRC_ROOT_UTIL}/Thread.cc
    ${SRC_ROOT_UTIL}/Thread.h
)

set(SRC_SOCKET
    ${SRC_ROOT_UTIL}/TcpListener.cc 
    ${SRC_ROOT_UTIL}/TcpListener.h
)

set(SRC_SQLITEMAP
    ${SRC_ROOT_UTIL}/SQLiteMap.cc 
    ${SRC_ROOT_UTIL}/SQLiteMap.h
)

set(SRC_ESIDEDSM
    ${SRC_ROOT_UTIL}/Dsm.cc 
    ${SRC_ROOT_UTIL}/Dsm.h 
    ${SRC_ROOT_UTIL}/DsmTypes.h
)

set(SRC_AESUTIL
    ${SRC_ROOT_UTIL}/AESUtil.cpp 
    ${SRC_ROOT_UTIL}/AESUtil.h 
    ${SRC_ROOT_UTIL}/types.h
)

set(SRC_TTTUTIL
    ${SRC_ROOT_UTIL}/ticTacToeUtil.cpp 
    ${SRC_ROOT_UTIL}/ticTacToeUtil.h 
    ${SRC_ROOT_UTIL}/colors.h
)
set(SRC_INSERTPASSWORDDB
    ${SRC_ROOT_UTIL}/insertPasswordDB.cpp
)
set(SRC_GETPASSWORDSDB
    ${SRC_ROOT_UTIL}/getPasswordsDB.cpp
)

#Find pthreads dependency, sqlite and dl (we need it)
find_package(Threads REQUIRED)
find_package(Sqlite REQUIRED)
find_package(DL REQUIRED)
find_package(OpenSSL REQUIRED)

#set the needed headers
set(INCLUDE_DIRS
    ${SRC_ROOT_UTIL}
    ${SQLITE_INCLUDE_DIR}
    ${DL_INCLUDE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)

include_directories(${INCLUDE_DIRS})

#Build the libs
add_library(esidesh SHARED ${SRC_SH})
add_library(esidesocket SHARED ${SRC_SOCKET})
add_library(esidethread SHARED ${SRC_THREAD})
add_library(esidesqlite SHARED ${SRC_SQLITEMAP})
add_library(esidedsm SHARED ${SRC_ESIDEDSM})
add_library(casoaes SHARED ${SRC_AESUTIL})
add_library(casotictactoe SHARED ${SRC_TTTUTIL})

#Set our dependency libs
set(DEPEND_DSM_LIBS
    esidesocket 
    esidethread
    esidedsm
)

add_executable(insertPasswordDB ${SRC_INSERTPASSWORDDB})
add_executable(getPasswordsDB ${SRC_GETPASSWORDSDB})

#Link the libs that require
target_link_libraries(esidethread ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(esidesqlite ${SQLITE_LIBRARIES} ${DL_LIBRARY})
target_link_libraries(esidedsm ${DEPEND_LIBS})
target_link_libraries(casoaes esidesqlite ${OPENSSL_LIBRARIES})
target_link_libraries(casotictactoe esidedsm)
target_link_libraries(insertPasswordDB casoaes)
target_link_libraries(getPasswordsDB casoaes)

#copy login database
file(COPY ${SRC_ROOT_UTIL}/logins.db  DESTINATION ${PROJECT_BINARY_DIR}/util)
file(COPY ${SRC_ROOT_UTIL}/logins.db  DESTINATION ${PROJECT_BINARY_DIR}/dns)
