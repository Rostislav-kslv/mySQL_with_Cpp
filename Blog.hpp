#ifndef BLOG_
#define BLOG_

#include <iomanip>
#include "DB.hpp"
#include "User.hpp"

#define clear_screen std::cout << "\x1B[2J\x1B[H"

class Blog {
  private:
    DB* db_m;
    User* user_m;

  public:
    Blog(DB* db) : db_m(db)
    {
    }

    ~Blog()
    {
        delete db_m;
    }

    void Menu();
    void LogIn();
    void SignIn();
    void MainPage();
    void Profile();
    void ChangePassword();
    void ChangeUsername();
    void ChangeEmail();
    void CreateBlog();
    void BlogPagePrint(int page, const std::vector<std::string>& names, const std::vector<std::string>& topics,
                       const std::vector<int>& ids);
    void PostPagePrint(int page, const std::vector<std::string>& content, const std::vector<int> ids,
                       const int blog_id);
    void CommentPagePrint(int page, const std::vector<std::string>& comments, const int post_id);
    void CommentsPage(const int post_id);
    void PostsPage(int blog_id);
    void BlogsPage();
};

#endif /* TMP_BLOG_ */
