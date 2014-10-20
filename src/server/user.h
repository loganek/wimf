#ifndef USER_H
#define USER_H

namespace Wimf {

class User
{
 private:
  long phone;

 public:
  User (long phone);
  virtual ~User () {}
};

}

#endif
