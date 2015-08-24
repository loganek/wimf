#ifndef USER_H
#define USER_H

#include "protocol/data_types.h"

namespace Wimf {

class User
{
private:
	user_id id;
	double latitude = 0.0;
	double longitude = 0.0;

public:
	User (user_id id) : id (id) {}
	virtual ~User () {}

	user_id get_id () const { return id; }

	void set_coords (double latitude, double longitude)
	{
		this->latitude = latitude;
		this->longitude = longitude;
	}

	double get_latitude () const { return latitude; }
	double get_longitude () const { return longitude; }
};

}

#endif
