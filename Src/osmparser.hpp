/*
 * xml.hpp
 *
 *  Created on: 23 Dec 2018
 *      Author: sid
 */

#ifndef SRC_OSMPARSER_HPP_
#define SRC_OSMPARSER_HPP_

#include <expat.h>
#include <fstream>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>


/*
 * node_parser
 */

class XmlAttr
{
private:
	const char** attr_;

public:
	explicit XmlAttr(const char** attr) : attr_(attr) { }
	std::string operator[](const std::string& key) const { return get(key); }

	std::string get(const std::string& key) const
	{
	        for (int i = 0; attr_[i]; i += 2) {
	                if (key == attr_[i])
	                        return std::string(attr_[i + 1]);
	        }
	        throw std::runtime_error(key + " not found");
	}

	int64_t asInt64(const std::string& key) const { return std::atoll(get(key).c_str()); }
	double asDouble(const std::string& key) const { return std::atof(get(key).c_str()); }
};

class ParseError: public std::runtime_error
{
private:
	int where_;

public:
	ParseError(const std::string& what, int where) : std::runtime_error(what), where_(where) { }
	int where() const { return where_; }
};

template<class Handler>
class OsmParser: public boost::noncopyable
{
private:
	static void start_element(void* data, const char* name, const char** attr)
	{
		static_cast<Handler*>(data)->startElement(std::string(name), XmlAttr(attr));
	}

	static void end_element(void* data, const char* name)
	{
		static_cast<Handler*>(data)->endElement(std::string(name));
	}

	XML_Parser parser_;

public:
	explicit OsmParser(Handler& handler) : parser_(XML_ParserCreate(0))
	{
		if (!parser_)
			throw std::runtime_error("Could not allocate parser");
		XML_SetUserData(parser_, &handler);
		XML_SetElementHandler(parser_, start_element, end_element);
	}
	~OsmParser()
	{
		XML_ParserFree(parser_);
	}

	void parse(std::istream& in)
	{
		char buf[64 * 1024];
		while (!in.eof())
		{
			in.read(buf, sizeof(buf));
			if (!XML_Parse(parser_, buf, in.gcount(), 0))
				throw ParseError(XML_ErrorString(XML_GetErrorCode(parser_)), XML_GetCurrentLineNumber(parser_));
		}

		XML_Parse(parser_, 0, 0, 1);
	}

	void parse(const std::string& name)
	{

		std::ifstream file(name.c_str(), std::ios::in | std::ios::binary);
		if (!file.good())
			throw std::runtime_error("failed to open file " + name);
		if (name.rfind(".gz") == name.size() - 3)
		{
			boost::iostreams::filtering_stream<boost::iostreams::input> in;
			in.push(boost::iostreams::gzip_decompressor());
			in.push(file);
			parse(in);
		}
		else if (name.rfind(".bz2") == name.size() - 4)
		{
			boost::iostreams::filtering_stream<boost::iostreams::input> in;
			in.push(boost::iostreams::bzip2_decompressor());
			in.push(file);
			parse(in);
		}
		else
		{
			parse(file);
		}
	}
};


#endif /* SRC_OSMPARSER_HPP_ */
