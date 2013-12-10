#include "Configuration.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <map>

enum contents {Undefined = 0, lat_n, lon_n, dayScene_n, nightScene_n, awayScene_n};
static std::map<std::string, contents> s_mapStringValues;

template <typename T>
T lexical_cast(const std::string& s) {
	std::stringstream ss(s);

	T result;
    if ((ss >> result).fail() || !(ss >> std::ws).eof()) {
		throw std::runtime_error("Bad cast");
	}

	return result;
}

Configuration::Configuration() : conf_ini_location("./cpp/examples/linux/server/Config.ini"), dayScene(""), nightScene(""), awayScene("") {
	create_string_map();
	std::ifstream conffile;
	if(open_filestream(conffile))
		if(parse_filestream(conffile))
		{}
}

bool Configuration::open_filestream(std::ifstream& conffile) {
	bool error = false;
	try {
		conffile.open(conf_ini_location.c_str());
		if(!conffile) {
			error = true;
			throw std::runtime_error("Could not open the file Config.ini");
		}
	}
	catch(std::exception const& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return !error;
}

bool Configuration::parse_filestream(std::ifstream& conffile) {
	std::string input;
	int linenr = 0;
	bool error = false;
	try {
		while(getline(conffile,input)) {
			++linenr;
			std::size_t found = input.find('=');
			if (found!=std::string::npos) {
				std::string name = input.substr(0,found);
				std::string value = input.substr(found+1);
				if(parse_variable(name, value))
				{}
			}
			else {
				error = true;
				throw std::runtime_error("Error in the Config.ini file, unable to parse line" + linenr);
			}
		}
		conffile.close();
	}
	catch(std::exception const& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return !error;
}

bool Configuration::parse_variable(std::string name, std::string value) {
	try {
		switch(s_mapStringValues[name])
		{
			case lat_n:
				lat = lexical_cast<float>(value);
				break;
			case lon_n:
				lon = lexical_cast<float>(value);
				break;
			case dayScene_n:
				dayScene = value;
				break;
			case nightScene_n:
				nightScene = value;
				break;
			case awayScene_n:
				awayScene = value;
				break;
			default:
				return false;
				break;
		}
	}
	catch (std::exception const& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return true;
}

void Configuration::create_string_map()
{
    s_mapStringValues["lat"] = lat_n;
	s_mapStringValues["lon"] = lon_n;
	s_mapStringValues["dayScene"] = dayScene_n;
	s_mapStringValues["nightScene"] = nightScene_n;
	s_mapStringValues["awayScene"] = awayScene_n;
}

bool Configuration::GetLocation(float &lat_, float &lon_) {
	lat_ = lat;
	lon_ = lon;
	return true;
}

bool Configuration::GetDayScene(std::string &dayScene_) {
	dayScene_ = dayScene;
	return true;
}
bool Configuration::GetNightScene(std::string &nightScene_) {
	nightScene_ = nightScene;
	return true;
}
bool Configuration::GetAwayScene(std::string &awayScene_) {
	awayScene_ = awayScene;
	return true;
}
