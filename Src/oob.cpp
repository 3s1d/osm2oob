/*
 * oob.cpp
 *
 *  Created on: 23 Dec 2018
 *      Author: sid
 */

#include <iostream>

#include "oob.hpp"

void OobWritter::startElement(const std::string& name, const XmlAttr& attr)
{
	if(name == "way")
	{
		track.clear();		//clean, just in case
	}
	else if(name == "nd")
	{
		track.push_back((*db)[attr.asInt64("ref")]);
	}
}

void OobWritter::endElement(const std::string& name)
{
	if(name == "way")
	{
		/* add obstacle into DB */
		Obstacle obst = Obstacle();
		obst.track = track;
		obstacles.push_back(obst);
		track.clear();
	}
}


void OobWritter::finish(void)
{
	std::ofstream myFile (fname, std::ios::out | std::ios::binary);

	/* header */
	myFile.write(id, strlen(id));
	time_t t = time(nullptr);
	myFile.write((char *) &t, sizeof(time_t));

	/* obstacles */
	for(auto obs : obstacles)
		obs.write(myFile);


	std::cout << "OSM written " << obstacles.size() << " shapes" << std::endl;

	/* write control kml */
	toKml();

	/* cleanup */
	myFile.close();
	obstacles.clear();
}


void OobWritter::toKml(void)
{
	std::cout << "Building test KML" << std::endl;

	std::ofstream kml(fname + ".kml", std::ios::out);

	kml << "<kml xmlns=\"http://earth.google.com/kml/2.0\">" << std::endl;
	kml << "<Document>" << std::endl;
	kml << "<name>osm2oob</name>" << std::endl;
	kml << "<description><![CDATA[by SKYTRAXX GmbH]]></description>" << std::endl;
	kml << "<open>0</open>" << std::endl;
	kml << "<Style id=\"TrackLine\"><LineStyle><color>ff00ff00</color><width>2</width></LineStyle></Style>" << std::endl;

	/* edges */
	int i=0;
	for(auto obs : obstacles)
	{

		oob_vertex_t last;
		last.lat_rad = obs.track.front().lat;
		last.lon_rad = obs.track.front().lon;
		last.altitude = obs.track.front().alt;

		for(std::vector<Coord3D>::iterator it = std::next(obs.track.begin()); it != obs.track.end(); ++it)
		{
			kml << "<Placemark>" << std::endl;
			kml << "<name>Track " << i++ << "</name>" << std::endl;
			kml << "<styleUrl>#TrackLine</styleUrl>" << std::endl;
			kml << "<visibility>1</visibility>" << std::endl;

			kml << "<LineString>" << std::endl;
			kml << "<altitudeMode>absolute</altitudeMode>" << std::endl;
			kml << "<coordinates>" << std::endl;
			kml << rad2deg(last.lon_rad) << "," << rad2deg(last.lat_rad) << "," << last.altitude << std::endl;

			/* update */
			last.lat_rad = it->lat;
			last.lon_rad = it->lon;
			last.altitude = it->alt;
			kml << rad2deg(last.lon_rad) << "," << rad2deg(last.lat_rad) << "," << last.altitude << std::endl;

			kml << "</coordinates>" << std::endl;
			kml << "</LineString>" << std::endl;

			kml << "</Placemark>" << std::endl;
		}
	}

	kml << "</Document>" << std::endl;
	kml << "</kml>" << std::endl;
	kml.close();
}

void OobWritter::addBazl(BazlCsv *bazl)
{
	/* adding shapes */
	obstacles.insert(obstacles.end(), bazl->obstacles.begin(), bazl->obstacles.end());
}
