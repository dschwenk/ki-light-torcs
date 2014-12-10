/*	controller class template for AI Light Torcs project
	author: Stefan Moebius
	date: 11.04.2012
*/
#include "controller.h"


#include <fstream>
#include <sstream>
#include <iostream>
#include <string>


using namespace std;



Controller::Controller()
{
	//TODO: evtl. KI Initialisieren
	//TODO: Trainingsdaten von Datei laden

	string trainingDataFile;
	bool inputTrainingFileValid = false;
	int inputTrainingFile = 0;
	while(!inputTrainingFileValid){
		do {
			// clear zum zuruecksetzen nach Fehler, sync um Puffer zu leeren
			cin.clear();
			cin.sync();
			cout << "\nWelche Trainingsdatendatei laden?\n";
			cout << "[0] Default - alle Daten (trainingData.log)\n";
			cout << "[1] Akkarampalle-mountain (AkkarampalleData.log)\n";
			cout << "[2] Limalonges-desert (LimalongesData.log)\n";
			cout << "[3] Ushite-mountain-snow (UshiteData.log)\n";
			cout << "[9] Dateinnamen eingeben\n";
		}
		while(!(cin >> inputTrainingFile));
		switch(inputTrainingFile){
			case 0:
				trainingDataFile = "trainingData.log";
				inputTrainingFileValid = true;
				break;
			case 1:
				trainingDataFile = "AkkarampalleData.log";
				inputTrainingFileValid = true;
				break;
			case 2:
				trainingDataFile = "LimalongesData.log";
				inputTrainingFileValid = true;
				break;
			case 3:
				trainingDataFile = "UshiteData.log";
				inputTrainingFileValid = true;
				break;
			case 9:
				// TODO keine Eingabe moeglich?
				cout << "Dateinname: ";
				cin.clear();
				cin.sync();
				getline(cin,trainingDataFile);
				inputTrainingFileValid = true;
				break;
			default:
				cout << "\nUngueltige Eingabe!\n";
				break;
		}
	}

	const char *cstrTrainingDataFile = trainingDataFile.c_str();

	// LogDatei / Trainingsdatei oeffnen und zeilenweise auslesen
	cout << "\nOeffne Trainingsdaten-Datei " << trainingDataFile << " und importiere Vektoren";

	ifstream trainingDataFileStream(cstrTrainingDataFile,ios::in);
	if(trainingDataFileStream.is_open()){
		string line;
		while(getline(trainingDataFileStream,line)){
			// string parsen und Werte in Vektor ablegen
			stringstream ss(line);
			LineVector* TmpLineVector;
			TmpLineVector = new LineVector();			
			float wert = 0;
			
			for(int i=0;i<ElementeJeVektor;i++){
				ss >> wert;
				TmpLineVector->lv.push_back(wert);
				if(ss.peek() == ','){
					ss.ignore();
				}
			}
			// Vektor ans Ende der Liste anhaengen
			this->LogFileLineVektorList.push_back(TmpLineVector);
		}
		cout << "\nTrainingsdaten importiert\n\n";
	}
	else {
		cout << "\nKonnte Traininsdaten-Datein nicht oeffnen!\n\n";
	}

	// initialisiere Notfallprogramm CarIsDrivingInWrongDirection
	this->distFromStartValue = 0;
	this->isCarDrivingInWrongDirection = false;
}


Controller::~Controller()
{
	//TODO: Trainingsdaten in Datei abspeichern

	/*
	// Sensorwerte + Fahrzeugaktion in Datei speichern
	// Winkel, Gang, Drehzahl, Tracksensoren (je 3 Werte zu einem Wert zusammengefasst), Gas, Bremse, Gang, Lenkung
	*/

	/*
	cout << "\Schreibe Trainingsdaten\n";
	for(int i=0;i<this->LogFileLineVektorList.size();i++){
		for(int j=0;j<ElementeJeVektor;j++){
			this->SensorDataStream << this->LogFileLineVektorList[i]->lv[j] << ",";
		}
		this->SensorDataStream << "\n";
	}

	this->SensorDataString = this->SensorDataStream.str();

	ofstream trainingDataLog("trainingDataDestOut.log", ios::out | ios::app);
	trainingDataLog << this->SensorDataString;
	trainingDataLog.close();

	*/

	
	// Logging
	//
	// convert stream to string, open stream to log, write data
	this->SensorDataString = this->SensorDataStream.str();

	ofstream trainingDataLog("myLog.log", ios::out | ios::app);
	trainingDataLog << this->SensorDataString;
	trainingDataLog.close();
	
}

