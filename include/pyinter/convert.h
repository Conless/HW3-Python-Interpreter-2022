#include "antlr4-runtime.h"
#include "int2048/int2048.h"

#include "pyinter/exception.h"

typedef sjtu::int2048 ll;

template <class T> class Convert {
  public:
    T operator()(const antlrcpp::Any &num);
};
template <> class Convert<ll> {
  public:
    ll operator()(const antlrcpp::Any &num) {
        if (num.is<ll>())
            return num.as<ll>();
        if (num.is<bool>())
            return ll(num.as<bool>());
        if (num.is<double>())
            return (ll)((int)num.as<double>());
        if (num.is<std::string>())
            return ll(num.as<std::string>());
        if (num.isNull())
            return 0;
        throw Exception("", UNIMPLEMENTED);
    }
};
template <> class Convert<double> {
  public:
    double operator()(const antlrcpp::Any &num) {
        if (num.is<ll>())
            return double(num.as<ll>());
        if (num.is<double>())
            return num.as<double>();
        if (num.is<std::string>()) {
            std::string str = num.as<std::string>();
            int flag = 0, sgn = 1;
            double x1 = 0, x2 = 0, div = 1;
            for (const auto &ch : str) {
                if (ch == '-') {
                    sgn = -sgn;
                    continue;
                }
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
        if (num.is<bool>())
            return num.as<bool>() ? 1.0 : 0.0;
        if (num.isNull())
            return 0;
        throw Exception("", UNIMPLEMENTED);
    }
};
template <> class Convert<bool> {
  public:
    bool operator()(const antlrcpp::Any &num) {
        if (num.is<std::string>()) {
            std::string str = num.as<std::string>();
            if (str != "")
                return true;
            else
                return false;
        }
        if (num.is<double>())
            return num.as<double>() != 0.0;
        if (num.is<bool>())
            return num.as<bool>();
        if (num.is<ll>())
            return bool(num.as<ll>() != 0);
        if (num.isNull())
            return false;
        throw Exception("", UNIMPLEMENTED);
    }
};

template <> class Convert<std::string> {
  public:
    std::string operator()(const antlrcpp::Any &num) {
        if (num.is<ll>())
            return num.as<ll>()._TOSTRING();
        if (num.is<double>()) {
            char s[100];
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
};