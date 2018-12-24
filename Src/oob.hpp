/*
 * oob.hpp
 *
 *  Created on: 23 Dec 2018
 *      Author: sid
 */

#ifndef SRC_OOB_HPP_
#define SRC_OOB_HPP_

#include <vector>

#include "bazlCsv.hpp"
#include "coord3d.hpp"
#include "nodeDb.hpp"
#include "osmparser.hpp"

#define deg2rad(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define rad2deg(angleRadians) ((angleRadians) * 180.0 / M_PI)

class OobWritter
{
private:
	std::string fname;
	//todo writing started?
	int written = 0;
	int dropped = 0;

	//todo type... altitude...
	std::vector<Coord3D> track;

	NodeDB *db = nullptr;
	BazlCsv *bazl = nullptr;

public:
	OobWritter(std::string fname) : fname(fname) { }
	void setNodeDb(NodeDB *db) { this->db = db; }
	void addBazl(BazlCsv *bazl);
	void finish(void);


	void startElement(const std::string& name, const XmlAttr& attr);
	void endElement(const std::string& name);


};



#endif /* SRC_OOB_HPP_ */
