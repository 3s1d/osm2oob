/*
 * xconGjson.cpp
 *
 *  Created on: Nov 23, 2023
 *      Author: sid
 */

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "oob.hpp"
#include "xconGjson.hpp"

using json = nlohmann::json;

bool XconGjson::load(const std::string &fname)
{
    /* Open the GeoJSON file */
    std::ifstream geojsonFile(fname);

    /* Check if the file is opened successfully */
    if (!geojsonFile.is_open()) {
        std::cerr << "Error: Unable to open GeoJSON file." << std::endl;
        return false;
    }
    /* Parse the GeoJSON data */
    json geojsonData;
    geojsonFile >> geojsonData;

    /* Close the file */
    geojsonFile.close();

	/* Access GeoJSON features */
	if(geojsonData.contains("features") == false)
		return false;

	auto features = geojsonData["features"];
	for(const auto& feature : features)
	{
		/* Access geometry */
		if(feature.contains("geometry") == false)
			continue;
		auto geometry = feature["geometry"];

		/* Access coordinates */
		if(geometry.contains("coordinates") == false)
			continue;
		auto coordinates = geometry["coordinates"];

		/* add obstacle */
		Obstacle obst = Obstacle();
		for(const auto& coordinate : coordinates)
		{
			Coord3D coord;
			coord.lon = deg2rad((double)coordinate[0]);
			coord.lat = deg2rad((double)coordinate[1]);
			coord.alt = coordinate[2];

			obst.track.push_back(coord);
		}
		obstacles.push_back(obst);
	}

	// Clear the JSON object after each iteration
	geojsonData.clear();

	return true;
}
