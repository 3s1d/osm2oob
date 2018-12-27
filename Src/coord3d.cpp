/*
 * coord3d.cpp
 *
 *  Created on: 27 Dec 2018
 *      Author: sid
 */

#include <math.h>
#include "coord3d.hpp"

double Coord3D::distance(const Coord3D &to)
{
	return (2.0*asin(sqrt( pow((sin((lat-to.lat)/2.0)),2.0) + cos(lat)*cos(to.lat)*pow((sin((lon-to.lon)/2.0f)),2.0) )) * D_FAK);
}


Coord3D operator-(const Coord3D& lhs, const Coord3D& rhs)
{
	return Coord3D(lhs.lat - rhs.lat, lhs.lon - rhs.lon);
}

bool operator!=(const Coord3D& lhs, const Coord3D& rhs)
{
	return (lhs.lat != rhs.lat) || (lhs.lon != rhs.lon);
}


