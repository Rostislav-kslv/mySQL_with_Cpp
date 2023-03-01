#ifndef DB_
#define DB_

#include <mysql-cppconn-8/mysql/jdbc.h>
#include <string>

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
    bool getUser(const std::string& username, const std::string& password);

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

bool DB::getUser(const std::string& username, const std::string& password)
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
            std::cout << "\n\t***You successfully logged in!***" << std::endl;
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
        }
        return false;
    }
    return true;
}

#endif /* DB_ */
