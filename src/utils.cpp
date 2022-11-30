#include "pyinter/utils.h"
#include "pyinter/exception.h"

int show_status;

void JudgeInput(int argc, char *argv[]) {
    if (argc >= 2) {
        if (!strcmp(argv[1], "--show-status=0"))
            show_status = 0;
        else if (!strcmp(argv[1], "--show-status=1"))
            show_status = 1;
    }
}

void OutputFunction(const char *s) {
    if (show_status)
        printf("Into function %s\n", s);
    return;
}

namespace TypeConverter {

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

bool ToBool(const std::string &str) {
    if (str == "True")
        return true;
    else if (str == "False")
        return false;
    else
        throw Exception("", UNIMPLEMENTED);
}

double ToDouble(const antlrcpp::Any &num) {
    if (num.is<int>())
        return static_cast<double>(num.as<int>());
    return num.as<double>();
}

} // namespace TypeConverter

bool validateVarName(const std::string &str) {
    for (const auto &ch : str) {
        if (!(ch >= 'a' && ch <= 'z') && !(ch >= 'A' && ch <= 'Z')) {
            return false;
        }
    }
    return true;
}

antlrcpp::Any operator+(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<std::string>() && right_value.is<std::string>())
        return std::move(left_value.as<std::string>() + right_value.as<std::string>());
    if (left_value.is<double>() || right_value.is<double>())
        return std::move(TypeConverter::ToDouble(left_value) + TypeConverter::ToDouble(right_value));
    if (left_value.is<int>() && right_value.is<int>())
        return std::move(left_value.as<int>() + right_value.as<int>());
    if (left_value.is<std::string>() && right_value.is<int>())
        return std::move(TypeConverter::stringToInt(left_value.as<std::string>()) + right_value.as<int>());
    throw Exception("", UNIMPLEMENTED);
}

antlrcpp::Any operator-(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<int>() && right_value.is<int>())
        return std::move(left_value.as<int>() - right_value.as<int>());
    else
        throw Exception("", UNIMPLEMENTED);
}
antlrcpp::Any operator*(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<std::string>() && right_value.is<int>()) {
        int rpt_time = right_value.as<int>();
        std::string rpt_data = left_value.as<std::string>(), rpt_res = "";
        while (rpt_time--)
            rpt_res += rpt_data;
        return std::move(rpt_res);
    }
    if (left_value.is<int>() && right_value.is<int>())
        return std::move(left_value.as<int>() * right_value.as<int>());
    else
        throw Exception("", UNIMPLEMENTED);
}
antlrcpp::Any operator/(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<int>() && right_value.is<int>())
        return std::move((double)left_value.as<int>() / (double)right_value.as<int>());
    else
        throw Exception("", UNIMPLEMENTED);
}
antlrcpp::Any operator%(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<int>() && right_value.is<int>())
        return std::move(left_value.as<int>() % right_value.as<int>());
    else
        throw Exception("", UNIMPLEMENTED);
}
antlrcpp::Any divide(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<int>() && right_value.is<int>())
        return left_value.as<int>() && right_value.as<int>();
    else
        throw Exception("", UNIMPLEMENTED);
}

bool operator<(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<std::string>() && right_value.is<std::string>())
        return left_value.as<std::string>() < right_value.as<std::string>();
    else if (left_value.is<std::string>() || right_value.is<std::string>())
        return false;

    if (left_value.is<double>() || right_value.is<double>())
        return left_value.as<double>() < right_value.as<double>();

    if (left_value.is<int>() && right_value.is<int>())
        return left_value.as<int>() < right_value.as<int>();

    throw Exception("", UNIMPLEMENTED);
}

bool operator>(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) { return right_value < left_value; }

bool operator==(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.isNull() && right_value.isNull())
        return true;
    else if (left_value.isNull() || right_value.isNull())
        return false;

    if (left_value.is<std::string>() && right_value.is<std::string>())
        return left_value.as<std::string>() == right_value.as<std::string>();
    else if (left_value.is<std::string>() || right_value.is<std::string>())
        return false;

    if (left_value.is<bool>() && right_value.is<bool>())
        return TypeConverter::ToBool(left_value) == TypeConverter::ToBool(right_value);

    if (left_value.is<double>() || right_value.is<double>())
        return left_value.as<double>() == right_value.as<double>();

    return left_value.as<int>() == right_value.as<int>();
}
bool operator>=(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) { return !(left_value < right_value); }

bool operator<=(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) { return !(right_value < left_value); }

bool operator!=(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) { return !(left_value == right_value); }

std::ostream &operator<<(std::ostream &out, const antlrcpp::Any print_num) {
    if (print_num.is<std::string>())
        out << print_num.as<std::string>();
    else if (print_num.is<double>())
        out << std::fixed << std::setprecision(6) << print_num.as<double>();
    else if (print_num.is<int>())
        out << print_num.as<int>();
    else if (print_num.is<bool>())
        out << (print_num.as<bool>() ? "True" : "False");
    else if (print_num.isNull())
        out << "None";
    else
        throw Exception("", UNIMPLEMENTED);
    return out;
}