float Controller::accelControl(CarState* cs, CarControl* cc)
{
	//TODO: hier code einfügen, der die Beschleunigung des Autos regelt
	//float accel = 1.0f; //Für einen ersten test hier einfach auf "Vollgas" gesetzt...
	//return accel;


	float accel = this->KNearest_accel;

	// Logging
	this->SensorDataStream << accel;
	cout << "\naccel: " << accel;

	// Gas geben
	return accel;

}

float Controller::brakeControl(CarState* cs, CarControl* cc)
{
	//TODO: hier code einfügen, der das Auto abbremst, z.B. in Kurven...
	// float brake = 0.0f;

	// Wert wurde von calcKNearestNeighbour berechnet/zurueckgegeben und in XY gespeichert
	float brake = this->KNearest_brake;

	// Logging
	this->SensorDataStream << "," << brake;
	cout << "  brake: " << brake;

	// bremsen
	return brake;
}

float Controller::steerControl(CarState* cs, CarControl* cc)
{
	//TODO: hier code für die Lenkung einfügen
	//Es muss eine Fleißkommazahl im Wertebereich von -1.0 bis 1.0 zurückgegeben werden.
	//-1.0: stark nach rechts lenken
	// 0.0: in keine Richtung lenken
	// 1.0: stark nach links lenken
	// float steer = 0.2f;

	// Wert wurde von calcKNearestNeighbour berechnet/zurueckgegeben und in XY gespeichert
	float steer = this->KNearest_steer;

	// Logging
	this->SensorDataStream << "," << steer;
	cout << "  steer: " << steer;

	// lenken
	return steer;
}


float Controller::clutchControl(CarState* cs, CarControl* cc)
{
	//TODO: Kupplung implementieren (nicht unbedingt nötig)
	float clutch = 0.0f;
	return clutch;
}

int Controller::gearControl(CarState* cs, CarControl* cc)
{
	//TODO: Gangschaltung implementieren
	// mögliche Werte sind -1 (Rückwärtsgang), 0 (Leerlauf), 1, 2, 3, 4, 5, 6
	// int gear = 1; //hier in diesem Beispiel einfach immer 1 Gang verwenden

	// Wert wurde von calcKNearestNeighbour berechnet/zurueckgegeben und in XY gespeichert
	int gear = this->KNearest_gear;

	// Logging
	this->SensorDataStream << "," << gear << "\n";
	cout << "  gear: " << gear << "\n" ;

	// schalten
	return gear;
}

void Controller::generateVector(CarState* cs, CarControl* cc)
{
	//TODO: KI Trainieren
	//TODO: Ausgabe durch KI berechnen

	// Aufruf der Funktionen in der Reihenfolge
	// generateVector,accelcontrol,brakeControl,steerControl,clutchControl,gearControl

	bool carPassedStartFinish = false;
	// pruefe ob Auto eben ueber Start/Ziel gefahren ist
	if((cs->getDistFromStart() >= 0) && (cs->getDistFromStart() < 2)){
		cout << "carPassedStartFinish\n";
		this->distFromStartValue = 0;
		carPassedStartFinish = true;
	}


	// Notfallprogramm - Strecke verlassen, alle Trackwerte == -1
	if((cs->getTrack(0) == -1) && (cs->getTrack(9) == -1) && (cs->getTrack(18) == -1)){
		// Auto ist neben der Strecke
		bringCarBackToRoad(cs,cc);
	}
	// Notfallprogramm 2 - Auto faehrt in die Falsche Richtung
	else if(cs->getDistFromStart() < this->distFromStartValue){
		turnCarToRightDrivingDirection(cs,cc);
	}
	// KEIN_ Notfall - Auto faehrt "normal"
	else {

		// fuer den Notfall speichern ob sich das das Auto links oder rechts auf die Streckenbegrenzung zubewegt
		this->notfallGetTrack0 = cs->getTrack(0);
		this->notfallGetTrack18 = cs->getTrack(18);

		/*
		 * OLD
		 * Nearest-Neighbour zur Berechnung des naechsten Nachbarn
		 * calcNearestNeighbour(cs, cc);
		 */

		// K-Nearest-Neighbour zur Berechnung der naechsten Nachbarn
		calcKNearestNeighbour(cs, cc);
	}

	// setze Wert nur wenn er eben nicht zurueck gesetzt wurde und das Auto in die richtige Richtung faehrt
	if((carPassedStartFinish == false) && (isCarDrivingInWrongDirection == false)){
		this->distFromStartValue = cs->getDistFromStart();
	}

	cc->setAccel(accelControl(cs, cc));
	cc->setBrake(brakeControl(cs, cc));
	cc->setSteer(steerControl(cs, cc));
	cc->setClutch(clutchControl(cs, cc));
	cc->setGear(gearControl(cs, cc));
}


