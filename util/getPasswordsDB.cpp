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
	cout << "Usage: getPasswordsDB [key]" << endl;
}

int main(int argc, char** argv)
{
    uint8_t *key = (uint8_t *)"01234567899876543210";
    
    if(argc > 2)
        usage();
    else if(argc == 2)
        key = (uint8_t *)argv[1];
    
    
    uint32_t salt[] = {12345, 54321};
    map<string, string> users;
    int len;
    uint8_t *descifrado;
    
    PracticaCaso::SQLiteMap * SQLiteMap = new PracticaCaso::SQLiteMap("logins.db");
    users = SQLiteMap->getMap();
    AESUtil aesCrypt(key, salt);
    
    cout << "#############################################\n";
    cout << "#    Login password  database get util    #\n";
    cout << "#############################################\n";

    map<string,string>::iterator it;
    
    for(it = users.begin(); it != users.end(); it++)
    {
        len = (it->second).size();
        descifrado = aesCrypt.decrypt((uint8_t *)it->second.c_str(), &len);
        
        cout << "----------------------------" << endl;
        cout << "User: " << it->first << endl; 
        cout << "pass(decrypted): " << (char *)descifrado << endl;
        cout << "pass(encrypted): " << it->second << endl;
        
    }
    
    return 0;
}
