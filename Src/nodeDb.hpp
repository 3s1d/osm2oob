/*
 * nodeDb.hpp
 *
 *  Created on: 23 Dec 2018
 *      Author: sid
 */

#ifndef SRC_NODEDB_HPP_
#define SRC_NODEDB_HPP_

#include <map>

#include "osmparser.hpp"

typedef struct
{
	float lat;
	float lon;
	float alt;
} coord3d;

class NodeDB
{
private:
	std::map<int64_t, coord3d> db;

	coord3d coord;
	int64_t id = 0;

public:
	NodeDB() { }

	coord3d operator[](const int64_t id) { return db[id]; }

	void startElement(const std::string& name, const XmlAttr& attr);
	void endElement(const std::string& name);
};




#endif /* SRC_NODEDB_HPP_ */
