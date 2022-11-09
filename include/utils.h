#ifndef APPLE_PIE_UTILS_H
#define APPLE_PIE_UTILS_H

#include "support/Any.h"
#include <string>

int stringToInt(const std::string &str) {
    int ret = 0;
    for (const auto &ch : str) {
        ret = ret * 10 + ch - '0';
    }
    return ret;
}

antlrcpp::Any stringToNum(const std::string &str) {
    int ret1 = 0, ret2 = 0;
    int div = 1;
    bool flag = 0;
    for (const auto &ch : str) {
        if (ch == '.') {
            flag = 1;
            continue;
        }
        if (!flag) {
            ret1 = ret1 * 10 + ch - '0';
        } else {
            ret2 = ret2 * 10 + ch - '0';
            div *= 10;
        }
    }
    if (flag)
        return (double)ret1 + ((double)ret2 / (double)div);
    return ret1;
}

    bool validateVarName(const std::string &str) {
    for (const auto &ch : str) {
        if (!(ch >= 'a' && ch <= 'z') && !(ch >= 'A' && ch <= 'Z')) {
            return false;
        }
    }
    return true;
}

#endif // APPLE_PIE_UTILS_H