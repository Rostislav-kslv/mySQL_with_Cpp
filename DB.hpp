#ifndef DB_
#define DB_

#include <mysql-cppconn-8/mysql/jdbc.h>
#include <string>
#include <vector>
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
    bool createPost(const std::string& content, int blog_id);
    bool getBlogList(std::vector<std::string>& names, std::vector<std::string>& topics, std::vector<int>& ids);
    bool getPostList(const int blog_id, std::vector<std::string>& content, std::vector<int>& ids);
    bool createComment(const std::string& content, int blog_id);
    bool getCommentList(const int post_id, std::vector<std::string>& comments);

    ~DB()
    {
        delete con;
    }
};

#endif /* DB_ */
