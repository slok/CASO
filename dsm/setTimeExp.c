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

extern "C" {
	#include <sys/time.h>
}

#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>

using namespace std;

//#include "TcpListener.h"
//#include "Dsm.h"

//#define BUFFER_SIZE 1024

void usage() {
	cout << "Usage: DsmTimeServer <port> <dns>" << endl;
}

time_t convertTime(string myStr)
{
    char *time[6];
    char *tokens;
    int i = 0;
    struct tm brokenTime;
    time_t actualTime;
    
    tokens = strtok((char *)myStr.c_str()," ,-:");
    
    //put in the array all the tokens: day, month, year, hour, min, sec
    while (tokens != NULL)
    {
        time[i] = tokens;
        tokens = strtok(NULL, " ,-:");
        i++;
    }
    
    brokenTime.tm_mday = atoi(time[0]);
    brokenTime.tm_mon = atoi(time[1])-1; //from 0 to 11
    brokenTime.tm_year = atoi(time[2]) - 1900; //from 1900
    brokenTime.tm_hour = atoi(time[3]);
    brokenTime.tm_min = atoi(time[4]);
    brokenTime.tm_sec = atoi(time[5]);
    
    actualTime = mktime(&brokenTime);
    return actualTime;
}


int main(int argc, char** argv) {
    char *tmp="23-03-2011, 20:07:41";
    string str = (string)tmp;
    struct timezone tzp;
    struct timeval tmv;
    //convert string to time_t
    tmv.tv_sec = convertTime(str);
    
    //set time
    settimeofday(&tmv, &tzp);

    //comprobacion
    char buffer[100];
    strftime(buffer,100,"%d-%m-%Y, %H:%M:%S",localtime(&(tmv.tv_sec)));
    cout << buffer << endl;
}
