/*
 * coord3d.hpp
 *
 *  Created on: 24 Dec 2018
 *      Author: sid
 */

#ifndef SRC_COORD3D_HPP_
#define SRC_COORD3D_HPP_

#define D_FAK			6371000.0f			// FAI Sphere, mean erath radis in meter

#include <math.h>

class Coord3D
{
public:
	float lat;
	float lon;
	float alt;

	double distance(const Coord3D &to)
	{
		return (2.0*asin(sqrt( pow((sin((lat-to.lat)/2.0)),2.0) + cos(lat)*cos(to.lat)*pow((sin((lon-to.lon)/2.0f)),2.0) )) * D_FAK);
	}
};


#endif /* SRC_COORD3D_HPP_ */
