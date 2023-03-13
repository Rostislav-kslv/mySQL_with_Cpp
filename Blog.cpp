#include "Blog.hpp"

int GetChoice(int min, int max)
{
    while (true) {
        std::cout << "\n\tEnter your choice: ";
        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(32767, '\n');
        } else if (choice >= min && choice <= max) {
            return choice;
        }
        std::cerr << "\n\tError! Plese select correct option." << std::endl;
    }
}

void Blog::Menu()
{
    std::cout << "\n\tChoose one option:" << std::endl;
    std::cout << "\n\t---1. LogIn" << std::endl;
    std::cout << "\n\t---2. SignIn" << std::endl;
    std::cout << "\n\n\t---3. Exit" << std::endl;
    int choice = GetChoice(1, 3);
    if (choice == 1) {
        LogIn();

    } else if (choice == 2) {
        SignIn();
    } else {
        clear_screen;
        return;
    }
}

void Blog::SignIn()
{
    clear_screen;
    std::cout << "\n\tSignIn" << std::endl;

    std::cout << "\n\t---Enter your username: ";
    std::string username;
    std::cin >> username;
    std::cout << "\n\t---Enter your mail address: ";
    std::string email;
    std::cin >> email;
    std::cout << "\n\t---Enter your password: ";
    std::string password;
    std::cin >> password;
    clear_screen;
    if (db_m->createUser(username, email, password)) {
        std::cout << "\n\tUser created" << std::endl;
        Menu();
    } else {
        std::cout << "\n\tFaild to create user" << std::endl;
        Menu();
    }
}

void Blog::LogIn()
{
    clear_screen;
    std::cout << "\n\tLogIn" << std::endl;

    std::cout << "\n\t---Enter your username: ";
    std::string username;
    std::cin >> username;
    std::cout << "\n\t---Enter your password: ";
    std::string password;
    std::cin >> password;
    clear_screen;
    if (db_m->getUser(username, password, user_m)) {
        MainPage();
    } else {
        Menu();
    }
}

void Blog::MainPage()
{
    std::cout << "\n\tWelcome, " << user_m->getUsername() << std::endl;

    std::cout << "\n\t---1. My profile" << std::endl;
    std::cout << "\n\t---2. Blogs" << std::endl;
    std::cout << "\n\t---3. Create new blog" << std::endl;
    std::cout << "\n\n\t---4. LogOut" << std::endl;

    int choice = GetChoice(1, 4);

    switch (choice) {
        case 1:
            clear_screen;
            Profile();
            break;
        case 2:
            clear_screen;
            BlogsPage();
            break;
        case 3:
            clear_screen;
            CreateBlog();
            break;
        case 4:
            delete user_m;
            clear_screen;
            Menu();
            break;
        default:
            break;
    }
}

void Blog::Profile()
{
    std::cout << "\n\tYour profile\n" << std::endl;
    std::cout << std::setw(25) << std::left << "\n\t* Username:" << std::setw(20) << std::left << user_m->getUsername()
              << std::endl;
    std::cout << std::setw(25) << std::left << "\n\t* Registration date:" << std::setw(10) << std::left
              << user_m->getDate() << std::endl;
    std::cout << std::setw(25) << std::left << "\n\t* E-mail address:" << std::setw(20) << std::left
              << user_m->getEmail() << std::endl;

    std::cout << "\n\n\tSettings: " << std::endl;
    std::cout << "\n\t---1. Change password" << std::endl;
    std::cout << "\n\t---2. Change username" << std::endl;
    std::cout << "\n\t---3. Change e-mail address" << std::endl;
    std::cout << "\n\n\t---4. Go back to main page" << std::endl;

    int choice = GetChoice(1, 4);

    switch (choice) {
        case 1:
            clear_screen;
            ChangePassword();
            break;
        case 2:
            clear_screen;
            ChangeUsername();
            break;
        case 3:
            clear_screen;
            ChangeEmail();
            break;
        case 4:
            clear_screen;
            MainPage();
            break;
        default:
            break;
    }
}

