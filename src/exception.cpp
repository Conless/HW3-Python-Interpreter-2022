#include "pyinter/exception.h"

Exception::Exception(const std::string &arg, ExceptionType type) {
    if (type == UNIMPLEMENTED)
        message = "Sorry, Python Interpreter do not implement this.";
    else if (type == UNDEFINED)
        message = "Undefined Variable: " + arg;
    else if (type == INVALID_FUNC_CALL)
        message = "Invalid function call: " + arg;
}
