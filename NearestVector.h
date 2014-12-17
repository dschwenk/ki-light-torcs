#pragma once

#include <vector>
#include "controller.h"
using namespace std;


// Nearest Neighbour Count
#define K 6


// Anzahl an Elementen je Vektor
static const int ElementeJeNearestNeighbourVektor = 5;
// NearestVector nv Index 0-4
// 0		NearestNeighbourCalcSum
// 1		accel
// 2		brake
// 3		gear
// 4		steer


// Klasse, die einen Vektor mit allen Werte eines NearestVektors enthaelt + ?

class NearestVector {
	// public:
	//	NearestVector();

	public:
		std::vector<float> nv;
		/* http://de.wikibooks.org/wiki/C++-Programmierung:_Vector
		vector<T> v(n, val); Deklariert v als vector, der bereits n Elemente vom Typ T mit dem Wert val enthält.
		*/
};
