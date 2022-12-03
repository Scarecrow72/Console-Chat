#pragma once
#include <string>

class Message
{
private:
    std::string m_from;
    std::string m_to;
    std::string m_text;

public:
    //Создание сообщения с заданными параметрами
    Message(const std::string &from, const std::string &to, const std::string &text)
    : m_from(from), m_to(to), m_text(text)
    {}

    //Возврат логина отправителя сообщения
    std::string get_from() const {return m_from;}
    
    //Возврат логина получателя сообщения
    std::string get_to() const {return m_to;}
    
    //Возврат текста сообщения
    std::string get_message() const {return m_text;}
};