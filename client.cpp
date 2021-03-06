/***************************************************************************
 
    file                 : client.cpp
    copyright            : (C) 2007 Daniele Loiacono
	
 ***************************************************************************/

 /***************************************************************************
 * Modifications:                                                          *
 * DATE         AUTHOR          CHANGES                                    *
 * ------------------------------------------------------------------------*
 * 2011-10-29	Stefan Moebius	Changes to the orginal are marked          *
								with //EDIT DATE                           *
 * 2012-04-13   Robin Lehmann   Some small changes, not really marked      *
 ***************************************************************************/
 
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/* Uncomment the following lines under windows */
//#define WIN32 // maybe not necessary because already define

#ifdef WIN32
#define __DRIVER_CLASS__ SimpleDriver     // put here the name of your driver class //EDIT 2011-10-29 moved in WIN32 section
#define __DRIVER_INCLUDE__ "SimpleDriver.h" // put here the filename of your driver h\\eader //EDIT 2011-10-29 moved in WIN32 section
#include <WinSock.h>
#else
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "controller.h"

// Hier eigenen Controller includen!


#include __DRIVER_INCLUDE__

/*** defines for UDP *****/
#define UDP_MSGLEN 1000
#define UDP_CLIENT_TIMEUOT 1000000
//#define __UDP_CLIENT_VERBOSE__
/************************/

#ifdef WIN32
typedef sockaddr_in tSockAddrIn;
#define CLOSE(x) closesocket(x)
#define INVALID(x) x == INVALID_SOCKET
#else
typedef int SOCKET;
typedef struct sockaddr_in tSockAddrIn;
#define CLOSE(x) close(x)
#define INVALID(x) x < 0
#endif

class __DRIVER_CLASS__;
typedef __DRIVER_CLASS__ tDriver;


using namespace std;

void parse_args(int argc, char *argv[], char *hostName, unsigned int &serverPort, char *id, unsigned int &maxEpisodes,
		  unsigned int &maxSteps, char *trackName, BaseDriver::tstage &stage);

int startClient(int argc, char* argv[], Controller* cntrl, string logfile, vector<pair<CarState, CarControl> >* path, bool manual, bool logging);






// Funktion zum laden von Pfaden. Der Parameter @file gibt an, wo sich die Datei befindet. Der Inhalt muss das Format CarState\nCarControl\n haben.

vector<pair<CarState, CarControl> > generatePath(string file)
{
    vector<pair<CarState, CarControl> > path;
    char s[1000];
    fstream logfile;    
    logfile.open(file.c_str(),fstream::in);

    cout << "Loading data from " << file << endl;

    for(unsigned int i = 0; !logfile.eof(); i++)
    {
        stringstream ss;
        logfile.getline(s, 1000, '\n');
        ss << s;
        pair<CarState, CarControl> value;
        value.first = CarState(ss.str());
        logfile.getline(s, 1000, '\n');
        ss.clear();
        ss.str(string());        
        ss << s;
        value.second = CarControl(ss.str());
        path.push_back(value);
    }
    cout << "data loaded" << endl;

    logfile.close();

    return path;
}



// Ab hier beginnt das Hauptprogramm. Die Menüpunkte sind nur Beispiele.

// Der Ablauf ist wie folgt:
// Man erstellt sich einen Controller, initialisiert diesen und füttert ihm Daten, wenn man das möchte.
// Alternativ kann man auch Manuel fahren, dann benötigt man keinen Controller.

// Dann wählt man für die Funtion:
// int startClient(int argc, char* argv[], Controller* cntrl, string logfile, vector<pair<CarState, CarControl> >* path, bool manual, bool logging);
// Die entsprechenden Parameter, einige mögliche Aufrufe sind unten angegeben.
// Mehr Erläuterungen bei den einzelnen Fällen.
// Alles nach der Mainmethode braucht normalerweise nicht geändert werden.


