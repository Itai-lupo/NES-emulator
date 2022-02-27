#pragma once
#include <stdexcept>
#include <string>

class romFileNotFoundException: public std::exception
{
    private:
        std::string msg;

    public:
        explicit romFileNotFoundException(const std::string& filePath)
            : msg(filePath + " rom file not found error") {}

        virtual ~romFileNotFoundException() noexcept {}

        virtual const char* what() const noexcept 
        { 
            return msg.c_str();
        }

};


class badRomHeaderException: public std::exception
{
    private:
        std::string msg;

    public:
        explicit badRomHeaderException(const std::string& msg)
            : msg(msg) {}

        virtual ~badRomHeaderException() noexcept {}

        virtual const char* what() const noexcept 
        { 
            return msg.c_str();
        }

};