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
	/*
	//todo close all
	int over = 0;
	int min = 1000;
	int max = 0;
	int sum = 0;
	for(auto obs : obstacles)
	{
		if(obs.track.size() < min)
			min = obs.track.size();
		if(obs.track.size() > max)
			max = obs.track.size();
		sum += obs.track.size();

		if(obs.track.size() > 255)
			over++;
	}

	std::cout << "min" << min << std::endl;
	std::cout << "max" << max << std::endl;
	std::cout << "sum" << sum << std::endl;
	std::cout << "avg" << sum / obstacles.size() << std::endl;
	std::cout << "over" << over << std::endl;
	*/

	std::ofstream myFile (fname, std::ios::out | std::ios::binary);

	/* header */
	myFile.write(id, strlen(id));
	time_t t = time(nullptr);
	myFile.write((char *) &t, sizeof(time_t));

	/* obstacles */
	for(auto obs : obstacles)
		obs.write(myFile);

	/* cleanup */
	myFile.close();
	obstacles.clear();

	std::cout << "OSM written " << obstacles.size() << " shapes" << std::endl;
}


void OobWritter::addBazl(BazlCsv *bazl)
{
	/* adding shapes */
	obstacles.insert(obstacles.end(), bazl->obstacles.begin(), bazl->obstacles.end());
}
