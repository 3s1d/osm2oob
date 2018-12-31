/*
 * coord3d.hpp
 *
 *  Created on: 24 Dec 2018
 *      Author: sid
 */

#ifndef SRC_COORD3D_HPP_
#define SRC_COORD3D_HPP_

#define D_FAK			6371000.0f			// FAI Sphere, mean earth radius in meter

#include <math.h>

class Coord3D
{
public:
	float lat;
	float lon;
	float alt;

	Coord3D(): lat(0), lon(0), alt(0) { }
	Coord3D(float lat, float lon, float alt = 0) : lat(lat), lon(lon), alt(alt) { }

	double distance(const Coord3D &to);
};

Coord3D operator-(const Coord3D& lhs, const Coord3D& rhs);
bool operator!=(const Coord3D& lhs, const Coord3D& rhs);

#endif /* SRC_COORD3D_HPP_ */
