#ifndef PYTHON_INTERPRETER_UTILS_H
#define PYTHON_INTERPRETER_UTILS_H

#include <string>

#include "Python3BaseVisitor.h"
#include "int2048/int2048.h"
#include "pyinter/exception.h"

typedef sjtu::int2048 ll;

namespace TypeConverter {
antlrcpp::Any stringToNum(const std::string &str);
ll toInt(const antlrcpp::Any &num);
std::string toString(const antlrcpp::Any &num);
bool toBool(const antlrcpp::Any &x);
double toDouble(const antlrcpp::Any &num);
} // namespace TypeConverter

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