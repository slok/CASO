
CASO Practice(2011)
===================

About
-----
This is the CASO project of the group **CASO-01** in 2010-2011 for the CASO subject of 5th year of computer Engineering in Deusto university. 

Git repository
--------------- 
[Github](https://github.com/slok/CASO)

Authors
---------
* Iraide (Sharem) Diaz Orueta (iraide.diaz [at] gmail.com)
* Xabier (sLoK) Larrakoetxae Gallego (slok69 [at] gmail.com)

Restrictions
------------
This project only runs on GNU/Linux or Cygwin

Dependencies
------------
* [SQLite3](http://www.sqlite.org/)
* [OpenSSL(libcrypt)](http://www.openssl.org/)
    
Build
------
    
    $ mkdir ./build && cd ./build
    $ cmake ..  
    $ make
    
Execute
-------

    $ cd [our build directory of cmake]

###dns:

    $ cd ./dns
    $ ./NameServer 4000 es
    $ ./NameServer 1234 deusto.es
    $ ./NameClient 4000 eside.deusto.es

###mdns:

    $ cd ./mdns
    $ ./mNameServer 4000 es
    $ ./mNameServer 5555 com
    $ cd ../dns
    $ ./NameClient 4000 upv.com

###Dsm:

    $ cd ./mdns
    $ ./mNameServer 4000 es
    $ cd ../dsm
    $ ./DsmServer 1234
    $ ./DsmTimeServer 4000 deusto.es
    $ ./DsmTimeClient 4000 deusto.es

###Proposal1:
    
    cd ./dns
    //you can create logins with the utility
    $ cd ../util
    $ ./insertPasswordDB.bin 
    $ ./getPasswordsDB.bin

    //execute the proposal
    $ cd ../mdns
    $ ./mNameServer 4000 es
    $ cd  ../dns
    $ ./ProposalDnsServer.bin 1234
    $ ./ProposalDnsClient.bin 4000 deusto.es

###proposal2:

    $ cd ./mdns
    $ ./mNameServer 4000 es
    $ cd ../dsm
    $ ./DsmServer 1234
    $ ./ProposalDsmReferee 4000 deusto.es
    $ ./ProposalDsmClient 4000 deusto.es xabi
    $ ./ProposalDsmClient 4000 deusto.es iraide
        
License
-------
[GPL v3](http://www.gnu.org/licenses/gpl-3.0.html)
