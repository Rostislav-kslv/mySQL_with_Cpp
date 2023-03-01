#ifndef USER_
#define USER_

#include <string>

class User {
  private:
    std::string username;

  public:
    User() = default;

    std::string getUsername()
    {
        return username;
    };
    void setUsername(const std::string& name)
    {
        username = name;
    }
};

#endif /* USER_ */
