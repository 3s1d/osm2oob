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
		//todo writting


		track.clear();
		written++;
	}
}


void OobWritter::finish(void)
{
	//todo close all


	std::cout << "Written " << written << " shapes" << std::endl;
}