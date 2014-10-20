#pragma once

#include <vector>
#include "controller.h"
using namespace std;


// Klasse, die einen Vektor mit allen Werte einer Zeile enthaelt + ?


// Anzahl an Elementen je Vektor 
static const int ElementeJeVektor = 68;
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


class LineVector {
	// public:
	//	LineVector();

	public:
		std::vector<float> lv;
		/* http://de.wikibooks.org/wiki/C++-Programmierung:_Vector
		vector<T> v(n, val); Deklariert v als vector, der bereits n Elemente vom Typ T mit dem Wert val enthält.
		*/
};
