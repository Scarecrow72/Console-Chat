#include "chat.h"
#include "user.h"
#include <iostream>
#include <vector>
#include <exception>
#include <memory>
#include <algorithm>

//Выход из программы
void Chat::escape()
{
    m_start = false;
}

//Возврат флага-указателя что пользователь авторизовался
bool Chat::get_autorisation() const
{
    return m_autorisation;
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
            for(auto &user : m_users)
            {
                if(new_login == user.get_login())
                {
                    //Выброс ошибки занятого состояния введенного логина
                    throw MatchLoginExc();
                }
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
    std::cout   << "Придумайте пароль"
                << "Пароль должен содержать заглавные и строчные английские буквы, цифры и символы !,\", #, $, %, &, \', (, ) \n"
                << "Пароль не должен содержать пробелы" << std::endl;
    std::cout << "Введите пароль: ";
    std::getline(std::cin, new_password);
    
    //Проверка ввода пароля
    if(new_password.empty())
    {
        std::cout << "Вы не ввели пароль." << std::endl;
        return;
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
    m_users.push_back(User(new_name, new_password, new_login));
}

//Функция авторизации пользователя
void Chat::autorisation()
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
        for(auto &user : m_users)
        {
            if(user_login == user.get_login())
            {
                if(user_password == user.get_password())
                {
                    std::cout << "Приветствую, " << user.get_name() << std::endl;
                    set_current_user(user);
                    m_autorisation = true;
                }
            }
        }
        //Выброс ошибки авторизации
        if(m_current_user == nullptr)
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
    while(true)
    {
        std::cout   << "Приветствую!\n" << "Введите\n" << "l - для авторизации | r - для регистрации \n"
                    << "e - для выхода из программы" << std::endl;
        std::string key;
        std::cin >> key;
        
        //Вызов функции авторизации пользователя
        if(key == "l")
        {
            autorisation();
            break;
        }
        
        //Вызов функциии регистрации пользователя
        else if(key == "r")
        {
            registration();
            continue;
        }
        
        //Выход из программы
        else if(key == "e")
        {
            escape();
            break;
        }
        
        //Обработка ошибки введенной команды
        else
        {
            continue;
        }
    }
}

//Функция создания сообщения
void Chat::create_message()
{
    //Ввод адресата по логину для личного сообщения
    std::string adress;
    bool match {false};
    do
    {
        std::cout   << "Введите логин пользователя для отправки личного сообщения"
                    << "или введите ALL для отправки сообщения всем пользователям" << std::endl;
        std::getline(std::cin, adress);
    }
    while(adress.empty());
    //Проверка существования адресата
    try
    {
        if(adress != "ALL")
        {
            for(auto &user : m_users)
            {
                if(adress == user.get_login())
                {
                    match = true;
                }
            }
            //Вывод ошибки, если адресата с таким логином не существует
            if(match == false)
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
    if(adress == "ALL" || match)
    {
        std::cout << "Введите текст сообщения: ";
        std::string text;
        std::getline(std::cin, text);
        m_messages.push_back(Message((*m_current_user).get_login(), adress, text));
    }
}

//Функция отображения списка пользователей
void Chat::show_users() const
{
    for(auto &user : m_users)
    {
        std::cout << "Пользователь: " << user.get_login() << "\t\t Имя: " << user.get_name();
        if((*m_current_user).get_login() == user.get_login())
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
    while(m_autorisation)
    {
        std::cout   << "w - создать сообщение  | l - просмотр списка пользователей\n" 
                    << "m - просмотр сообщений | p - смена пароля \n" 
                    << "n - смена имени        | e - выход" << std::endl;
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
            m_autorisation = false;
        }
        
        //Смена пароля пользователя
        else if(key == "p")
        {          
            std::string new_password;    
            std::cout   << "Пароль должен содержать заглавные и строчные английские буквы, цифры и символы !,\", #, $, %, &, \', (, ) \n"
                        << "Пароль не должен содержать пробелы" << std::endl;
            std::cout << "Введите новый пароль: ";
            std::getline(std::cin, new_password);
            
            //Проверка ввода пароля
            if(new_password.empty())
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
                            for(auto &user: m_users)
                            {
                                if((*m_current_user).get_login() == user.get_login())
                                {
                                    user.set_password(new_password);
                                    set_current_user(user);
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
            for(auto &user : m_users)
            {
                if((*m_current_user).get_login() == user.get_login())
                {
                    user.set_name(new_name);
                    set_current_user(user);
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