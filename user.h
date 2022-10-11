#pragma once
#include <string>

class User
{
private:
    std::string m_name;
    std::string m_password;
    const std::string m_login;

public:

    //Создание пользователя с заданными параметрами
    User(std::string &name, std::string &password, std::string &login)
        : m_name(name), m_password(password), m_login(login)
    {}
    
    //Возврат имени пользователя
    std::string get_name() const {return m_name;}

    //Изменением имени пользователя
    void set_name(std::string &name)
    {
        m_name = name;
    }

    //Возврат логина пользователя
    std::string get_login() const {return m_login;}
    
    //Возврат пароля пользователя
    std::string get_password() const {return m_password;}

    //Изменение пароля пользователя
    void set_password(std::string &password) {m_password = password;}

    ~User() {}
};