/*
 * groundlevel.h
 *
 *  Created on: Jul 23, 2018
 *      Author: sid
 */

#ifndef VARIO_FLIGHTCLT_ELEVATION_H_
#define VARIO_FLIGHTCLT_ELEVATION_H_

// Some HGT files contain 1201 x 1201 points (3 arc/90m resolution)
#define HGT_DATA_WIDTH_3	1201ul
// Some HGT files contain 3601 x 3601 points (1 arc/30m resolution)
#define HGT_DATA_WIDTH_1	3601ul
// Some HGT files contain 3601 x 1801 points (1 arc/30m resolution)
#define HGT_DATA_WIDTH_1_HALF	1801ul

/*
 * gdallocationinfo -wgs84 ~/Downloads/N49E010.hgt 10.929334 49.492432
 * Report:
 *  Location: (1115P,609L)
 *  Band 1:
 *   Value: 313
 *
 */

#include <iostream>
#include <fstream>

#include "coord3d.hpp"

class Elevation
{
private:
	uint16_t numPointsX;
	uint16_t numPointsY;
	Coord3D anchor;
	uint8_t buffer[HGT_DATA_WIDTH_1 * HGT_DATA_WIDTH_1 * 2 + 1];

	bool readHgt(const Coord3D &posAnchor);

	std::string user = "";
	std::string pw = "";

public:
	Elevation() : numPointsX(0), numPointsY(0) { }
	Elevation(const Elevation&) = delete;
//	~Elevation() { system("rm cache/*.hgt"); }

	bool at(Coord3D &posRad);
};

extern Elevation elevation;

#endif /* VARIO_FLIGHTCLT_ELEVATION_H_ */
