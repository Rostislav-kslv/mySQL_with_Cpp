#ifndef USER_
#define USER_

#include <string>

class User {
  private:
    // int id;
    std::string username;
    std::string password;
    std::string email;
    std::string reg_date;

  public:
    User() = default;

    User(const std::string& username, const std::string& password, const std::string& email,
         const std::string& reg_date)
        : username(username), password(password), email(email), reg_date(reg_date)
    {
    }

    std::string getUsername()
    {
        return username;
    };
    std::string getPassword()
    {
        return password;
    };
    std::string getDate()
    {
        return reg_date;
    };
    std::string getEmail()
    {
        return email;
    };
    void setUsername(const std::string& name)
    {
        this->username = name;
    }
    void setPassword(const std::string& password)
    {
        this->password = password;
    }
    void setEmail(const std::string& email)
    {
        this->email = email;
    }
};

#endif /* USER_ */
