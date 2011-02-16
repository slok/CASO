// SQLiteMap.cc
// author: dipina@eside.deusto.es
// compile: g++ SQLiteMap.cc -lsqlite3 -o SQLiteMap
#include "SQLiteMap.h"

namespace PracticaCaso {
	SQLiteMap::SQLiteMap(string fn): fileName(fn), dbh(0) {
		// Process the contents of the mapping file
		this->loadMappings(fn);
    }

	SQLiteMap::SQLiteMap(const SQLiteMap& rhs) {
		fileName = rhs.fileName;
		dbh = rhs.dbh;
		this->loadMappings(fileName);
	}

	SQLiteMap::~SQLiteMap() {
		cout << "SQLiteMap: destructor called " << endl;
		this->close();
	}

	void SQLiteMap::loadMappings(string mappingsDBFileName) {
		// Loads the mappings stored at SQLite DB into the map loadMappings
		// In the case that the DB does not exist, create it, its structure is given by file KeyValueDB.sql
		// If a select * from KeyValuePair executed through a sqlite3_get_table does not return SQLITE_OK, it means that the table does not exist, and needs being created
		// If there are unexpected error exit the program with exit(1)
        /////////////////////////////////////////////////////////////////////////open
        cout << "Opening DnsMap database ..." << endl;
        if (sqlite3_open(mappingsDBFileName.c_str(), &dbh) != SQLITE_OK) {
            cerr << "Can't open database: " << sqlite3_errmsg(dbh) << endl;
            sqlite3_close(dbh);
            exit(1);
        }
        ////////////////////////////////////////////////////////////////////////
        char **result;
        int nrow;
        int ncol;
        char *errorMsg;
        cout << "Checking if KeyValuePair table already exists ..." << endl;
        if (sqlite3_get_table(dbh, "select * from KeyValuePair", &result, &nrow, &ncol, &errorMsg) != SQLITE_OK) {
            cerr << errorMsg << endl;
            sqlite3_free(errorMsg);
            if (sqlite3_get_table(dbh, "create table KeyValuePair(key_element BLOB NOT NULL PRIMARY KEY, value_element BLOB)", &result, &nrow, &ncol, &errorMsg) != SQLITE_OK) {
                cerr << errorMsg << endl;
                sqlite3_free(errorMsg);
                sqlite3_close(dbh);
                exit(1);
            } else {
                cout << "Table KeyValuePair created" << endl;
                sqlite3_free_table(result);
            }
        }
        else
        {
            if (sqlite3_get_table(dbh, "select * from KeyValuePair", &result, &nrow, &ncol, &errorMsg) != SQLITE_OK) {
                cerr << errorMsg << endl;
                sqlite3_free(errorMsg);
            } else {
                // sqlite3 returns one extra row with the column headers
                
                for (int i=1; i<=nrow; i++) 
                {
                    this->dns2IpPortMap[result[i*ncol]] = result[i*ncol+1];
                }
                sqlite3_free_table(result);
            }
        }
	}

	
	map<string, string> SQLiteMap::getMap() {
		return this->dns2IpPortMap;
	}


	string SQLiteMap::get(string key) {
        //return null;
        map<string,string>::iterator iter;
        iter=this->dns2IpPortMap.find(key);
        if (iter != this->dns2IpPortMap.end() )
        { 
            return iter->second;
        }
        else
            return "";
	}

	void SQLiteMap::set(string mapKey, string mapValue) {
		// Undertake the update of the STL map and the database. Bear in mind that it there is not an entry with a passed key an INSERT will have to be executed, if there was already such an entry an UPDATE will take place
        char **result;
        int nrow;
        int ncol;
        char *errorMsg;
        string sqliteStr = "";
        typedef map<string, string>::const_iterator CI;
        CI iter;
        iter = this->dns2IpPortMap.find(mapKey);
        
        if (iter != this->dns2IpPortMap.end() ) //values is in the map, so we update
            sqliteStr = "update KeyValuePair set value_element='"+mapValue+"' where key_element='"+mapKey+"'";
        else //value isn't in the map, so we insert
            sqliteStr = "insert into KeyValuePair values('"+mapKey+"', '"+mapValue+"')";
            
        if (sqlite3_get_table(dbh, sqliteStr.c_str(), &result, &nrow, &ncol, &errorMsg) != SQLITE_OK) {
            cerr << errorMsg << endl;
            sqlite3_free(errorMsg);
            sqlite3_close(dbh);
            exit(1);
        } else {
            cout << "Row created: " << nrow << endl;
            this->dns2IpPortMap[mapKey] = mapValue;
            sqlite3_free_table(result);
        }
        
        
    }


	void SQLiteMap::close() {
		sqlite3_close(this->dbh);
	}

	ostream & operator << (ostream & os, SQLiteMap &t) {
		os << "DB file name: " << t.fileName << endl;
		os << "DNS mappings:" << endl;
		typedef map<string, string>::const_iterator CI;
		for (CI p = t.dns2IpPortMap.begin(); p != t.dns2IpPortMap.end(); ++p) {
			os <<  p->first << " : " << p->second << endl;
		}
		return os;
	}
}


/*
// global variable
PracticaCaso::SQLiteMap * SQLiteMap_pointer;

// function called when CTRL-C is pressed
void ctrl_c(int)
{
    printf("\nCTRL-C was pressed...\n");
	delete SQLiteMap_pointer;
}


void usage() {
	cout << "Usage: SQLiteMap <name-mappings-db-file>" << endl;
	exit(1);
}

int main(int argc, char** argv) {
	signal(SIGINT,ctrl_c);

	if (argc != 2) {
		usage();
	}

	PracticaCaso::SQLiteMap * SQLiteMap = new PracticaCaso::SQLiteMap((string)argv[1]);
	cout << "SQLiteMap instance: " << endl << SQLiteMap << endl;
	//SQLiteMap_pointer = &SQLiteMap;
	SQLiteMap_pointer = SQLiteMap;

	//SQLiteMap.set("saludo", "hola");
	//SQLiteMap.set("despedida", "adios");
	SQLiteMap->set("saludo", "hola");
	SQLiteMap->set("despedida", "adios");

	//cout << "SQLiteMap[\"saludo\"] = " << SQLiteMap.get("saludo") << endl;
	//cout << "SQLiteMap[\"despedida\"] = " << SQLiteMap.get("despedida") << endl;
	cout << "SQLiteMap[\"saludo\"] = " << SQLiteMap->get("saludo") << endl;
	cout << "SQLiteMap[\"despedida\"] = " << SQLiteMap->get("despedida") << endl;

	cout << "SQLiteMap instance after 2 sets: " << endl << *SQLiteMap << endl;

	//map<string, string> savedSQLiteMap = SQLiteMap.getMap();
	map<string, string> savedSQLiteMap = SQLiteMap->getMap();
	cout << "Recuperado el mapa" << endl;
	typedef map<string, string>::const_iterator CI;
	for (CI p = savedSQLiteMap.begin(); p != savedSQLiteMap.end(); ++p) {
		cout << "dns2IpPortMap[" << p->first << "] = " << p->second << endl;
	}
	
	delete SQLiteMap_pointer;
    
}*/


