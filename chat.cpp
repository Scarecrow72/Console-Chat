#include "chat.h"
#include "user.h"
#include <iostream>
#include <vector>
#include <exception>
#include <memory>
#include <algorithm>
#include <string>
#include <unordered_map>
#include "SHA-1.h"

typedef unsigned int uint;

//Выход из программы
void Chat::escape()
{
    m_start = false;
}

//Возврат флага-указателя что пользователь авторизовался
bool Chat::get_authorisation() const
{
    return m_authorisation;
}

//Функция возврата включенного состояния программы
bool Chat::is_chat_work() const
{
    return m_start;
}

//Функция установки указателя на текущего пользователя
void Chat::set_current_user(const User& autorised)
{
    m_current_user = std::make_unique<User>(autorised);
}

//Функция создания нового пользователя
void Chat::registration()
{
    std::cin.ignore(32767, '\n');
    
    //Создание нового логина
    std::string new_login;    
    std::cout << "Введите логин: ";
    std::getline(std::cin, new_login);
    
    //Проверка правильности ввода логина
    //Проверка логина на то что он не пуст.
    if(new_login.empty())
    {
        std::cout << "Вы не ввели логин." << std::endl;
        return;
    }
    else
    {
        //Проверка логина на его уникальность.
        try
        {
            
            std::unordered_map<std::string, User>::iterator it = m_users.find(new_login);
            if(it != m_users.end())
            {
                //Выброс ошибки занятого состояния введенного логина
                throw MatchLoginExc();
            }
            
        }
        catch(const std::exception& e)
        {
            //Вывод сообщения о том, что логин не уникален
            std::cerr << e.what() << '\n';
            return;
        } 
    }
    
    //Создание пароля
    std::string new_password;    
    std::cout   << "Придумайте пароль\n"
                << "Пароль должен содержать заглавные и строчные английские буквы, цифры и символы !,\", #, $, %, &, \', (, ) \n"
                << "Пароль не должен содержать пробелы и должен состоять из 8 символов" << std::endl;
    std::cout << "Введите пароль: ";
    std::getline(std::cin, new_password);
    
    //Проверка ввода пароля
    if(new_password.empty() || new_password.length() < 8)
    {
        std::cout << "Вы не ввели пароль." << std::endl;
        return;
    }
    else
    {
        //Проверка соответствия пароля условиям создания
        bool get_num {false}, get_symb {false}, get_big {false}, get_small {false};
        for(const char c : new_password)
        {
            if(c >= 65 && c <= 90)
                get_big = true;
            else if(c >= 33 && c <= 41)
                get_symb = true;
            else if(c >= 48 && c <= 57)
                get_num = true;   
            else if (c >= 97 && c <= 122)
                get_small = true;
        }
        
        //Проверка правильности создания пароля
        if(get_big && get_symb && get_num && get_small)
        {
            //Подтверждение пароля
            std::string correct_password;
            std::cout << "Подтвердите пароль: ";
            std::getline(std::cin, correct_password);
            
            //Проверка правильности подтверждения пароля
            if(new_password != correct_password)
            {
                std::cout << "Пароль не прошел подтверждение" << std::endl;
                return;
            }
        }
        else
        {
            //Вывод сообщения о том, что пароль не соответствует параметрам безопасности
            std::cout << "Пароль не соответствует требованиям безопасности" << std::endl;
            return;
        }
    }    
    
    //Создание имени пользователя
    std::string new_name;
    while(true)
    {
        std::cout << "Придумайте имя пользователя: ";
        std::getline(std::cin, new_name);
        if(new_name.empty())
            continue;
        else
            break;
    }
    
    //Добавление нового пользователя в список пользователей
    uint *sha1_pass = sha1(new_password.c_str(), sizeof(new_password.c_str()-1));
    m_users.insert({new_login, User(new_name, sha1_pass, new_login)});
    delete[] sha1_pass;
}

