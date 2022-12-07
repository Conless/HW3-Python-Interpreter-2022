#ifndef PYTHON_INTERPRETER_UTILS_H
#define PYTHON_INTERPRETER_UTILS_H

#include "antlr4-runtime.h"

antlrcpp::Any stringToNum(const std::string &str);

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