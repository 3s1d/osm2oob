/*
 * osm2oob.cpp
 *
 *  Created on: 23 Dec 2018
 *      Author: sid
 */

#include <iostream>
#include <boost/filesystem.hpp>

#include "nodeDb.hpp"
#include "oob.hpp"
#include "osmparser.hpp"

using namespace std;

struct path_leaf_string
{
    std::string operator()(const boost::filesystem::directory_entry& entry) const
    {
        return entry.path().leaf().string();
    }
};

void read_directory(const std::string& name, vector<string>& v)
{
	boost::filesystem::path p(name);
	boost::filesystem::directory_iterator start(p);
	boost::filesystem::directory_iterator end;
	std::transform(start, end, std::back_inserter(v), path_leaf_string());
}


int main(int argc, char* argv[])
{
	cerr << "Open Street Map Ways to Open Obstacle Binary (OOB) converter" << endl; // prints API to AIR converter

	if (argc != 2)
	{
		cerr << "usage: " << argv[0] << " osmFolder " << endl;
		return 1;
	}

	/* get files */
	// note: no subfolders supported
	vector<string> files;
	read_directory(argv[1], files);

	OobWritter oob = OobWritter("world.oob");

	/* iterate over all files */
	for(auto file : files)
	{
		/* is osm file */
		//todo improve check
		if(file.find(".osm") == std::string::npos && file.find(".osm.bz") == std::string::npos && file.find(".osm.gz") == std::string::npos)
			continue;
		string fname = string(argv[1]);
		fname += "/" + file;
		std::cout << "Converting file: " << fname << std::endl;

		cout << "Stage 1: node acquiring" << endl;
		NodeDB nodes = NodeDB();
		OsmParser<NodeDB> nodeParser(nodes);
		nodeParser.parse(fname);

		cout << "Stage 2: build ways" << endl;
		oob.setNodeDb(&nodes);
		OsmParser<OobWritter> wayParser(oob);
		wayParser.parse(fname);

	}

	oob.finish();
}


