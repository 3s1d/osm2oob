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

	//todo type
}

void OobWritter::endElement(const std::string& name)
{
	if(name == "way")
	{
		/* check for similar version in BAZL data */
		if(bazl != nullptr && bazl->isSimilar(track.front(), track.back()))
		{
			dropped++;
			track.clear();
			return;
		}

		//todo build obstacle and write


		track.clear();
		written++;
	}
}


void OobWritter::finish(void)
{
	//todo close all


	std::cout << "OSM written " << written << " shapes (dropped: " << dropped << ")" << std::endl;
}


void OobWritter::addBazl(BazlCsv *bazl)
{
	/* add reference */
	this->bazl = bazl;

	//todo write

	//todo
	std::cout << "BAZL written " << bazl->numObst() << " shapes" << std::endl;

}