int main(int argc, char *argv[]) {
    int input;
    // set default files

    // sourcefile und logfile dienen zum Speichern der Pfade für die Dateien.
    string sourcefile = "./sourcefile.txt";
    string logfile = "./logfile.txt";

    // An dieser Stelle kann auch ein anderer Controller verwandt werden. 
    // Das Laden des Codes in den #includes am Anfang der Datei nicht vergessen.
    // In Windows muss der eigene Controller im Projekt sein, unter Linux in die Zeile:
    // OBJECTS = WrapperBaseDriver.o SimpleParser.o CarState.o CarControl.o keyboard.o controller.o $(DRIVER_OBJ)
    // nach controller.o als Dateiname.o eintragen, sonst gehts nicht.
    
    Controller* cntrl;
    cntrl = new Controller();

    do {
        cout << "   Option Menu" << endl;
        cout << "1. Start client (manual control)" << endl;
        cout << "2. Start client (manual control) with logging" << endl;
        cout << "3. Start " << cntrl->getControllerName() << " (automatic control)" << endl;
        cout << "4. Start " << cntrl->getControllerName() << " (automatic control) with logging" << endl;
        cout << "5. Start " << cntrl->getControllerName() << " using a preloaded path" << endl;
        cout << "6. Start " << cntrl->getControllerName() << " using a preloaded path with logging" << endl;
        cout << "7. Change file config" << endl;
        cout << "8. Change controller config" << endl;
        cout << "0. to quit." << endl;
        
        cin >> input;
        
        if(input == 1) 
        {
            // Für Fall 1 werden die Standardparameter übergeben, dann kein Controller (NULL), der string logfile, kein Pfad (NULL)
            // manuelle Steuerung ist aktiviert (true) und logging ist ausgeschaltet (false)

            startClient(argc, argv, NULL, logfile, NULL, true, false);
        }
        else if(input == 2) 
        {
            // Für Fall 2 werden die Standardparameter übergeben, dann kein Controller (NULL), der string logfile, kein Pfad (NULL)
            // manuelle Steuerung ist aktiviert (true) und logging ist angeschaltet (true)

            startClient(argc, argv, NULL, logfile, NULL, true, true);
        }
        else if(input == 3)
        {
            // Für Fall 3 werden die Standardparameter übergeben, der oben initialisierte Controller (cntrl), der string logfile, kein Pfad (NULL)
            // manuelle Steuerung ist aus (false) und logging ist ausgeschaltet (false)

            startClient(argc, argv, cntrl, logfile, NULL, false, false);
        }
        else if(input == 4)
        {
            // Für Fall 4 werden die Standardparameter übergeben, der oben initialisierte Controller (cntrl), der string logfile, kein Pfad (NULL)
            // manuelle Steuerung ist aus (false) und logging ist angeschaltet (true)

            startClient(argc, argv, cntrl, logfile, NULL, false, true);
        }
        else if(input == 5)
        {
            // Für Fall 5 werden die Standardparameter übergeben, der oben initialisierte Controller (cntrl), der string logfile, die Addresse des Pfades path (&path)
            // manuelle Steuerung ist aus (false) und logging ist ausgeschaltet (false)

            vector<pair<CarState, CarControl> > path = generatePath(sourcefile);
            startClient(argc, argv, cntrl, logfile, &path, false, false);
            path.clear();
        }
        else if(input == 6)
        {          
            // Für Fall 6 werden die Standardparameter übergeben, der oben initialisierte Controller (cntrl), der string logfile, die Addresse des Pfades path (&path)
            // manuelle Steuerung ist aus (false) und logging ist angeschaltet (true)

            vector<pair<CarState, CarControl> > path = generatePath(sourcefile);
            startClient(argc, argv, cntrl, logfile, &path, false, true);
            path.clear();
        }
        else if(input == 7)
        {
            // Ich hoffe die englischen Ausgaben machen hier klar, was in diesem Teil des Codes passiert

            cout << "   Fileconfig Menu" << endl;
            cout << "1. Change source file for automatic driving." << endl;
            cout << "2. Change path and name of the log file." << endl;
            cout << "0. Back." << endl;
            cin >> input;
            if(input == 1)
            {
                cout << "Please enter an operation system conform string for the sourcefile. (e.g. default: ./logs/sourcefile.txt)" << endl;
                string tmpfile;
                cin >> tmpfile;
                bool worked = false;
                cout << "Testing " << tmpfile << endl;
                try
                {
                    vector<pair<CarState, CarControl> > path = generatePath(tmpfile);
                    worked = true;
                    path.clear();
                }
                catch(exception e)
                {
                    cout << "invalid string " << tmpfile << endl;
                }
                if(worked)
                    sourcefile = tmpfile;
            }
            else if(input == 2)
            {
                cout << "Please enter an operation system conform string for the logfile, beware it will be OVERWRITTEN. (e.g. default: ./logs/logfile.txt)" << endl;
                string tmpfile;
                cin >> tmpfile;
                bool worked = false;
                cout << "Testing " << tmpfile << endl;
                try
                {
                    vector<pair<CarState, CarControl> > path = generatePath(tmpfile);
                    worked = true;
                    path.clear();
                }
                catch(exception e)
                {
                    cout << "invalid string " << tmpfile << endl;
                }
                if(worked)
                    logfile = tmpfile;
            }
        }
        // Hier kann man sich einen Controller aussuchen, der dann für die folgenden Rennen benutzt wird.
        else if(input == 8)
        {
            cout << "   Please choose a Controller:" << endl;
            cout << "1. Standard Controller (controller.h/cpp)" << endl;
            cout << "0. Back" << endl;

            cin >> input;
            if (input == 1)
            {
                cntrl = new Controller();
            }
        }

    }
    while(input != 0);
    
    delete cntrl;    
}

