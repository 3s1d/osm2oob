/*
 * bazlCsv.cpp
 *
 *  Created on: 24 Dec 2018
 *      Author: sid
 */

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "oob.hpp"
#include "bazlCsv.hpp"

void BazlCsv::add(std::string fname)
{
	std::ifstream file(fname);
	std::string line = "";
	while(getline(file, line))
	{
		std::vector<std::string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));

		/* for now, we ignore telephone, power lines, and suspension-bridges */
		//if(vec[1].find("POWER") != std::string::npos || vec[1].find("TELEPHONE") != std::string::npos ||
		//		vec[1].find("BRIDGE") != std::string::npos)
		//	continue;

		/* add obstacle */
		Obstacle obst;
		for(std::vector<std::string>::iterator it = std::next(vec.begin(), 2); it != vec.end(); ++it)
		{
			Coord3D coord;
			coord.lon = deg2rad(std::atof(it->c_str()));
			it = std::next(it);
			coord.lat = deg2rad(std::atof(it->c_str()));
			it = std::next(it);
			coord.alt = std::atof(it->c_str());
			obst.track.push_back(coord);
		}
		obstacles.push_back(obst);
	}

	// Close the File
	file.close();
}


bool BazlCsv::isSimilar(const Coord3D &start, const Coord3D &end)
{
	for(auto obst : obstacles)
	{
		//note: distance Trockener Steg - Klein Matterhorn cable distance 84m!
		if((obst.track.front().distance(start) < 60.0f || obst.track.front().distance(end) < 60.0f) &&
				(obst.track.back().distance(start) < 60.0f || obst.track.back().distance(end) < 60.0f))
		{
			//std::cout << std::min(obst.track.front().distance(start), obst.track.front().distance(end)) << " -- " <<
			//	 std::min(obst.track.back().distance(start), obst.track.back().distance(end)) << ": ";
			//std::cout << rad2deg(obst.track.front().lat) << "," << rad2deg(obst.track.front().lon) << " - "
			//		<< rad2deg(obst.track.back().lat) << "," << rad2deg(obst.track.back().lon) <<
			//		"  %%%  " << rad2deg(start.lat) << "," << rad2deg(start.lon) << " - "
			//		<< rad2deg(end.lat) << "," << rad2deg(end.lon) << std::endl;

			return true;
		}
	}

	return false;
}
