#ifndef USER_H
#define USER_H

#include <string>
#include <cmath>

namespace Wimf {

class User
{
private:
	std::int32_t id;
	std::string nickname;
	static constexpr double invalid_coord = 99999.0;
	double latitude = invalid_coord;
	double longitude = invalid_coord;
	double radius = 1.0;

public:
	User (std::int32_t id, const std::string& user_name) : id (id), nickname (user_name) {}
	virtual ~User () {}

	std::int32_t get_id () const { return id; }
	std::string get_nickname () const { return nickname; }

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

	// todo move somewhere else; "utils", or something like that
	// todo create new class Point/Coordinates?
	static bool coords_valid (double latitude, double longitude)
	{
		return !(latitude < -90.0 || latitude > 90.0 || longitude < -180.0 || longitude > 180.0);
	}
};

}

#endif