void Blog::ChangePassword()
{
    std::cout << "\n\tEnter your current password: ";
    std::string password;
    std::cin >> password;
    if (password == user_m->getPassword()) {
        std::cout << "\n\n\tEnter your new password: ";
        std::string new_password;
        std::cin >> new_password;
        if (db_m->changeUserField("passwd", new_password, user_m)) {
            clear_screen;
            std::cout << "\n\n\t***Password switched successful***" << std::endl;
            Profile();
        }
    } else {
        clear_screen;
        std::cout << "\n\t***Password is not correct, try again***" << std::endl;
        ChangePassword();
    }
}

void Blog::ChangeUsername()
{
    std::cout << "\n\tEnter your new username or enter your current if you don't want to change it: ";
    std::string username;
    std::cin >> username;
    if (username == user_m->getUsername()) {
        clear_screen;
        Profile();
    } else {
        if (db_m->changeUserField("user_name", username, user_m)) {
            clear_screen;
            std::cout << "\n\n\t***Username switched successful***" << std::endl;
            Profile();
        } else {
            clear_screen;
            std::cout << "\n\t***Username is already taken***" << std::endl;
            ChangeUsername();
        }
    }
}

void Blog::ChangeEmail()
{
    std::cout << "\n\tEnter your new email or enter your current if you don't want to change it: ";
    std::string email;
    std::cin >> email;
    if (email == user_m->getEmail()) {
        clear_screen;
        Profile();
    } else {
        if (db_m->changeUserField("email_addr", email, user_m)) {
            clear_screen;
            std::cout << "\n\n\t***Email switched successful***" << std::endl;
            Profile();
        } else {
            clear_screen;
            std::cout << "\n\t***Email is already taken***" << std::endl;
            ChangeEmail();
        }
    }
}

void Blog::CreateBlog()
{
    std::cout << "\n\tEnter blog name: ";
    std::string name;
    std::cin.ignore(1);
    std::getline(std::cin, name);
    std::cout << "\n\tChoose blog topic: ";
    std::string topic;
    std::getline(std::cin, topic);
    if (db_m->createBlog(name, topic)) {
        clear_screen;
        std::cout << "\n\t***Blog created***";
        MainPage();
    } else {
        clear_screen;
        std::cout << "\n\t***Error! Blog wasn't created***";
        MainPage();
    }
}

void Blog::BlogPagePrint(int page, const std::vector<std::string>& names, const std::vector<std::string>& topics,
                         const std::vector<int>& ids)
{
    if ((page - 1) * 5 < names.size() && page > 0) {
        clear_screen;
        std::cout << "\n\tBlogs | Page #" << page << std::endl;
        for (int i = (page - 1) * 5; i < names.size(); i++) {
            std::cout << "\n\t* " << std::setw(2) << std::right << (i % 5) + 1 << ". " << std::setw(25) << std::left
                      << names[i] << " | " << std::setw(25) << std::left << topics[i] << std::endl;
            if ((i + 1) % 5 == 0) {
                break;
            }
        }
        std::cout << "\n\n\t---Enter blog number to open it's page" << std::endl;
        std::cout << "\n\t---Enter (0) to select page" << std::endl;
        std::cout << "\n\t---Enter (6) to go back to main page" << std::endl;

        int choice = GetChoice(0, 6);
        if (choice == 0) {
            std::cout << "\n\tEnter the number of page: ";
            std::cin >> page;
            BlogPagePrint(page, names, topics, ids);
        } else if (choice == 6) {
            clear_screen;
            MainPage();
        } else {
            clear_screen;
            PostsPage(ids[choice + (page - 1) * 5 - 1]);
        }
    } else {
        clear_screen;
        std::cout << "\n\t***Current page does not exist***" << std::endl;
        MainPage();
    }
}