// Im restlichen Teil des Codes wird der Datentransfer zwischen dem TORCs Server und dem Client hier geregelt. 
// Außerdem wird eine Driver Instanz d angelegt. An diese werden dann die Daten vom Server weitergeleitet und dessen Controlanweisungen werden an den Server gesandt.


int startClient(int argc, char *argv[], Controller* cntrl, string logfile, vector<pair<CarState, CarControl> >* path = NULL, bool manual = false, bool logging = false)
{
    SOCKET socketDescriptor;
    int numRead;

    char hostName[1000];
    unsigned int serverPort;
    char id[1000];
    unsigned int maxEpisodes;
    unsigned int maxSteps;
//    bool noise;
//    double noiseAVG;
//    double noiseSTD;
//    long seed;
    char trackName[1000];
    BaseDriver::tstage stage;

    tSockAddrIn serverAddress;
    struct hostent *hostInfo;
    struct timeval timeVal;
    fd_set readSet;
    char buf[UDP_MSGLEN];


#ifdef WIN32 
     /* WinSock Startup */

     WSADATA wsaData={0};
     WORD wVer = MAKEWORD(2,2);
     int nRet = WSAStartup(wVer,&wsaData);

     if(nRet == SOCKET_ERROR)
     {
 	std::cout << "Failed to init WinSock library" << std::endl;
	exit(1);
     }
#endif

//    parse_args(argc,argv,hostName,serverPort,id,maxEpisodes,maxSteps,noise,noiseAVG,noiseSTD,seed,trackName,stage);

    parse_args(argc,argv,hostName,serverPort,id,maxEpisodes,maxSteps,trackName,stage);

//    if (seed>0)
//    	srand(seed);
//    else
//    	srand(time(NULL));

    hostInfo = gethostbyname(hostName);
    if (hostInfo == NULL)
    {
        cout << "Error: problem interpreting host: " << hostName << "\n";
        exit(1);
    }

    // Print command line option used
    cout << "***********************************" << endl;

    cout << "HOST: "   << hostName    << endl;

    cout << "PORT: " << serverPort  << endl;

    cout << "ID: "   << id     << endl;

    cout << "MAX_STEPS: " << maxSteps << endl; 

    cout << "MAX_EPISODES: " << maxEpisodes << endl;

//    if (seed>0)
//    	cout << "SEED: " << seed << endl;

    cout << "TRACKNAME: " << trackName << endl;

    if (stage == BaseDriver::WARMUP)
		cout << "STAGE: WARMUP" << endl;
	else if (stage == BaseDriver::QUALIFYING)
		cout << "STAGE:QUALIFYING" << endl;
	else if (stage == BaseDriver::RACE)
		cout << "STAGE: RACE" << endl;
	else
		cout << "STAGE: UNKNOWN" << endl;

	cout << "***********************************" << endl;
    // Create a socket (UDP on IPv4 protocol)
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (INVALID(socketDescriptor))
    {
        cerr << "cannot create socket\n";
        exit(1);
    }

    // Set some fields in the serverAddress structure.
    serverAddress.sin_family = hostInfo->h_addrtype;
    memcpy((char *) &serverAddress.sin_addr.s_addr,
           hostInfo->h_addr_list[0], hostInfo->h_length);
    serverAddress.sin_port = htons(serverPort);

    tDriver d(cntrl, logfile, path, manual, logging);
    strcpy(d.trackName,trackName);
    d.stage = stage;

    bool shutdownClient=false;
    unsigned long curEpisode=0;
    do
    {
        /***********************************************************************************
        ************************* UDP client identification ********************************
        ***********************************************************************************/
        do
        {
        	// Initialize the angles of rangefinders
        	float angles[19];
        	d.init(angles);
        	string initString = SimpleParser::stringify(string("init"),angles,19);
            cout << "Sending id to server: " << id << endl;
            initString.insert(0,id);
            cout << "Sending init string to the server: " << initString << endl;
            if (sendto(socketDescriptor, initString.c_str(), initString.length(), 0,
                       (struct sockaddr *) &serverAddress,
                       sizeof(serverAddress)) < 0)
            {
                cerr << "cannot send data ";
                CLOSE(socketDescriptor);
                exit(1);
            }

            // wait until answer comes back, for up to UDP_CLIENT_TIMEUOT micro sec
            FD_ZERO(&readSet);
            FD_SET(socketDescriptor, &readSet);
            timeVal.tv_sec = 0;
            timeVal.tv_usec = UDP_CLIENT_TIMEUOT;

            if (select(socketDescriptor+1, &readSet, NULL, NULL, &timeVal))
            {
                // Read data sent by the solorace server
                memset(buf, 0x0, UDP_MSGLEN);  // Zero out the buffer.
                numRead = recv(socketDescriptor, buf, UDP_MSGLEN, 0);
                if (numRead < 0)
                {
                    cerr << "didn't get response from server...";
                }
    		    else
    		    {
                	cout << "Received: " << buf << endl;

                	if (strcmp(buf,"***identified***")==0)
                    		break;
            	}
	      }

        }  while(1);

	unsigned long currentStep=0;

        do
        {
            // wait until answer comes back, for up to UDP_CLIENT_TIMEUOT micro sec
            FD_ZERO(&readSet);
            FD_SET(socketDescriptor, &readSet);
            timeVal.tv_sec = 0;
            timeVal.tv_usec = UDP_CLIENT_TIMEUOT;

            if (select(socketDescriptor+1, &readSet, NULL, NULL, &timeVal))
            {
                // Read data sent by the solorace server
                memset(buf, 0x0, UDP_MSGLEN);  // Zero out the buffer.
                numRead = recv(socketDescriptor, buf, UDP_MSGLEN, 0);
                if (numRead < 0)
                {
                    cerr << "didn't get response from server?";
                    CLOSE(socketDescriptor);
                    exit(1);
                }

#ifdef __UDP_CLIENT_VERBOSE__
                cout << "Received: " << buf << endl;
#endif

                if (strcmp(buf,"***shutdown***")==0)
                {
                    d.onShutdown();
                    shutdownClient = true;
                    cout << "Client Shutdown" << endl;
                    break;
                }

                if (strcmp(buf,"***restart***")==0)
                {
                    d.onRestart();
                    cout << "Client Restart" << endl;
                    break;
                }
                /**************************************************
                 * Compute The Action to send to the solorace sever
                 **************************************************/

        		if ( (++currentStep) != maxSteps)
        		{
                   	string action = d.drive(string(buf));
                   	memset(buf, 0x0, UDP_MSGLEN);
        			sprintf(buf,"%s",action.c_str());
        		}
        		else
        			sprintf (buf, "(meta 1)");

                if (sendto(socketDescriptor, buf, strlen(buf)+1, 0,
                           (struct sockaddr *) &serverAddress,
                           sizeof(serverAddress)) < 0)
                {
                    cerr << "cannot send data ";
                    CLOSE(socketDescriptor);
                    exit(1);
                }
#ifdef __UDP_CLIENT_VERBOSE__
                else
                    cout << "Sending " << buf << endl;
#endif
            }
            else
            {
                cout << "** Server did not respond in 1 second.\n";
            }
        } while(1);
    } while(shutdownClient==false && ( (++curEpisode) != maxEpisodes) );

    if(shutdownClient == false)
        d.onShutdown();
    CLOSE(socketDescriptor);
#ifdef WIN32
    WSACleanup();
#endif
    return 0;

}

