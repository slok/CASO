/*
Copyright (C) 2011 Iraide Diaz (Sharem)
Copyright (C) 2011 Xabier Larrakoetxea (slok)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include "AESUtil.h"
#include "SQLiteMap.h" 
#include "types.h"

void usage() {
	cout << "Usage: insertPasswordDB [key]" << endl;
}

int main(int argc, char** argv)
{
    uint8_t *key = (uint8_t *)"01234567899876543210";
    
    if(argc > 2)
        usage();
    else if(argc == 2)
        key = (uint8_t *)argv[1];
    
    
    uint32_t salt[] = {12345, 54321};
    string user, pass;
    uint8_t *testText = NULL;
    int len;
    uint8_t *cifrado;
    bool exit = false;
    string aux ;
    PracticaCaso::SQLiteMap * SQLiteMap = new PracticaCaso::SQLiteMap("logins.db");
    AESUtil aesCrypt(key, salt);
    cout << "#############################################\n";
    cout << "#    Login password  database store util    #\n";
    cout << "#############################################\n";

    while(!exit)
    {
        cout << "Insert login: ";
        cin >> user;
        cout << "Insert password: ";
        cin >> pass;
        
        len = pass.size();
        testText = (uint8_t *)pass.c_str();
        cifrado = aesCrypt.encrypt(testText, &len);
        SQLiteMap->set(user.c_str(), (char *)cifrado);
        
        cout << "Inserted in database!" << endl;
        cout << "continue?[y/n]";
        cin >> aux;
        
        if(strcmp(aux.c_str(),"n") == 0)
            exit = true;
    }
    return 0;
}
