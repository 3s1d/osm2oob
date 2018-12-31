/*
 * oob.hpp
 *
 *  Created on: 23 Dec 2018
 *      Author: sid
 */

#ifndef SRC_OOB_HPP_
#define SRC_OOB_HPP_

#include <vector>

#include "bazlCsv.hpp"
#include "coord3d.hpp"
#include "nodeDb.hpp"
#include "osmparser.hpp"

#define deg2rad(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define rad2deg(angleRadians) ((angleRadians) * 180.0 / M_PI)

/*
 * Format:
 * 3byte char      'OOB'
 * 1byte uint8_t   version number, currently 1
 * 8byte time_t	   built time
 *
 * Obstacle Edge:
 * 4byte float latitude rad	vertex0
 * 4byte float longitude rad	vertex0
 * 4byte MSL altitude		vertex0
 * 4byte float latitude rad	vertex1
 * 4byte float longitude rad	vertex1
 * 4byte MSL altitude		vertex1
 * --------------------------
 * 24bytes
 *
 *
 * note: multiple edges are considered as a polygon if vertex1 of the previous edge is equal to vertex0 of the current edge
 */

typedef struct
{
	float lat_rad;
	float lon_rad;
	float altitude;
} __attribute__((packed)) oob_vertex_t;

class OobWritter
{
private:
	std::string fname;

	/* Temp Obstalce */
	//todo type... altitude...
	std::vector<Coord3D> track;
	NodeDB *db = nullptr;

	/* all obstacles */
	std::vector<Obstacle> obstacles;

	void toKml(void);
public:
	const char *id = "OOB\x1";

	OobWritter(std::string fname) : fname(fname) { }
	void setNodeDb(NodeDB *db) { this->db = db; }
	void addBazl(BazlCsv *bazl);
	void addTester(void);
	void finish(void);


	void startElement(const std::string& name, const XmlAttr& attr);
	void endElement(const std::string& name);


};



#endif /* SRC_OOB_HPP_ */
