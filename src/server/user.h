#ifndef USER_H
#define USER_H

#include "protocol/data_types.h"

namespace Wimf {

class User
{
private:
	user_id id;

public:
	User (user_id id) : id (id) {}
	virtual ~User () {}

	user_id get_id () const { return id; }
};

}

#endif