void Blog::PostPagePrint(int page, const std::vector<std::string>& content, const std::vector<int> ids,
                         const int blog_id)
{
    if (((page - 1) * 5 < content.size() && page > 0) || content.empty()) {
        clear_screen;
        std::cout << "\n\tBlog_ID " << blog_id << std::endl;

        std::cout << "\n\tPosts | Page #" << page << std::endl;
        for (int i = (page - 1) * 5; i < content.size(); i++) {
            std::cout << "\n\t* " << std::setw(2) << std::right << (i % 5) + 1 << ". " << std::setw(25) << std::left
                      << content[i] << std::endl;
            if ((i + 1) % 5 == 0) {
                break;
            }
        }
        std::cout << "\n\n\t---Enter post number to open it's page" << std::endl;
        std::cout << "\n\t---Enter (0) to select page" << std::endl;
        std::cout << "\n\t---Enter (6) to create post" << std::endl;
        std::cout << "\n\t---Enter (7) to go back to main page" << std::endl;

        int choice = GetChoice(0, 7);
        if (choice == 0) {
            std::cout << "\n\tEnter the number of page: ";
            std::cin >> page;
            PostPagePrint(page, content, ids, blog_id);
        } else if (choice == 6) {
            std::cout << "\n\tCreate post";
            std::cout << "\n\tPost content: ";
            std::string text;
            std::cin.ignore(1);
            std::getline(std::cin, text);
            if (db_m->createPost(text, blog_id)) {
                clear_screen;
                std::cout << "\n\t***Post created***";
                MainPage();
            } else {
                clear_screen;
                std::cout << "\n\t***Error creating post***";
                MainPage();
            }

        } else if (choice == 7) {
            clear_screen;
            MainPage();
        } else {
            clear_screen;
            CommentsPage(ids[choice + (page - 1) * 5 - 1]);
        }
    } else {
        clear_screen;
        std::cout << "\n\t***Current page does not exist***" << std::endl;
        MainPage();
    }
}

void Blog::CommentPagePrint(int page, const std::vector<std::string>& comments, const int post_id)
{
    if (((page - 1) * 5 < comments.size() && page > 0) || comments.empty()) {
        clear_screen;
        std::cout << "\n\tPost_ID " << post_id << std::endl;

        std::cout << "\n\tComments| Page #" << page << std::endl;
        for (int i = (page - 1) * 5; i < comments.size(); i++) {
            std::cout << "\n\t* " << std::setw(2) << std::right << (i % 5) + 1 << ". " << std::setw(25) << std::left
                      << comments[i] << std::endl;
            if ((i + 1) % 5 == 0) {
                break;
            }
        }

        std::cout << "\n\n\t---Enter (0) to select page" << std::endl;
        std::cout << "\n\t---Enter (1) to create new comment" << std::endl;
        std::cout << "\n\t---Enter (2) to go back to main page" << std::endl;

        int choice = GetChoice(0, 2);
        if (choice == 0) {
            std::cout << "\n\tEnter the number of page: ";
            std::cin >> page;
            CommentPagePrint(page, comments, post_id);
        } else if (choice == 1) {
            std::cout << "\n\tCreate comment";
            std::cout << "\n\tComment content: ";
            std::string text;
            std::cin.ignore(1);
            std::getline(std::cin, text);
            clear_screen;
            if (db_m->createComment(text, post_id)) {
                std::cout << "\n\t***Comment created***";
                MainPage();
            } else {
                std::cout << "\n\t***Error creating comment***";
                MainPage();
            }

        } else if (choice == 2) {
            clear_screen;
            MainPage();
        }
    } else {
        clear_screen;
        std::cout << "\n\t***Current page does not exist***" << std::endl;
        MainPage();
    }
}

void Blog::CommentsPage(const int post_id)
{
    std::vector<std::string> comments;

    if (db_m->getCommentList(post_id, comments)) {
        CommentPagePrint(1, comments, post_id);
    } else {
        clear_screen;
        std::cout << "\n\t***Error! Smth went wrong***";
        BlogsPage();
    }
}

void Blog::PostsPage(int blog_id)
{
    std::vector<std::string> content;
    std::vector<int> ids;

    if (db_m->getPostList(blog_id, content, ids)) {
        PostPagePrint(1, content, ids, blog_id);
    } else {
        clear_screen;
        std::cout << "\n\t***Error! Smth went wrong***";
        BlogsPage();
    }
}

void Blog::BlogsPage()
{
    std::vector<std::string> names;
    std::vector<std::string> topics;
    std::vector<int> ids;

    if (db_m->getBlogList(names, topics, ids)) {
        BlogPagePrint(1, names, topics, ids);
    } else {
        clear_screen;
        std::cout << "\n\t***Error! Smth went wrong***";
        MainPage();
    }
}
