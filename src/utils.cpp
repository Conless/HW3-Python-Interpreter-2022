#include "pyinter/utils.h"
#include "pyinter/exception.h"

int show_status;

typedef sjtu::int2048 ll;

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

std::string toString(antlrcpp::Any &num) {
    if (num.is<ll>())
        return num.as<ll>()._TOSTRING();
    if (num.is<double>()) {
        char *s;
        sprintf(s, "%.6lf", num.as<double>());
        return std::string(s);
    }
    throw Exception("", UNIMPLEMENTED);
}

ll stringToInt(const std::string &str) {
    ll ret = 0;
    for (const auto &ch : str) {
        ret = ret * 10 + ch - '0';
    }
    return ret;
}

antlrcpp::Any stringToNum(const std::string &str) {
    bool flag = 0;
    for (const auto &ch : str) {
        if (ch == '.') {
            flag = 1;
            break;
        }
    }
    if (!flag) {
        ll ret = str;
        return ret;
    } else {
        flag = 0;
        double x1 = 0, x2 = 0, div = 1;
        for (const auto &ch : str) {
            if (ch == '.') {
                flag = 1;
                continue;
            }
            if (!flag) {
                x1 = x1 * 10 + ch - '0';
            } else {
                x2 = x2 * 10 + ch - '0';
                div *= 10;
            }
        }
        return x1 + x2 / div;
    }
}

bool ToBool(const std::string &str) {
    if (str == "True")
        return true;
    else if (str == "False")
        return false;
    else
        throw Exception("", UNIMPLEMENTED);
}
bool toBool(const antlrcpp::Any &x) {
    if (x.is<bool>())
        return x.as<bool>();
    if (x.is<ll>())
        return bool(x.as<ll>() != 0);
    if (x.is<std::pair<ll, antlrcpp::Any>>())
        return toBool(x.as<std::pair<ll, antlrcpp::Any>>().second);
    throw Exception("", UNIMPLEMENTED);
}

double ToDouble(const antlrcpp::Any &num) {
    if (num.is<ll>())
        return double(num.as<ll>());
    if (num.is<double>())
        return num.as<double>();
    if (num.is<std::string>())
        return stringToNum(num);
    throw Exception("", UNIMPLEMENTED);
}

bool validateVarName(const std::string &str) {
    for (const auto &ch : str) {
        if (!(ch >= 'a' && ch <= 'z') && !(ch >= 'A' && ch <= 'Z')) {
            return false;
        }
    }
    return true;
}
} // namespace TypeConverter

antlrcpp::Any operator+(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<std::string>() && right_value.is<std::string>())
        return std::move(left_value.as<std::string>() + right_value.as<std::string>());
    // TODO
    if (left_value.is<double>() || right_value.is<double>())
        return std::move(TypeConverter::ToDouble(left_value) + TypeConverter::ToDouble(right_value));
    if (left_value.is<ll>() && right_value.is<ll>())
        return std::move(left_value.as<ll>() + right_value.as<ll>());
    if (left_value.is<std::string>() && right_value.is<ll>())
        return std::move(TypeConverter::stringToInt(left_value.as<std::string>()) + right_value.as<ll>());
    else {
        if (left_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) + right_value;
        if (right_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) + right_value;
        throw Exception("", UNIMPLEMENTED);
    }
}

antlrcpp::Any operator-(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<ll>() && right_value.is<ll>())
        return std::move(left_value.as<ll>() - right_value.as<ll>());
    if (left_value.is<double>() || right_value.is<double>())
        return TypeConverter::ToDouble(left_value) - TypeConverter::ToDouble(right_value);
    else {
        if (left_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) - right_value;
        if (right_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) - right_value;
        throw Exception("", UNIMPLEMENTED);
    }
}
antlrcpp::Any operator*(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<std::string>() && right_value.is<ll>()) {
        ll rpt_time = right_value.as<ll>();
        std::string rpt_data = left_value.as<std::string>(), rpt_res = "";
        for (ll i = 1; i <= rpt_time; i += 1)
            rpt_res += rpt_data;
        return std::move(rpt_res);
    }
    if (right_value.is<std::string>() && left_value.is<ll>()) {
        ll rpt_time = left_value.as<ll>();
        std::string rpt_data = right_value.as<std::string>(), rpt_res = "";
        for (ll i = 1; i <= rpt_time; i += 1)
            rpt_res += rpt_data;
        return std::move(rpt_res);
    }
    if (left_value.is<ll>() && right_value.is<ll>())
        return std::move(left_value.as<ll>() * right_value.as<ll>());

    if (left_value.is<double>() || right_value.is<double>())
        return TypeConverter::ToDouble(left_value) * TypeConverter::ToDouble(right_value);
    else {
        if (left_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) * right_value;
        if (right_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) * right_value;
        throw Exception("", UNIMPLEMENTED);
    }
}
antlrcpp::Any operator/(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    // TODO
    if (left_value.is<ll>() && right_value.is<ll>())
        return std::move(left_value.as<ll>() / right_value.as<ll>());

    if (left_value.is<double>() || right_value.is<double>())
        return TypeConverter::ToDouble(left_value) / TypeConverter::ToDouble(right_value);
    else {
        if (left_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) / right_value;
        if (right_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) / right_value;
        throw Exception("", UNIMPLEMENTED);
    }
}
antlrcpp::Any operator%(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<ll>() && right_value.is<ll>())
        return std::move(left_value.as<ll>() % right_value.as<ll>());
    else
        throw Exception("", UNIMPLEMENTED);
}
antlrcpp::Any divide(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<ll>() && right_value.is<ll>())
        return (ll)(left_value.as<ll>() / right_value.as<ll>());
    if (left_value.is<double>() || right_value.is<double>())
        return int(TypeConverter::ToDouble(left_value) / TypeConverter::ToDouble(right_value));
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

    if (left_value.is<ll>() && right_value.is<ll>())
        return left_value.as<ll>() < right_value.as<ll>();

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

    if (left_value.is<ll>() && right_value.is<ll>())
        return left_value.as<ll>() == right_value.as<ll>();
    throw Exception("", UNIMPLEMENTED);
}
bool operator>=(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) { return !(left_value < right_value); }

bool operator<=(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) { return !(right_value < left_value); }

bool operator!=(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) { return !(left_value == right_value); }

std::ostream &operator<<(std::ostream &out, const antlrcpp::Any print_num) {
    if (print_num.is<std::string>())
        out << print_num.as<std::string>();
    else if (print_num.is<double>())
        out << std::fixed << std::setprecision(6) << print_num.as<double>();
    else if (print_num.is<ll>())
        out << print_num.as<ll>();
    else if (print_num.is<bool>())
        out << (print_num.as<bool>() ? "True" : "False");
    else if (print_num.isNull())
        out << "None";
    else
        throw Exception("", UNIMPLEMENTED);
    return out;
}