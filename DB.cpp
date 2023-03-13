#include "DB.hpp"

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
            std::cout << "\n\t***Username is already taken or email already exists in database***" << std::endl;
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
            user = new User(res->getString("user_name"), res->getString("passwd"), res->getString("email_addr"),
                            res->getString("registration_date"));

        } else {
            std::cout << "\n\t***Wrong password or username***" << std::endl;
        }

        delete pstmt;
        delete res;
    } catch (sql::SQLException& e) {
        if (e.getErrorCode() == 0) {
            std::cout << "\n\t***Wrong password or username***" << std::endl;
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
            std::cout << "\n\t***Blog with current name already exists***" << std::endl;
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

bool DB::getBlogList(std::vector<std::string>& names, std::vector<std::string>& topics, std::vector<int>& ids)
{
    try {
        sql::ResultSet* res;
        sql::Statement* stmt;

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM Blog");

        while (res->next()) {
            names.push_back(res->getString("blog_name"));
            topics.push_back(res->getString("topic"));
            ids.push_back(res->getInt("id"));
        }

        delete stmt;
        delete res;
    } catch (sql::SQLException& e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
    return true;
}

bool DB::getPostList(const int blog_id, std::vector<std::string>& content, std::vector<int>& ids)
{
    try {
        sql::ResultSet* res;
        sql::PreparedStatement* pstmt;
        pstmt = con->prepareStatement("SELECT * FROM Post WHERE blog_id=(?)");
        pstmt->setInt(1, blog_id);
        res = pstmt->executeQuery();

        while (res->next()) {
            ids.push_back(res->getInt("id"));
            content.push_back(res->getString("content"));
        }

        delete pstmt;
        delete res;
    } catch (sql::SQLException& e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
    return true;
}

bool DB::createPost(const std::string& content, int blog_id)
{
    sql::PreparedStatement* pstmt;
    try {
        pstmt = con->prepareStatement("INSERT INTO Post(blog_id, content) VALUES (?, ?)");
        pstmt->setInt(1, blog_id);
        pstmt->setString(2, content);
        pstmt->execute();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
    return true;
}

bool DB::createComment(const std::string& content, int blog_id)
{
    sql::PreparedStatement* pstmt;
    try {
        pstmt = con->prepareStatement("INSERT INTO Comments(post_id, content) VALUES (?, ?)");
        pstmt->setInt(1, blog_id);
        pstmt->setString(2, content);
        pstmt->execute();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
    return true;
}

bool DB::getCommentList(const int post_id, std::vector<std::string>& comments)
{
    try {
        sql::ResultSet* res;
        sql::PreparedStatement* pstmt;
        pstmt = con->prepareStatement("SELECT * FROM Comments WHERE post_id=(?)");
        pstmt->setInt(1, post_id);
        res = pstmt->executeQuery();

        while (res->next()) {
            comments.push_back(res->getString("content"));
        }

        delete pstmt;
        delete res;
    } catch (sql::SQLException& e) {
        std::cout << "# ERR: SQLException in " << __FILE__;
        std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cout << "# ERR: " << e.what();
        std::cout << " (MySQL error code: " << e.getErrorCode();
        std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
    return true;
}
