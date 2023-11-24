/*
 * osm2oob.cpp
 *
 *  Created on: 23 Dec 2018
 *      Author: sid
 */

#include <iostream>
#include <boost/filesystem.hpp>

#include "bazlCsv.hpp"
#include "nodeDb.hpp"
#include "oob.hpp"
#include "osmparser.hpp"
#include "xconGjson.hpp"

#include "elevation.h" //tbr

#define DO_BAZLFLYLAND

using namespace std;

//Land Vorarlberg – data.vorarlberg.gv.at
//Land Tirol - data.tirol.gv.at -> https://gis.tirol.gv.at/ogd/verkehr_technik/FLUGHINDERNISSE_tirol.zip
//http://gis2.provinz.bz.it/geobrowser/?project=geobrowser_pro&view=Luftfahrthindernisse&lang=de

struct path_leaf_string
{
	std::string operator()(const boost::filesystem::directory_entry &entry) const
	{
		return entry.path().leaf().string();
	}
};

void read_directory(const std::string &name, vector<string> &v)
{
	boost::filesystem::path p(name);
	boost::filesystem::directory_iterator start(p);
	boost::filesystem::directory_iterator end;
	std::transform(start, end, std::back_inserter(v), path_leaf_string());
}

int main(int argc, char *argv[])
{
	cerr << "Open Street Map Ways / GeoJson to Open Obstacle Binary (OOB) converter (Version 2.0)" << endl; // prints API to AIR converter

	if (argc < 3 || argc > 5 || argc == 4)
	{
		cerr << "usage: " << argv[0] << " osmFolder oobFolder [username] [password]" << endl;
		return 1;
	}

	if (argc > 3)
	{
		elevation.user = argv[3];
		elevation.pw = argv[4];
	}

	/* get files */
	// note: no subfolders supported
	vector<string> files;
	read_directory(argv[1], files);

	/*
	 * V2
	 */
	/* iterate over all files */
	for (auto file : files)
	{
		/* input file */
		string fnameIn = string(argv[1]);
		fnameIn += "/" + file;

		/* output file */
		string fnameOut = string(argv[2]);
		fnameOut += "/" + file;

		size_t dotPosition = fnameIn.find_last_of('.');

		if (dotPosition != std::string::npos)
		{
			std::string baseName = fnameOut.substr(0, dotPosition);
			fnameOut = baseName + ".oob";
		}
		else
		{
			fnameOut += ".oob";
		}

		OobWritter oob = OobWritter(fnameOut);

		/* convert */
#ifdef DO_BAZLFLYLAND
		/* text from flyland */
		if (file.find(".txt") != std::string::npos)
		{
			cout << "Converting BAZL/flyland data: " << fnameIn << endl;
			BazlCsv bazl = BazlCsv();
			bazl.add(fnameIn);
			oob.add(bazl.obstacles);
		} else
#endif

		if (file.find(".geojson") != std::string::npos)
		{
			/* it's a geojson */
			cout << "Converting geojson(xcontest) data: " << fnameIn << " to " << fnameOut << endl;
			XconGjson xgjson = XconGjson();
			xgjson.load(fnameIn);
			oob.add(xgjson.obstacles);
		}
		else
		{
			/* it's a osm file */
			//todo improve check
			if (file.find(".osm") == std::string::npos && file.find(".osm.bz") == std::string::npos && file.find(".osm.gz") == std::string::npos)
				continue;
			std::cout << "Converting OSM file: " << fnameIn << " to " << fnameOut << std::endl;

			//cout << "Stage 1: node acquiring" << endl;
			NodeDB nodes = NodeDB();
			OsmParser<NodeDB> nodeParser(nodes);
			nodeParser.parse(fnameIn);

			//cout << "Stage 2: build ways" << endl;
			oob.setNodeDb(&nodes);
			OsmParser<OobWritter> wayParser(oob);
			wayParser.parse(fnameIn);
		}

		oob.finish();
	}

	/*
	 * V1 + KML
	 */

	cout << "Legacy stuff" << endl;

	OobWritter oob = OobWritter("world.oob");

	/* iterate over all files */
	for (auto file : files)
	{
		/* input file */
		string fnameIn = string(argv[1]);
		fnameIn += "/" + file;

		/* convert */
#ifdef DO_BAZLFLYLAND
		/* text from flyland */
		if (file.find(".txt") != std::string::npos)
		{
			cout << "Converting BAZL/flyland data: " << fnameIn << endl;
			BazlCsv bazl = BazlCsv();
			bazl.add(fnameIn);
			oob.add(bazl.obstacles);
		} else
#endif

		if (file.find(".geojson") != std::string::npos)
		{
			/* it's a geojson */
			cout << "Converting geojson(xcontest) data: " << fnameIn << endl;
			XconGjson xgjson = XconGjson();
			xgjson.load(fnameIn);
			oob.add(xgjson.obstacles);
		}
		else
		{
			/* it's a osm file */
			//todo improve check
			if (file.find(".osm") == std::string::npos && file.find(".osm.bz") == std::string::npos && file.find(".osm.gz") == std::string::npos)
				continue;
			std::cout << "Converting OSM file: " << fnameIn << std::endl;

			//cout << "Stage 1: node acquiring" << endl;
			NodeDB nodes = NodeDB();
			OsmParser<NodeDB> nodeParser(nodes);
			nodeParser.parse(fnameIn);

			//cout << "Stage 2: build ways" << endl;
			oob.setNodeDb(&nodes);
			OsmParser<OobWritter> wayParser(oob);
			wayParser.parse(fnameIn);
		}
	}

	/* write files */
	oob.finish(true);
	oob.toKml();
}
