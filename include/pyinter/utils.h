#ifndef APPLE_PIE_UTILS_H
#define APPLE_PIE_UTILS_H

#include "support/Any.h"
#include <string>

int stringToInt(const std::string &str);

antlrcpp::Any stringToNum(const std::string &str);

bool validateVarName(const std::string &str);

#endif // APPLE_PIE_UTILS_H