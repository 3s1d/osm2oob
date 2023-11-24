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


void OobWritter::finish(bool vOne)
{
	std::ofstream myFile (fname, std::ios::out | std::ios::binary);

	/* header */
	myFile.write(vOne ? idOld : id, strlen(id));
	time_t t = time(nullptr);
	myFile.write((char *) &t, sizeof(time_t));

	if(vOne == false)
	{
		/* bounding box */
		float lat_top = -M_PI;
		float lat_bot = M_PI;
		float lon_left = M_PI*2.0f;
		float lon_right = -M_PI*2.0f;
		for(auto obs : obstacles)
			for(auto vertex : obs.track)
			{
				if(vertex.lat > lat_top)
					lat_top = vertex.lat;
				if(vertex.lat < lat_bot)
					lat_bot = vertex.lat;
				if(vertex.lon > lon_right)
					lon_right = vertex.lon;
				if(vertex.lon < lon_left)
					lon_left = vertex.lon;
			}
		myFile.write((char *) &lat_top, sizeof(float));
		myFile.write((char *) &lat_bot, sizeof(float));
		myFile.write((char *) &lon_left, sizeof(float));
		myFile.write((char *) &lon_right, sizeof(float));

		/* num obstacles */
		uint16_t numObst = obstacles.size();
		myFile.write((char *) &numObst, sizeof(uint16_t));
	}

	/* obstacles */
	for(auto obs : obstacles)
		obs.write(myFile, vOne);

	std::cout << "OSM written " << obstacles.size() << " shapes (" << (vOne ? "v1)" : "v2)") << std::endl;

	/* cleanup */
	myFile.close();
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

void OobWritter::addTester(void)
{
	Obstacle obst;
	obst.track.push_back(Coord3D(deg2rad(49.481), deg2rad(10.785), 370));
	obst.track.push_back(Coord3D(deg2rad(49.486), deg2rad(10.791), 360));
	obstacles.push_back(obst);
}

void OobWritter::add(std::vector<Obstacle> &obst)
{
	/* adding shapes */
	obstacles.insert(obstacles.end(), obst.begin(), obst.end());
}
