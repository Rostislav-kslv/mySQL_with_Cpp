
#include <mysql-cppconn-8/mysql/jdbc.h>
#include <iostream>
#include <memory>
#include "Blog.hpp"

int main()
{
    std::unique_ptr<Blog> blog =
        std::make_unique<Blog>(new DB("tcp://127.0.0.1:3306", "admin", "password", "blogdb"), new User);
    clear_screen;

    blog->Menu();

    return 0;
}
