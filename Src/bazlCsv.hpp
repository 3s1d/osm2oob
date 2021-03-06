/*
 * bazlCsv.hpp
 *
 *  Created on: 24 Dec 2018
 *      Author: sid
 */

#ifndef SRC_BAZLCSV_HPP_
#define SRC_BAZLCSV_HPP_

#include <string>
#include <vector>

#include "obstacle.hpp"

class BazlCsv
{
private:
	const std::string delimeter = "#,;";

public:
	std::vector<Obstacle> obstacles;

	BazlCsv() { }
	void add(std::string fname);

	bool isSimilar(const Coord3D &start, const Coord3D &end);

	int numObst(void) { return obstacles.size(); }
};



#endif /* SRC_BAZLCSV_HPP_ */