//void parse_args(int argc, char *argv[], char *hostName, unsigned int &serverPort, char *id, unsigned int &maxEpisodes,
//		  unsigned int &maxSteps,bool &noise, double &noiseAVG, double &noiseSTD, long &seed, char *trackName, BaseDriver::tstage &stage)
void parse_args(int argc, char *argv[], char *hostName, unsigned int &serverPort, char *id, unsigned int &maxEpisodes,
		  unsigned int &maxSteps, char *trackName, BaseDriver::tstage &stage)
{
    int		i;

    // Set default values
    maxEpisodes=0;
    maxSteps=0;
    serverPort=3001;
    strcpy(hostName,"localhost");
    strcpy(id,"championship2011");
//    noise=false;
//    noiseAVG=0;
//    noiseSTD=0.05;
//    seed=0;
    strcpy(trackName,"unknown");
    stage=BaseDriver::UNKNOWN;


    i = 1;
    while (i < argc)
    {
    	if (strncmp(argv[i], "host:", 5) == 0)
    	{
    		sprintf(hostName, "%s", argv[i]+5);
    		i++;
    	}
    	else if (strncmp(argv[i], "port:", 5) == 0)
    	{
    		sscanf(argv[i],"port:%d",&serverPort);
    		i++;
    	}
    	else if (strncmp(argv[i], "id:", 3) == 0)
    	{
    		sprintf(id, "%s", argv[i]+3);
    		i++;
	    }
    	else if (strncmp(argv[i], "maxEpisodes:", 12) == 0)
    	{
    		sscanf(argv[i],"maxEpisodes:%ud",&maxEpisodes);
    	    i++;
    	}
    	else if (strncmp(argv[i], "maxSteps:", 9) == 0)
    	{
    		sscanf(argv[i],"maxSteps:%ud",&maxSteps);
    		i++;
    	}
//    	else if (strncmp(argv[i], "seed:", 5) == 0)
//    	{
//    	    	sscanf(argv[i],"seed:%ld",&seed);
//    	    	i++;
//    	}
    	else if (strncmp(argv[i], "track:", 6) == 0)
    	{
    	    	sscanf(argv[i],"track:%s",trackName);
    	    	i++;
    	}
    	else if (strncmp(argv[i], "stage:", 6) == 0)
    	{
				int temp;
    		   	sscanf(argv[i],"stage:%d",&temp);
    		   	stage = (BaseDriver::tstage) temp;
    	    	i++;
    	    	if (stage<BaseDriver::WARMUP || stage > BaseDriver::RACE)
					stage = BaseDriver::UNKNOWN;
    	}
    	else {
    		i++;		/* ignore bad args */
    	}
    }
}
