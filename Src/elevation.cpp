/*
 * groundlevel.cpp
 *
 *  Created on: Jul 23, 2018
 *      Author: sid
 */

#include <iostream>
#include <cmath>

#include "oob.hpp"
#include "coord3d.hpp"
#include "elevation.h"

bool Elevation::readHgt(const Coord3D &posAnchor)
{
	/* determine filename */
	anchor = posAnchor;
	char lat_c = anchor.lat>=0.0f ? 'N' : 'S';
	char lon_c = anchor.lon>=0.0f ? 'E' : 'W';
	char path[128];
	snprintf(path, sizeof(path), "cache/%c%02.f%c%03.f.hgt", lat_c, std::abs(anchor.lat), lon_c, std::abs(anchor.lon));

	/* open */
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if(file.is_open() == false)
	{
		std::cout << path << "  --  file not found, yet!" << std::endl;

		/* looking for zip file */
		char sysCmd[512];
		char zipFile[128];
		snprintf(zipFile, sizeof(zipFile), "cache/%c%02.f%c%03.f.SRTMGL1.hgt.zip", lat_c, std::abs(anchor.lat), lon_c, std::abs(anchor.lon));
		std::ifstream zipf(zipFile);
		if(zipf.good() == false)
		{
			std::cout << zipFile << "  --  file not found either -> downloading!" << std::endl;

			/* get authentication */
			if(user.length() == 0 || pw.length() == 0)
			{
				std::cout << "Username: ";
				std::cin >> user;
				std::cout << "Password: ";
				std::cin >> pw;
			}

			/* download */
			snprintf(sysCmd, sizeof(sysCmd),
	"wget --user '%s' --password '%s' http://e4ftl01.cr.usgs.gov/MEASURES/SRTMGL1.003/2000.02.11/%c%02.f%c%03.f.SRTMGL1.hgt.zip -P cache/",
					user.c_str(), pw.c_str(), lat_c, std::abs(anchor.lat), lon_c, std::abs(anchor.lon));
			std::cout << "download: " << sysCmd << std::endl;
			system(sysCmd);

		}
		else
		{
			zipf.close();
		}

		/* unzip */
		snprintf(sysCmd, sizeof(sysCmd), "unzip %s -d cache", zipFile);
		system(sysCmd);

		/* reopen */
		file.open(path, std::ios::in | std::ios::binary);
		if(file.good() == false)
		{
			anchor.lat = 0.0f;
			anchor.lon = 0.0f;
			return false;
		}
	}

	/* decompressor/read */
	file.read((char *)buffer, sizeof(buffer));
	int read = file.gcount();
	file.close();

	/* determine type/width */
	//note: 1201x1201, or 3601x1801, or 3601x3601 tiles
	switch(read)
	{
	case HGT_DATA_WIDTH_3 * HGT_DATA_WIDTH_3 * 2:
		numPointsX = HGT_DATA_WIDTH_3;
		numPointsY = HGT_DATA_WIDTH_3;
		break;
	case HGT_DATA_WIDTH_1 * HGT_DATA_WIDTH_1 * 2:
		numPointsX = HGT_DATA_WIDTH_1;
		numPointsY = HGT_DATA_WIDTH_1;
		break;
	case HGT_DATA_WIDTH_1 * HGT_DATA_WIDTH_1_HALF * 2:
		numPointsX = HGT_DATA_WIDTH_1_HALF;
		numPointsY = HGT_DATA_WIDTH_1;
		break;
	default:
		/* unsupported */
		std::cout << "HGT not supported. Unknown size: " << read << std::endl;
		return false;
	}

	return true;
}

bool Elevation::at(Coord3D &posRad)
{
	const Coord3D posDeg = Coord3D(rad2deg(posRad.lat), rad2deg(posRad.lon));

	/* open file if required */
	Coord3D ref = Coord3D(std::floor(posDeg.lat), std::floor(posDeg.lon));
	if(ref != anchor && readHgt(ref) == false)
		return false;

	/* build delta */
	Coord3D delta = posDeg - anchor;
	if(delta.lat>1.0f || delta.lat<0.0f || delta.lon>1.0f || delta.lon<0.0f)
		return false;

	/* position in file */
	uint32_t x = (uint32_t) (delta.lon*((float)(numPointsX-1)) +.5f);
	uint32_t y = (uint32_t) (numPointsY - delta.lat*((float)(numPointsY-1)) -.5f);	//+1.0f-0.5f
	int pos = (x + numPointsX*y) * 2;

	/* elevation */
	//note: switch big endian to little endian
	int16_t altitude = (int16_t) (((uint16_t)buffer[pos]<<8) | (uint16_t)buffer[pos+1]);
	posRad.alt = altitude;

	return true;
}


Elevation elevation;
