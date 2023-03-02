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
};

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
            // blogs
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

#endif /* TMP_BLOG_ */
