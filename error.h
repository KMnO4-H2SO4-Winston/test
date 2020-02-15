#ifndef SOLVEEQUATIONS_ERROR_H
#define SOLVEEQUATIONS_ERROR_H
#include <exception>

class InputError : public std::exception {
public:
    InputError(const char* msg) : _msg(msg) {
        _msg = "InputError: " + _msg;
    }
    const char* what() const throw() {
        return _msg.c_str();
    }
private:
    std::string _msg;
};

class FormatError : public std::exception {
public:
    FormatError(const char* msg) : _msg(msg) {
        _msg = "FormatError: " + _msg;
    }
    const char* what() const throw() {
        return _msg.c_str();
    }
private:
    std::string _msg;
};

#endif //SOLVEEQUATIONS_ERROR_H
