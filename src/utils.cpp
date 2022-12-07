#include "pyinter/utils.h"

#include "pyinter/exception.h"
#include "pyinter/convert.h"

typedef sjtu::int2048 ll;

antlrcpp::Any operator+(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<std::string>() && right_value.is<std::string>())
        return left_value.as<std::string>() + right_value.as<std::string>();
    if (left_value.is<double>() || right_value.is<double>())
        return Convert<double>()(left_value) + Convert<double>()(right_value);
    return Convert<ll>()(left_value) + Convert<ll>()(right_value);
}

antlrcpp::Any operator-(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<double>() || right_value.is<double>())
        return Convert<double>()(left_value) - Convert<double>()(right_value);
    return Convert<ll>()(left_value) - Convert<ll>()(right_value);
}

antlrcpp::Any operator*(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<std::string>() && right_value.is<ll>()) {
        ll rpt_time = right_value.as<ll>();
        std::string rpt_data = left_value.as<std::string>(), rpt_res = "";
        for (ll i = 1; i <= rpt_time; i += 1)
            rpt_res += rpt_data;
        return rpt_res;
    }
    if (right_value.is<std::string>() && left_value.is<ll>()) {
        ll rpt_time = left_value.as<ll>();
        std::string rpt_data = right_value.as<std::string>(), rpt_res = "";
        for (ll i = 1; i <= rpt_time; i += 1)
            rpt_res += rpt_data;
        return rpt_res;
    }
    if (left_value.is<double>() || right_value.is<double>())
        return Convert<double>()(left_value) * Convert<double>()(right_value);
    return Convert<ll>()(left_value) * Convert<ll>()(right_value);
}
antlrcpp::Any operator/(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    return Convert<double>()(left_value) / Convert<double>()(right_value);
}
antlrcpp::Any operator%(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    return Convert<ll>()(left_value) % Convert<ll>()(right_value);
}
antlrcpp::Any divide(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<double>() || right_value.is<double>())
        return int(Convert<double>()(left_value) / Convert<double>()(right_value));
    return Convert<ll>()(left_value) / Convert<ll>()(right_value);
}

bool operator<(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.is<std::string>() && right_value.is<std::string>())
        return left_value.as<std::string>() < right_value.as<std::string>();
    if (left_value.is<double>() || right_value.is<double>())
        return Convert<double>()(left_value) < Convert<double>()(right_value);
    return Convert<ll>()(left_value) < Convert<ll>()(right_value);
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
        return left_value.as<bool>() == right_value.as<bool>();
    if (left_value.is<double>() || right_value.is<double>())
        return left_value.as<double>() == right_value.as<double>();
    return Convert<ll>()(left_value) == Convert<ll>()(right_value);
}
bool operator>=(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) { return !(left_value < right_value); }

bool operator<=(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) { return !(right_value < left_value); }

bool operator!=(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.isNull() || right_value.isNull()) {
        if (left_value.isNull() && right_value.isNull())
            return false;
        else
            return true;
    }
    return !(left_value == right_value);
}

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
    else if (print_num.is<std::vector<antlrcpp::Any>>()) {
        for (auto print_data : print_num.as<std::vector<antlrcpp::Any>>())
            out << print_data << " ";
    } else
        throw Exception("", UNIMPLEMENTED);
    return out;
}