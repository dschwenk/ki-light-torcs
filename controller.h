/*	controller class template for AI Light Torcs project
	author: Robin Lehmann
	date: 12.04.2012
*/
#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "CarState.h"
#include "CarControl.h"

using namespace std;
#include <vector>
#include "LineVector.h"
#include <list>
#include "NearestVector.h"
	

class Controller
{
public:
	Controller();
	~Controller();


	// Logginzwecke	
	ostringstream SensorDataStream;
	string SensorDataString;

	// Vektor, der alle Objekte (Zeilen/Vektoren) der Trainingsdatei beinhaltet
	vector<LineVector*> LogFileLineVektorList;

	// TODO: evtl. private - kostet evtl. Perfomance da zusaetzliche Get/Set-Funktionsaufrufe?
	// needed for NearestNeighbour
	float currentLowest_accel;
	float currentLowest_brake;
	float currentLowest_steer;
	int currentLowest_gear;

	// needed for KNearestNeighbour
	float KNearest_accel;
	float KNearest_brake;
	float KNearest_steer;
	int KNearest_gear;
	float KNearest_sum;

	// needed for "Notfallprogramm" - BringCarBackToRoad
	float notfallGetTrack0;
	float notfallGetTrack9;
	float notfallGetTrack18;

	// needed for "Notfallprogramm" - turnCarToRightDrivingDirection
	float distFromStartValue;
	bool isCarDrivingInWrongDirection;

	// needed for "Notfallprogramm" - Auto hat sich festgefahren
	float distRaced;
	int counterTimeDriveBackward;
	int counterTimeDriveForward;
	bool isCarStuck;
	int counterDriveBackward;
	int counterDriveForward;
	bool FlagDriveBackward;
	bool FlagDriveForward;


	// automated Control for the car
	virtual void generateVector(CarState* cs, CarControl* cc);

	// get controller name
	virtual string getControllerName() { return "standard controller"; };

private:
	// subfunction for the automated control
	float accelControl(CarState* cs, CarControl* cc);

	// subfunction for the automated control
	float brakeControl(CarState* cs, CarControl* cc);

	// subfunction for the automated control
	float steerControl(CarState* cs, CarControl* cc);

	// subfunction for the automated control
	float clutchControl(CarState* cs, CarControl* cc);

	// subfunction for the automated control
	int gearControl(CarState* cs, CarControl* cc);


	// Berechnung des naechsten Nachbarn
	void calcNearestNeighbour(CarState* cs, CarControl* cc);

	// Berechnung der naechsten K Nachbarn
	void calcKNearestNeighbour(CarState* cs, CarControl* cc);

	// Auto abbremsen (bei Notfall)
	void slowCarDown(CarState* cs, CarControl* cc);

	// Bringe das Auto zurueck auf die Strecke
	void bringCarBackToRoad(CarState* cs, CarControl* cc);

	// Auto muss umdrehen
	void turnCarToRightDrivingDirection(CarState* cs, CarControl* cc);

	// Auto hat sich festgefahren
	void driveBackward(CarState* cs, CarControl* cc);
	void driveForward(CarState* cs, CarControl* cc);
	void decideDriveForBackward(CarState* cs, CarControl* cc);


};

#endif