void Controller::turnCarToRightDrivingDirection(CarState* cs, CarControl* cc){
	// Kontrollausgabe der aktuellen Trackwerte + Geschwindigkeit
	cout << "\turnCarToRightDrivingDirection\n";
	// cout << cs->getTrack(0) << "\t" << cs->getTrack(9) << "\t" << cs->getTrack(18) << "\n";
	// cout << cs->getSpeedX() << "\t" << cs->getSpeedY() << "\n";

	isCarDrivingInWrongDirection = true;

	// das Auto ist langsam
	if(cs->getSpeedX() <= 20){
		// gebe leicht Gas
		this->KNearest_accel = 0.6;
		this->KNearest_brake = 0;
		this->KNearest_gear = 1;
		// pruefe ob Auto weiter links oder rechts auf der Strecke steht
		// Auto steht weiter rechts auf der Strecke
		if(this->notfallGetTrack0 > this->notfallGetTrack18){
			// lenke nach links
			this->KNearest_steer = 1;
		}
		// Auto steht weiter links links auf der Strecke
		else {
			// lenke nach rechts
			this->KNearest_steer = -1;
		}
	}
	// das Auto ist mit hoher Geschwindigkeit neben der Strecke
	else {
		// bremsen um das Auto zum langsamer werden / anhalten zu bringen
		slowCarDown(cs,cc);
	}

}


void Controller::bringCarBackToRoad(CarState* cs, CarControl* cc){

	// Kontrollausgabe der aktuellen Trackwerte + Geschwindigkeit
	cout << "\nbringCarBackToRoad\n";
	// cout << cs->getTrack(0) << "\t" << cs->getTrack(9) << "\t" << cs->getTrack(18) << "\n";
	// cout << cs->getSpeedX() << "\t" << cs->getSpeedY() << "\n";

	// das Auto ist langsam bzw. steht neben der Strecke
	if(cs->getSpeedX() <= 20){
		// gebe leicht Gas
		this->KNearest_accel = 0.6;
		this->KNearest_brake = 0;
		this->KNearest_gear = 1;
		// pruefe ob Auto links oder rechts von der Strecke steht
		// Auto steht rechts neben der Strecke wenn der Streckenabstand links gross war
		if(this->notfallGetTrack0 > this->notfallGetTrack18){
			// lenke nach links
			this->KNearest_steer = 0.4;
		}
		// Auto steht links neben der Strecke
		else {
			// lenke nach rechts
			this->KNearest_steer = -0.4;
		}
	}
	// das Auto ist mit hoher Geschwindigkeit neben der Strecke
	else {
		// bremsen um das Auto zum langsamer werden / anhalten zu bringen
		slowCarDown(cs,cc);
	}
}


void Controller::slowCarDown(CarState* cs, CarControl* cc){
	// bremsen um das Auto zum langsamer werden / anhalten zu bringen
	this->KNearest_accel = 0;
	this->KNearest_brake = 0.45;
	this->KNearest_steer = 0;
	float speedX = cs->getSpeedX();
	if(speedX > 150){
		this->KNearest_gear = 5;
	}
	else if (speedX > 120){
		this->KNearest_gear = 4;
	}
	else if (speedX > 70){
		this->KNearest_gear = 3;
	}
	else if (speedX > 30){
		this->KNearest_gear = 2;
	}
	else {
		this->KNearest_gear = 1;
	}
}



