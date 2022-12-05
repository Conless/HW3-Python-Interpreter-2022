#include "pyinter/utils.h"
#include "pyinter/exception.h"

namespace TypeConverter {

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

ll toInt(const antlrcpp::Any &num) {
    if (num.is<double>())
        return (ll)((int)num.as<double>());
    if (num.is<ll>())
        return num.as<ll>();
    if (num.is<std::string>())
        return ll(num.as<std::string>());
    if (num.is<bool>())
        return ll(num.as<bool>());
    if (num.isNull())
        return 0;
    throw Exception("", UNIMPLEMENTED);
}
std::string toString(const antlrcpp::Any &num) {
    if (num.is<ll>())
        return num.as<ll>()._TOSTRING();
    if (num.is<double>()) {
        char *s;
        sprintf(s, "%.6lf", num.as<double>());
        return std::string(s);
    }
    if (num.is<std::string>())
        return num.as<std::string>();
    if (num.is<bool>())
        return num.as<bool>() ? "True" : "False";
    if (num.isNull())
        return "None";
    throw Exception("", UNIMPLEMENTED);
}
bool toBool(const antlrcpp::Any &x) {
    if (x.is<std::string>()) {
        std::string str = x.as<std::string>();
        if (str != "")
            return true;
        else return false;
    }
    if (x.is<double>())
        return x.as<double>() != 0.0;
    if (x.is<bool>())
        return x.as<bool>();
    if (x.is<ll>())
        return bool(x.as<ll>() != 0);
    if (x.isNull())
        return false;
    throw Exception("", UNIMPLEMENTED);
}
double toDouble(const antlrcpp::Any &num) {
    if (num.is<ll>())
        return double(num.as<ll>());
    if (num.is<double>())
        return num.as<double>();
    if (num.is<std::string>())
        return stringToNum(num);
    if (num.is<bool>())
        return num.as<bool>() ? 1.0 : 0.0;
    if (num.isNull())
        return 0;
    throw Exception("", UNIMPLEMENTED);
}
} // namespace TypeConverter

antlrcpp::Any operator+(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.isNull() && right_value.isNull())
        return {};
    if (left_value.is<std::string>() && right_value.is<std::string>())
        return std::move(left_value.as<std::string>() + right_value.as<std::string>());
    if (left_value.is<double>() || right_value.is<double>())
        return std::move(TypeConverter::toDouble(left_value) + TypeConverter::toDouble(right_value));
    if (left_value.is<ll>() && right_value.is<ll>())
        return std::move(left_value.as<ll>() + right_value.as<ll>());
    if (left_value.is<std::string>() && right_value.is<ll>())
        return std::move(TypeConverter::stringToNum(left_value.as<std::string>()).as<ll>() + right_value.as<ll>());
    if (left_value.is<bool>() || right_value.is<bool>()) {
        if (left_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) + right_value;
        if (right_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) + right_value;
    }
    return TypeConverter::toInt(left_value) + TypeConverter::toInt(right_value);
}

antlrcpp::Any operator-(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.isNull() && right_value.isNull())
        return {};
    if (left_value.is<ll>() && right_value.is<ll>())
        return std::move(left_value.as<ll>() - right_value.as<ll>());
    if (left_value.is<double>() || right_value.is<double>())
        return TypeConverter::toDouble(left_value) - TypeConverter::toDouble(right_value);
    if (left_value.is<bool>() || right_value.is<bool>()) {
        if (left_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) - right_value;
        if (right_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) - right_value;
    }
    return TypeConverter::toInt(left_value) + TypeConverter::toInt(right_value);
}
antlrcpp::Any operator*(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.isNull() || right_value.isNull())
        return {};
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
        return TypeConverter::toDouble(left_value) * TypeConverter::toDouble(right_value);
    if (left_value.is<bool>() || right_value.is<bool>()) {
        if (left_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) * right_value;
        if (right_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) * right_value;
    }
    return TypeConverter::toInt(left_value) * TypeConverter::toInt(right_value);
}
antlrcpp::Any operator/(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) {
    if (left_value.isNull())
        return {};
    if (left_value.is<ll>() && right_value.is<ll>())
        return std::move(TypeConverter::toDouble(left_value.as<ll>()) / TypeConverter::toDouble(right_value.as<ll>()));

    if (left_value.is<double>() || right_value.is<double>())
        return TypeConverter::toDouble(left_value) / TypeConverter::toDouble(right_value);
    if (left_value.is<bool>() || right_value.is<bool>()) {
        if (left_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) / right_value;
        if (right_value.is<bool>())
            return (antlrcpp::Any)((ll)((int)left_value.as<bool>())) / right_value;
    }
    return TypeConverter::toInt(left_value) * TypeConverter::toInt(right_value);
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
        return int(TypeConverter::toDouble(left_value) / TypeConverter::toDouble(right_value));
    else
        throw Exception("", UNIMPLEMENTED);
}

bool operator<(const antlrcpp::Any &left_val, const antlrcpp::Any &right_val) {
    antlrcpp::Any left_value = left_val, right_value = right_val;
    if (left_value.isNull() && right_value.isNull())
        return false;
    if (left_value.isNull() || right_value.isNull()) {
        if (right_value.isNull())
            std::swap(left_value, right_value);
        if (right_value.is<ll>())
            left_value = ll(0);
        else if (right_value.is<std::string>())
            right_value = "";
        else if (right_value.is<double>())
            right_value = 0.0;
        else if (right_value.is<bool>())
            right_value = false;
        else while (true)
                ;
    }
    if (left_value.is<std::string>() && right_value.is<std::string>())
        return left_value.as<std::string>() < right_value.as<std::string>();
    else if (left_value.is<std::string>() || right_value.is<std::string>())
        return false;

    if (left_value.is<double>() || right_value.is<double>())
        return left_value.as<double>() < right_value.as<double>();

    if (left_value.is<ll>() && right_value.is<ll>())
        return left_value.as<ll>() < right_value.as<ll>();
    return false;
}

bool operator>(const antlrcpp::Any &left_value, const antlrcpp::Any &right_value) { return right_value < left_value; }

bool operator==(const antlrcpp::Any &left_val, const antlrcpp::Any &right_val) {
    antlrcpp::Any left_value = left_val, right_value = right_val;
    if (left_value.isNull() && right_value.isNull())
        return true;
    if (left_value.isNull() || right_value.isNull()) {
        if (right_value.isNull())
            std::swap(left_value, right_value);
        if (right_value.is<ll>())
            left_value = ll(0);
        else if (right_value.is<std::string>())
            right_value = "";
        else if (right_value.is<double>())
            right_value = 0.0;
        else if (right_value.is<bool>())
            right_value = false;
    }

    if (left_value.is<std::string>() && right_value.is<std::string>())
        return left_value.as<std::string>() == right_value.as<std::string>();
    else if (left_value.is<std::string>() || right_value.is<std::string>())
        return false;

    if (left_value.is<bool>() && right_value.is<bool>())
        return left_value.as<bool>() == right_value.as<bool>();

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
    else if (print_num.is<std::vector<antlrcpp::Any>>()) {
        for (auto print_data : print_num.as<std::vector<antlrcpp::Any>>())
            out << print_data << " ";
    } else
        throw Exception("", UNIMPLEMENTED);
    return out;
}