#pragma once
#include <string>
#include <cstring>

class User
{
private:
    std::string m_name;
    uint* m_password;
    const std::string m_login;

public:

    //Создание пользователя с заданными параметрами
    User(const std::string &name, const uint *password, const std::string &login)
        : m_name(name), m_login(login)
    {
        m_password = new uint[5];
        for(int i = 0; i < 5; ++i)
            m_password[i] = password[i];
    }
    
    //Возврат имени пользователя
    std::string get_name() const {return m_name;}

    //Изменением имени пользователя
    void set_name(const std::string &name)
    {
        m_name = name;
    }

    //Возврат логина пользователя
    std::string get_login() const {return m_login;}
    
    //Возврат пароля пользователя
    uint* get_password() const {return m_password;}

    //Изменение пароля пользователя
    void set_password(uint password[]) 
    {
        for(int i = 0; i < 5; ++i)
            m_password[i] = password[i];
    }

    ~User() 
    {
        //delete[] m_password;
        //m_password = nullptr;
    }
};