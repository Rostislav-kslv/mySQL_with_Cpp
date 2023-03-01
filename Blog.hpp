#ifndef BLOG_
#define BLOG_

#include "DB.hpp"
#include "User.hpp"

#define clear_screen std::cout << "\x1B[2J\x1B[H"

class Blog {
  private:
    DB* db_m;
    User* user_m;

  public:
    Blog(DB* db, User* user) : db_m(db), user_m(user)
    {
    }

    ~Blog()
    {
        delete db_m;
        delete user_m;
    }

    void Menu();
    void LogIn();
    void SignIn();
};

int GetChoice()
{
    while (true) {
        std::cout << "\n\tEnter your choice: ";
        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(32767, '\n');
        } else if (choice == 1 || choice == 2) {
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
    int choice = GetChoice();
    if (choice == 1) {
        LogIn();

    } else {
        SignIn();
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

    if (db_m->createUser(username, email, password)) {
        std::cout << "\n\tUser created" << std::endl;
    } else {
        std::cout << "\n\tFaild to create user" << std::endl;
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

    db_m->getUser(username, password);
}

#endif /* TMP_BLOG_ */
