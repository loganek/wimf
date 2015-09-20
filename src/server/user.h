#ifndef USER_H
#define USER_H

#include "protocol/data_types.h"

#include <string>
#include <cmath>

namespace Wimf {

class User
{
private:
	user_id id;
	std::string nickname;
	double latitude = 0.0;
	double longitude = 0.0;
	double radius = 1.0;

public:
	User (user_id id, const std::string& user_name) : id (id), nickname (user_name) {}
	virtual ~User () {}

	user_id get_id () const { return id; }

	void set_coords (double latitude, double longitude)
	{
		this->latitude = latitude;
		this->longitude = longitude;
	}

	double get_latitude () const { return latitude; }
	double get_longitude () const { return longitude; }

	bool in_range (double latitude, double longitude)
	{
		return std::pow (latitude - this->latitude, 2) + std::pow (longitude - this->longitude, 2) < radius * radius;
	}
};

}

#endif
