#ifndef DB_
#define DB_

#include <mysql-cppconn-8/mysql/jdbc.h>
#include <string>
#include "User.hpp"

class DB {
  private:
    std::string root_m;
    std::string password_m;
    std::string addr_m;
    sql::Driver* driver;
    sql::Connection* con;

  public:
    DB(const std::string& addr, const std::string& root, const std::string& password, const std::string& schema)
        : root_m(root), password_m(password), addr_m(addr)
    {
        driver = get_driver_instance();
        con = driver->connect(addr_m, root_m, password_m);
        con->setSchema(schema);
    }

    bool createUser(const std::string& username, const std::string& email, const std::string& password);
    bool getUser(const std::string& username, const std::string& password, User*& user);
    bool changeUserField(const std::string& field, const std::string& value, User*& user);
    bool createBlog(const std::string& name, const std::string& topic);

    ~DB()
    {
        delete con;
    }
};

bool DB::createUser(const std::string& username, const std::string& email, const std::string& password)
{
    sql::PreparedStatement* pstmt;
    try {
        pstmt = con->prepareStatement(
            "INSERT INTO Users(user_name, email_addr, passwd, registration_date) VALUES (?, ?, ?, CURDATE())");
        pstmt->setString(1, username);
        pstmt->setString(2, email);
        pstmt->setString(3, password);
        pstmt->execute();
        delete pstmt;
    } catch (sql::SQLException& e) {
        if (e.getErrorCode() == 1062) {
            // clear_screen;
            std::cout << "\n\t***Username is already taken or email already exists in database***" << std::endl;
            // Menu();
        } else {
            std::cout << "# ERR: SQLException in " << __FILE__;
            std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
            std::cout << "# ERR: " << e.what();
            std::cout << " (MySQL error code: " << e.getErrorCode();
            std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
        return false;
    }
    return true;
}

bool DB::getUser(const std::string& username, const std::string& password, User*& user)
{
    try {
        sql::ResultSet* res;
        sql::PreparedStatement* pstmt;

        pstmt = con->prepareStatement("SELECT * FROM Users WHERE user_name IN (?)");
        pstmt->setString(1, username);
        res = pstmt->executeQuery();
        res->next();

        if (res->getString("passwd") == password) {
            // clear_screen;
            // std::cout << "\n\t***You successfully logged in!***" << std::endl;
            user = new User(res->getString("user_name"), res->getString("passwd"), res->getString("email_addr"),
                            res->getString("registration_date"));

        } else {
            // clear_screen;
            std::cout << "\n\t***Wrong password or username***" << std::endl;
            // Menu();
        }

        delete pstmt;
        delete res;
    } catch (sql::SQLException& e) {
        if (e.getErrorCode() == 0) {
            std::cout << "\n\t***Wrong password or username***" << std::endl;
            // Menu();
        } else {
            std::cout << "# ERR: SQLException in " << __FILE__;
            std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
            std::cout << "# ERR: " << e.what();
            std::cout << " (MySQL error code: " << e.getErrorCode();
            std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
        return false;
    }
    return true;
}

bool DB::changeUserField(const std::string& field, const std::string& value, User*& user)
{
    try {
        sql::ResultSet* res;
        sql::PreparedStatement* pstmt;

        if (field == "passwd") {
            pstmt = con->prepareStatement("UPDATE Users SET passwd = (?) WHERE user_name=(?)");
        } else if (field == "user_name") {
            pstmt = con->prepareStatement("UPDATE Users SET user_name = (?) WHERE user_name=(?)");
        } else if (field == "email_addr") {
            pstmt = con->prepareStatement("UPDATE Users SET email_addr = (?) WHERE user_name=(?)");
        }

        pstmt->setString(1, value);
        pstmt->setString(2, user->getUsername());
        res = pstmt->executeQuery();

        delete pstmt;
        delete res;
    } catch (sql::SQLException& e) {
        if (e.getErrorCode() == 1062) {
            std::cout << "\n\t***Already exists in system***" << std::endl;
            // Menu();
        } else {
            std::cout << "# ERR: SQLException in " << __FILE__;
            std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
            std::cout << "# ERR: " << e.what();
            std::cout << " (MySQL error code: " << e.getErrorCode();
            std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }

        return false;
    }
    if (field == "passwd") {
        user->setPassword(value);
    } else if (field == "user_name") {
        user->setUsername(value);
    } else if (field == "email_addr") {
        user->setEmail(value);
    }
    return true;
}

bool DB::createBlog(const std::string& name, const std::string& topic)
{
    sql::PreparedStatement* pstmt;
    try {
        pstmt = con->prepareStatement("INSERT INTO Blog(blog_name, topic) VALUES (?, ?)");
        pstmt->setString(1, name);
        pstmt->setString(2, topic);
        pstmt->execute();
        delete pstmt;
    } catch (sql::SQLException& e) {
        if (e.getErrorCode() == 1062) {
            // clear_screen;
            std::cout << "\n\t***Blog with current name already exists***" << std::endl;
            // Menu();
        } else {
            std::cout << "# ERR: SQLException in " << __FILE__;
            std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
            std::cout << "# ERR: " << e.what();
            std::cout << " (MySQL error code: " << e.getErrorCode();
            std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        }
        return false;
    }
    return true;
}

#endif /* DB_ */
