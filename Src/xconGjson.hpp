/*
 * xconGjson.h
 *
 *  Created on: Nov 23, 2023
 *      Author: sid
 */

#ifndef XCONGJSON_HPP_
#define XCONGJSON_HPP_

class XconGjson
{
private:


public:
	std::vector<Obstacle> obstacles;

	XconGjson() { }
	bool load(const std::string &fname);

};



#endif /* XCONGJSON_HPP_ */