//Функция авторизации пользователя
void Chat::authorisation()
{
    //Ввод логина и пароля
    std::cout << "Введите логин: ";
    std::string user_login;
    std::cin.ignore(32767, '\n');
    std::getline(std::cin, user_login);
    
    //Проверка что пользователь ввел свой логин
    if(user_login.empty())
    {
        std::cout << "Вы не ввели логин" << std::endl;
        return;
    }
    std::cout << "Введите пароль: ";
    std::string user_password;
    std::getline(std::cin, user_password);

    //Проверка что пользователь ввел пароль
    if(user_password.empty())
    {
        std::cout << "Вы не ввели пароль" << std::endl;
        return;
    }
    try
    {
        //Проверка правильности ввода логина и пароля
        std::unordered_map<std::string, User>::iterator it = m_users.find(user_login);
        if(it != m_users.end())
        {
            //std::cout << "Пользователь найден" <<std::endl;
            if(user_login == it -> second.get_login())
            {
                //std::cout << "Логины пользователей совпадают" << std::endl;
                uint *sha1_user_password = sha1(user_password.c_str(), sizeof(user_password.c_str()));
                for(int i = 0; i < 5; ++i)
                {
                    if(*(sha1_user_password + i) != *((it -> second.get_password()) + i))
                        throw BadAutorisationExc();
                }
                std::cout << "Приветствую, " << it -> second.get_name() << std::endl;
                set_current_user(it -> second);
                m_authorisation = true;
                delete[] sha1_user_password;
                return;
            }
        }
        //Выброс ошибки авторизации
        else
            throw BadAutorisationExc();
    }
    //Вывод сообщения об ошибке авторизации
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

//Меню авторизации/регистрации пользователя
void Chat::show_start_menu()
{
    while(m_start && !m_authorisation)
    {
        std::cout   << "Приветствую!\n" << "Введите\n" << "l - для авторизации | r - для регистрации \n"
                    << "e - для выхода из программы" << std::endl;
        char key;
        std::cin >> key;
        switch(key)
        {
        //Вызов функции авторизации пользователя
        case 'l':
            {
                authorisation();
                break;
            }
        //Вызов функциии регистрации пользователя
        case 'r':
            {
                registration();
                break;
            }
        //Выход из программы
        case 'e':
            {
                escape();
                break;
            }
        default:
            {
                std::cin.ignore(32767, '\n');
                break;
            }
        }
    }
}

//Функция создания сообщения
void Chat::create_message()
{
    //Ввод адресата по логину для личного сообщения
    std::string address;
    bool match {false};
    do
    {
        std::cout   << "Введите логин пользователя для отправки личного сообщения"
                    << "или введите ALL для отправки сообщения всем пользователям" << std::endl;
        std::getline(std::cin, address);
    }
    while(address.empty());
    //Проверка существования адресата
    try
    {
        if(address != "ALL")
        {
            std::unordered_map<std::string, User>::iterator it = m_users.find(address);
            if(it != m_users.end())
            {
                match = true;
            }
            //Вывод ошибки, если адресата с таким логином не существует
            else
                throw NoMatchLoginExc();
        }
    }
    catch(const std::exception& e)
    {
        //Вывод сообщения о том, что пользователя с таким логином не существует
        std::cerr << e.what() << '\n';
        return;
    }
    //Ввод текста сообщения при правильном введенном адресе
    if(address == "ALL" || match)
    {
        std::cout << "Введите текст сообщения: ";
        std::string text;
        std::getline(std::cin, text);
        m_messages.push_back(Message((*m_current_user).get_login(), address, text));
    }
}

//Функция отображения списка пользователей
void Chat::show_users() const
{
    for(auto elem : m_users)
    {
        std::cout << "Пользователь: " << elem.second.get_login() << "\t\t Имя: " << elem.second.get_name();
        if((*m_current_user).get_login() == elem.second.get_login())
            std::cout << " <- это Вы";
        std::cout << std::endl;
    }
}

//Функция просмотра сообщений
void Chat::show_messages() const
{
    for(auto &message : m_messages)
    {
        //Проверка уровня доступа к сообщениям в чате если сообщение лично или если сообщение в общем чате
        if(message.get_to() == (*m_current_user).get_login() || message.get_to() == "ALL")
        {
            if(message.get_to() == (*m_current_user).get_login())
                std::cout << "Личное сообщение" << std::endl;
            else
                std::cout << "Общий чат" << std::endl;
            std::cout   << "Сообщение от: " << message.get_from() << '\n' << '\n'
                        << message.get_message() << '\n' << std::endl;
        }
    }
}

//Меню авторизованного пользователя
void Chat::show_user_menu()
{
    while(m_authorisation)
    {
        std::cout   << "w - создать сообщение  | l - просмотр списка пользователей\n" 
                    << "m - просмотр сообщений | p - смена пароля \n" 
                    << "n - смена имени        | d - удалиться из чата \n"
                    << "e - выход" << std::endl;
        std::string key;
        std::getline(std::cin, key);
        
        //Вызов функции создания сообщения
        if(key == "w")
        {
            create_message();
        }
        
        //Вызов функции просмотра сообщений
        else if(key == "l")
        {
            show_users();
        }
        
        //Вызов функции просмотра сообщений
        else if(key == "m")
        {
            show_messages();
        }
        
        //Выход из чата
        else if(key == "e")
        {
            m_current_user = nullptr;
            m_authorisation = false;
        }
        
        //Смена пароля пользователя
        else if(key == "p")
        {          
            std::string new_password;    
            std::cout   << "Пароль должен содержать заглавные и строчные английские буквы, цифры и символы !,\", #, $, %, &, \', (, ) \n"
                        << "Пароль не должен содержать пробелы и должен содержать не менее 8 символов" << std::endl;
            std::cout << "Введите новый пароль: ";
            std::getline(std::cin, new_password);
            
            //Проверка ввода пароля
            if(new_password.empty() || new_password.length() < 8)
            {
                std::cout << "Вы не ввели пароль." << std::endl;
                break;
            }
            else
            {
                //Проверка соответствия пароля условиям создания
                bool get_num {false}, get_symb {false}, get_big {false}, get_small {false};
                for(int i = 0; i < new_password.length(); ++i)
                {
                    if((new_password[i] >= 65) && (new_password[i] <= 90))
                        get_big = true;
                    else if((new_password[i] >= 33) && (new_password[i] <= 41))
                        get_symb = true;
                    else if((new_password[i] >= 48) && (new_password[i] <= 57))
                        get_num = true;   
                    else if ((new_password[i] >= 97) && (new_password[i] <= 122))
                        get_small = true;
                    else
                        continue;
                }
                //Проверка правильности создания пароля
                if(get_big && get_symb && get_num && get_small)
                {
                    //Подтверждение пароля
                    while(true)
                    {
                        std::string correct_password;
                        std::cout << "Подтвердите пароль: ";
                        std::getline(std::cin, correct_password);
                        //Проверка подверждения пароля
                        if(new_password != correct_password)
                        {
                            std::cout << "Пароль не прошел подтверждение" << std::endl;
                            break;
                        }
                        else
                        {
                            //Изменение пароля пользователя при успешном подтверждении
                            std::unordered_map<std::string, User>::iterator it = m_users.find((*m_current_user).get_login());
                            if(it != m_users.end())
                            {
                                if((*m_current_user).get_login() == it -> second.get_login())
                                {
                                    uint *sha1_pass = sha1(new_password.c_str(), sizeof(new_password.c_str()-1));
                                    (*m_current_user).set_password(sha1_pass);
                                    delete[] sha1_pass;
                                }
                            }
                            break;
                        }
                    }
                }   
                else
                {
                    std::cout << "Новый пароль не отвечает требованиям безопасности" << std::endl;
                }
            }             
        }
        
        //Смена имени пользователя
        else if(key == "n")
        {
            std::string new_name;
            std::cout << "Введите новое имя пользователя: ";
            std::getline(std::cin, new_name);
            std::unordered_map<std::string, User>::iterator it = m_users.find((*m_current_user).get_login());
            if(it != m_users.end())
            {
                if((*m_current_user).get_login() == it -> second.get_login())
                {
                    it -> second.set_name(new_name);
                    set_current_user(it -> second);
                }
            }
        }

        //Удаление из чата
        else if(key == "d")
        {
            std::unordered_map<std::string, User>::iterator it = m_users.find((*m_current_user).get_login());
            if(it != m_users.end())
            {
                if((*m_current_user).get_login() == it -> second.get_login())
                {
                    delete[] it -> second.get_password();
                    m_users.erase(it);
                    m_current_user = nullptr;
                    m_authorisation = false;
                }
            }
        }
        
        //Обработка ошибки введенной команды
        else
        {
            continue;
        }
    }
}

Chat::~Chat()
{
    for(auto elem : m_users)
        delete[] elem.second.get_password();
}