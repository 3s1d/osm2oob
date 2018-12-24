/*
 * nodeDb.cpp
 *
 *  Created on: 23 Dec 2018
 *      Author: sid
 */

#include <iostream>

#include "oob.hpp"
#include "nodeDb.hpp"

void NodeDB::startElement(const std::string& name, const XmlAttr& attr)
{
	if(name == "node")
	{
		/* prepare node for adding */
		coord.lat = deg2rad(attr.asDouble("lat"));
		coord.lon = deg2rad(attr.asDouble("lon"));
		coord.alt = -10000.0f;		//not an attributes
		id = attr.asInt64("id");
	}
	else if(name == "tag" && attr["k"] == "ele")
	{
		/* add altitude */
		coord.alt = attr.asDouble("v");
	}
}

void NodeDB::endElement(const std::string& name)
{
	/* put node into map */
	if(name == "node")
		db[id] = coord;
}

