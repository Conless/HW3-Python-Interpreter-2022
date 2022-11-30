#ifndef PYTHON_INTERPRETER_UTILS_H
#define PYTHON_INTERPRETER_UTILS_H

#include <string>

#include "Python3BaseVisitor.h"
#include "int2048/int2048.h"
#include "pyinter/exception.h"

void JudgeInput(int argc, char *argv[]);

void OutputFunction(const char *s);

int stringToInt(const std::string &str);

namespace TypeConverter {

antlrcpp::Any stringToNum(const std::string &str);

double ToDouble(const antlrcpp::Any &num);

bool ToBool(const std::string &str);

bool validateVarName(const std::string &str);

}

antlrcpp::Any operator+(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value);
antlrcpp::Any operator-(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value);
antlrcpp::Any operator*(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value);
antlrcpp::Any operator/(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value);
antlrcpp::Any operator%(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value);
antlrcpp::Any divide(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value);

bool operator<(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value);
bool operator>(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value);
bool operator==(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value);
bool operator>=(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value);
bool operator<=(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value);
bool operator!=(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value);

std::ostream &operator<<(std::ostream &out, const antlrcpp::Any print_num);

#endif