void Controller::calcKNearestNeighbour(CarState* cs, CarControl* cc){

	// Berechnung der K naechsten Nachbarn

	// Linevektor lv Index 0-67
	// 0		angle
	// 1		distFromStart
	// 2		gear
	// 3		lastLapTime
	// 4-39		opponents
	// 40		rpm
	// 41		speedX
	// 42		speedY
	// 43		speedZ
	// 44-62	track
	// 63		trackPos
	// 64		gear
	// 65		accel
	// 66		brake
	// 67		steering

	// NearestVector nv Index 0-4
	// 0		NearestNeighbourCalcSum
	// 1		accel
	// 2		brake
	// 3		gear
	// 4		steer

	// Liste, die die K naechsten Vektoren enthaelt
	list<NearestVector*> NearestVectorList;

	// ersten Vektor mit Nullwerten initialisieren und der Liste hinzufuegen
	NearestVector* TmpNearestVector;
	TmpNearestVector = new NearestVector();
	float lowestsum = 10000000000000000000.0;
	(*TmpNearestVector).nv.push_back(lowestsum);
	for(int i=0;i<4;i++){
		(*TmpNearestVector).nv.push_back(0.0);
	}
	NearestVectorList.push_front(TmpNearestVector);


	// Iterator
	list<NearestVector*>::iterator it;


	float NearestNeighbourDistanceSum = 0.0;
	int getTrackIndex;

	float normedGear;
	float normedGetGear;
	float normedRPM;
	float normedGetRPM;
	float normedSpeedX;
	float normedGetSpeedX;
	float normedSpeedY;
	float normedGetSpeedY;

	// cout << "\ncalcKNearestNeighbour";

	// gehe alle Vektoren durch ...
	for(int i=0;i<this->LogFileLineVektorList.size();i++){
		// Tracksensor ID
		getTrackIndex = 0;
		NearestNeighbourDistanceSum = 0.0;

		// berechne Abstand


		// Angle / Winkel zur Streck
		NearestNeighbourDistanceSum += ((this->LogFileLineVektorList[i]->lv[0] - cs->getAngle()) * (this->LogFileLineVektorList[i]->lv[0] - cs->getAngle()));

		// Gang - normiert
		normedGear = this->LogFileLineVektorList[i]->lv[2];
		normedGetGear = cs->getGear();
		NearestNeighbourDistanceSum += ((normedGear - normedGetGear) * (normedGear - normedGetGear));


		// RPM - normiert
		normedRPM = this->LogFileLineVektorList[i]->lv[40] / 1000;
		normedGetRPM = cs->getRpm() / 1000;
		NearestNeighbourDistanceSum += ((normedRPM - normedGetRPM) * (normedRPM - normedGetRPM));


		// SppedX - normiert
		normedSpeedX = this->LogFileLineVektorList[i]->lv[41] / 35;
		normedGetSpeedX = cs->getSpeedX() / 35;
		NearestNeighbourDistanceSum += ((normedSpeedX - normedGetSpeedX) * (normedSpeedX - normedGetSpeedX));


		// SppedY - normiert
		normedSpeedY = this->LogFileLineVektorList[i]->lv[42] / 28;
		normedGetSpeedY = cs->getSpeedY() / 28;
		NearestNeighbourDistanceSum += ((normedSpeedY - normedGetSpeedY) * (normedSpeedY - normedGetSpeedY));

		// TrackSensorDaten
		// j+=2 -> "nur" 10 anstatt 19 werte beruecksichtigt
		// j+=3 -> "nur" 7 anstatt 19 werte beruecksichtigt

		for(int j=44;j<=62;j+=2){
			NearestNeighbourDistanceSum += ((this->LogFileLineVektorList[i]->lv[j] - cs->getTrack(getTrackIndex)) * (this->LogFileLineVektorList[i]->lv[j] - cs->getTrack(getTrackIndex)));
			getTrackIndex += 2;
		}
		/*
		NearestNeighbourDistanceSum += ((this->LogFileLineVektorList[i]->lv[44] - cs->getTrack(0)) * (this->LogFileLineVektorList[i]->lv[44] - cs->getTrack(0)));
		NearestNeighbourDistanceSum += ((this->LogFileLineVektorList[i]->lv[49] - cs->getTrack(5)) * (this->LogFileLineVektorList[i]->lv[49] - cs->getTrack(5)));
		NearestNeighbourDistanceSum += ((this->LogFileLineVektorList[i]->lv[53] - cs->getTrack(9)) * (this->LogFileLineVektorList[i]->lv[53] - cs->getTrack(9)));
		NearestNeighbourDistanceSum += ((this->LogFileLineVektorList[i]->lv[57] - cs->getTrack(13)) * (this->LogFileLineVektorList[i]->lv[57] - cs->getTrack(13)));
		NearestNeighbourDistanceSum += ((this->LogFileLineVektorList[i]->lv[62] - cs->getTrack(18)) * (this->LogFileLineVektorList[i]->lv[62] - cs->getTrack(18)));
		*/

		// gehe Liste durch und pruefe ob die Summe kleiner ist als bei einem der K naechsten Vektoren
		for(it=NearestVectorList.begin();it != NearestVectorList.end(); ++it){
			// cout << "\npruefe summe kleiner als bei einem k naechsten vektor";
			if(NearestNeighbourDistanceSum < (*it)->nv[0]){
				// Fuege sum, accel, brake, steer, gear Werte zum NearestVector hinzu
				NearestVector* TmpNearestVector;
				TmpNearestVector = new NearestVector();
				(*TmpNearestVector).nv.push_back(NearestNeighbourDistanceSum);
				// 64 gear, 65 accel, 66 brake, 67 steering
				(*TmpNearestVector).nv.push_back(this->LogFileLineVektorList[i]->lv[65]);
				(*TmpNearestVector).nv.push_back(this->LogFileLineVektorList[i]->lv[66]);
				(*TmpNearestVector).nv.push_back(this->LogFileLineVektorList[i]->lv[67]);
				(*TmpNearestVector).nv.push_back(this->LogFileLineVektorList[i]->lv[64]);
				NearestVectorList.insert(it,TmpNearestVector);

				// pruefe ob mehr als K Vektoren in der Liste sind
				if(NearestVectorList.size() > K){
					// entferne letztes Element / letzten Vektor aus der Liste
					NearestVectorList.pop_back();
					// cout << "\nmehr als k vektoren, letzen vektor loeschen";
				}
				// Vektor war kleiner, Liste muss nicht weiter durchgegangen werden
				break;
			}
		}
	}
	// alle Vektorenabstaende wurden berechnet
	// cout << "\nalle Vektorenabstaende wurden berechnet";

	// Testausgabe der niedrigsten summe (des am best passenden vektors)
	it=NearestVectorList.begin();
	cout << (*it)->nv[0] << "  |  ";

	float accelsum = 0, brakesum = 0, steersum = 0;
	int gearsum = 0;

	for(it=NearestVectorList.begin();it != NearestVectorList.end(); ++it){
		// cout << "\nberechne accelsum, breakesum etc";
		accelsum += (*it)->nv[1];
		brakesum += (*it)->nv[2];
		steersum += (*it)->nv[3];
		gearsum += (*it)->nv[4];
		// Kontrollausgabe der K naechsten Vektoren
		cout << "\nNV: sum" << (*it)->nv[0] << "\taccel" << (*it)->nv[1] << "\tbrake" << (*it)->nv[2] << "\tsteer" << (*it)->nv[3] << "\tgear" << (*it)->nv[4];
	}

	float accel = accelsum / K;
	float brake = brakesum / K;
	float steer = steersum / K;
	// int gear = (int)gearsum / (K-0.85);
	// K-0.85, berechnet den Gang akkurater -> float 1.9 => int 1

	// setting gear "manuel" is accurater than calculating / prevents gear from heavy overfitting
	float speedX = cs->getSpeedX();
	if(speedX > 240){
		this->KNearest_gear = 6;
	}
	else if(speedX > 200 && speedX < 230){
		this->KNearest_gear = 5;
	}
	else if (speedX > 140 && speedX < 190){
		this->KNearest_gear = 4;
	}
	else if (speedX > 90 && speedX < 130){
		this->KNearest_gear = 3;
	}
	else if (speedX > 40 && speedX < 80){
		this->KNearest_gear = 2;
	}
	else if (speedX < 35){
		this->KNearest_gear = 1;
	}
	else {
		// do nothing, keep current gear
	}


	this->KNearest_accel = accel;
	this->KNearest_brake = brake;
	this->KNearest_steer = steer;
	// this->KNearest_gear = gear;

}


