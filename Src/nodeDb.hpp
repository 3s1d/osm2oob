/*
 * nodeDb.hpp
 *
 *  Created on: 23 Dec 2018
 *      Author: sid
 */

#ifndef SRC_NODEDB_HPP_
#define SRC_NODEDB_HPP_

#include <map>

#include "coord3d.hpp"
#include "osmparser.hpp"


class NodeDB
{
private:
	std::map<int64_t, Coord3D> db;

	Coord3D coord;
	int64_t id = 0;

public:
	NodeDB() { }

	Coord3D operator[](const int64_t id) { return db[id]; }

	void startElement(const std::string& name, const XmlAttr& attr);
	void endElement(const std::string& name);
};




#endif /* SRC_NODEDB_HPP_ */
