#ifndef PYTHON_INTERPRETER_H
#define PYTHON_INTERPRETER_H

#include <string>

enum ExceptionType { UNDEFINED, UNIMPLEMENTED, INVALID_VARNAME, INVALID_FUNC_CALL };

class Exception {

  private:
    std::string message;

  public:
    Exception(const std::string &arg, ExceptionType type);
    std::string what() { return message; }
};

Exception::Exception(const std::string &arg, ExceptionType type) {
    if (type == UNIMPLEMENTED)
        message = "Sorry, Python Interpreter do not implement this.";
    else if (type == UNDEFINED)
        message = "Undefined Variable: " + arg;
    else if (type == INVALID_FUNC_CALL)
        message = "Invalid function call: " + arg;
}

#endif
