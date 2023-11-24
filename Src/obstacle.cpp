/*
 * obstacle.cpp
 *
 *  Created on: 27 Dec 2018
 *      Author: sid
 */

#include <iostream>

#include "oob.hpp"

void Obstacle::write(std::ofstream &file, bool vOne)
{
	/* old v1 style */
	if(vOne)
	{
		oob_vertex_t last;
		last.lat_rad = track.front().lat;
		last.lon_rad = track.front().lon;
		last.altitude = track.front().alt;

		for(std::vector<Coord3D>::iterator it = std::next(track.begin()); it != track.end(); ++it)
		{
			/* tuple */
			file.write((char *) &last, sizeof(oob_vertex_t));

			/* update */
			last.lat_rad = it->lat;
			last.lon_rad = it->lon;
			last.altitude = it->alt;
			file.write((char *) &last, sizeof(oob_vertex_t));
		}

		return;
	}

	if(track.size() > 256)
	{
		std::cerr << "Splitting Obstacle (" << track.size() << "vertex)" << std::endl;

		Obstacle obs;
		for(std::vector<Coord3D>::iterator it = std::next(track.begin()); it != track.end(); ++it)
		{
			obs.track.push_back(*it);

			/* emit obstacle */
			if(obs.track.size() >= 254)
			{
				obs.write(file);

				/* insert current element in new vector to close the gap */
				obs.track.clear();
				obs.track.push_back(*it);
			}
		}

		/* write tail */
		if(obs.track.size() > 1)
			obs.write(file);
		return;
	}

	if(track.size() == 0)
	{
		std::cerr << "Obstacle w/ no vertex!" << std::endl;
		return;
	}

	if(track.size() == 1)
	{
		std::cerr << "Obstacle w/ only one vertex! Discarding." << std::endl;
		return;
	}


	/* obstacle header */
	char magic = 'O';
	file.write((char *) &magic, sizeof(char));
	uint8_t numVertex = track.size() - 1;
	file.write((char *) &numVertex, sizeof(uint8_t));

	for(std::vector<Coord3D>::iterator it = std::next(track.begin()); it != track.end(); ++it)
	{
		oob_vertex_t vertex;
		vertex.lat_rad = it->lat;
		vertex.lon_rad = it->lon;
		vertex.altitude = it->alt;
		file.write((char *) &vertex, sizeof(oob_vertex_t));
	}
}

