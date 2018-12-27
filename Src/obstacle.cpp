/*
 * obstacle.cpp
 *
 *  Created on: 27 Dec 2018
 *      Author: sid
 */

#include "oob.hpp"

void Obstacle::write(std::ofstream &file)
{
	oob_edge_t last;
	last.lat_rad = track.front().lat;
	last.lon_rad = track.front().lon;
	last.altitude = track.front().alt;

	for(std::vector<Coord3D>::iterator it = std::next(track.begin()); it != track.end(); ++it)
	{
		/* tuple */
		file.write((char *) &last, sizeof(oob_edge_t));

		/* update */
		last.lat_rad = it->lat;
		last.lon_rad = it->lon;
		last.altitude = it->alt;
		file.write((char *) &last, sizeof(oob_edge_t));
	}

}