void Controller::calcNearestNeighbour(CarState* cs, CarControl* cc){

	// Berechnung des naechsten Nachbarn

	// Linevektor index
	// 0		angle
	// 1		distFromStart
	// 2		gear
	// 3		lastLapTime
	// 4-39		opponents
	// 40		rpm
	// 41		speedX
	// 42		speedY
	// 43		speedZ
	// 44-62	track
	// 63		trackPos
	// 64		gear
	// 65		accel
	// 66		brake
	// 67		steering
	
	float lowestsum = 10000000000000000000.0;
	float sum = 0.0;
	int m;

	// cout << "calcNearestNeighbour";
	
	// gehe alle Vektoren durch ...
	for(int i=0;i<this->LogFileLineVektorList.size();i++){
		// Tracksensor ID
		m = 0;	
		sum = 0.0;
		// berechne Abstand
		for(int j=44;j<=62;j++){
			sum += ((this->LogFileLineVektorList[i]->lv[j] - cs->getTrack(m)) * (this->LogFileLineVektorList[i]->lv[j] - cs->getTrack(m)));
			m++;
		}

		// Naechster Vektor berechnet - Werte im Objekt speichern
		if(sum < lowestsum){
			this->currentLowest_accel = this->LogFileLineVektorList[i]->lv[65];
			this->currentLowest_brake = this->LogFileLineVektorList[i]->lv[66];
			this->currentLowest_gear = this->LogFileLineVektorList[i]->lv[64];
			this->currentLowest_steer = this->LogFileLineVektorList[i]->lv[67];
	
			lowestsum = sum;
		}
	}
	
}
