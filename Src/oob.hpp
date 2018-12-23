/*
 * oob.hpp
 *
 *  Created on: 23 Dec 2018
 *      Author: sid
 */

#ifndef SRC_OOB_HPP_
#define SRC_OOB_HPP_

#include <vector>

#include "nodeDb.hpp"
#include "osmparser.hpp"

class OobWritter
{
private:
	std::string fname;
	//todo writing started?
	int written = 0;

	//todo type... altitude...
	std::vector<coord3d> track;

	NodeDB *db = nullptr;

public:
	OobWritter(std::string fname) : fname(fname) { }
	void setNodeDb(NodeDB *db) { this->db = db; }
	void finish(void);


	void startElement(const std::string& name, const XmlAttr& attr);
	void endElement(const std::string& name);


};



#endif /* SRC_OOB_HPP_ */
