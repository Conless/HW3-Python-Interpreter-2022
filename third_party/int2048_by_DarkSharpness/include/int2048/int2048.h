#ifndef FUCK_YOU_PY
#define FUCK_YOU_PY

#define NUMBER_TYPE 1
#include "complex.h"
#include "vector.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace sjtu {

/**
 * You can access that at
 * https://github.com/lzyrapx/Competitive-Programming-Docs/.
 *
 *
 * @return log2(x) in 32-bit.
 */
inline unsigned int LOG2(unsigned x) {
    unsigned ret;
    __asm__ __volatile__("bsrl %1, %%eax" : "=a"(ret) : "m"(x));
    return ret;
}

class int2048 : public array<uint64_t> {
  public:
    static constexpr uint64_t DIV_minLen = 8;
    static constexpr int32_t judgeMap[2] = {1, -1};
    static constexpr uint64_t MULT_minLen = 12;
    static constexpr uint64_t unit[20] = {1,
                                          10,
                                          100,
                                          1000,
                                          10000,
                                          100000,
                                          1000000,
                                          10000000,
                                          100000000,
                                          1000000000,
                                          10000000000,
                                          100000000000,
                                          1000000000000,
                                          10000000000000,
                                          100000000000000,
                                          1000000000000000,
                                          10000000000000000,
                                          100000000000000000,
                                          1000000000000000000,
                                          10000000000000000000ULL};
    static constexpr uint64_t initLen = 2;
    static constexpr uint64_t baseBit = 3;
    static constexpr uint64_t base = 1e3;
    // The maximum 01bits for NTT multiplication
    static constexpr uint64_t FFT_maxBit = 20;
    // The maximum length for NTT multiplication
    static constexpr uint64_t FFT_maxLen = 1 << FFT_maxBit;
    // Unit roots for FFT
    static complex root[FFT_maxBit];
    static std::string numMap[base];

    /* Get the reverse vector.(With small optimization) */
    static void getRev(array<size_t> &rev, size_t len) {
        static size_t last = -1;
        if (len == last)
            return;
        rev.resize(len);
        rev[0] = 0;
        for (size_t i = 1; i < len; ++i) {
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) * len >> 1);
        }
    }

    static inline void FFT_init() {
        for (size_t i = 0; i < FFT_maxBit; ++i) {
            root[i].set(PI / double(1ULL << i));
        }
        buffer.resize(3);
        for (size_t i = 0; i < base; ++i) {
            size_t ret = i;
            for (size_t j = baseBit - 1; ~j; --j) {
                buffer[j] = (ret % 10) ^ '0';
                ret /= 10;
            }
            numMap[i] = buffer;
        }
    }

    static inline uint64_t Round(double num) { return num + 0.5; }

    static void FFT(complex *A, size_t len, bool opt = 0) {
        size_t cnt = 0;
        for (size_t i = 1; i < len; i <<= 1) {
            complex wn = root[cnt++]; // unit root
            if (opt)
                conjugate(wn);
            for (size_t j = 0; j < len; j += (i << 1)) {
                complex w(1.0, 0.0);
                for (size_t k = 0; k < i; ++k, w *= wn) {
                    complex x = A[j | k];
                    complex y = A[j | k | i] * w;
                    A[j | k] = x + y;
                    A[j | k | i] = x - y;
                }
            }
        }
    }
    static void IFFT(complex *A, size_t len) { return FFT(A, len, 1); }
    static void Reverse(complex *A, size_t *rev, size_t len) {
        for (size_t i = 0; i < len; ++i) {
            if (i < rev[i])
                std::swap(A[i], A[rev[i]]);
        }
    }
    /* Multiplication based on Fast Fourier Transformation. */
    friend int2048 &Mult_FT(int2048 &X, const int2048 &Y) {
        X.sign ^= Y.sign;

        size_t maxLen = X.size() + Y.size();
        size_t len = 1 << (LOG2(maxLen - 1) + 1);

        static array<complex> A;
        A.resize(len);

        { // Make out A and avoid too much logical judgement.
            size_t i = 0;
            if (X.size() > Y.size()) {
                while (i < Y.size()) {
                    A[i].real = X[i], A[i].imag = Y[i];
                    ++i;
                }
                while (i < X.size()) {
                    A[i].real = X[i], A[i].imag = 0;
                    ++i;
                }
            } else { // Y.size() >= X.size()
                while (i < X.size()) {
                    A[i].real = X[i], A[i].imag = Y[i];
                    ++i;
                }
                while (i < Y.size()) {
                    A[i].imag = Y[i], A[i].real = 0;
                    ++i;
                }
            }
            while (i < len) {
                A[i].real = A[i].imag = 0;
                ++i;
            }
        }

        static array<size_t> rev;
        getRev(rev, len);

        Reverse(A.begin(), rev.begin(), len);
        FFT(A.begin(), len);

        for (size_t i = 0; i < len; ++i) {
            A[i] *= A[i];
        }

        Reverse(A.begin(), rev.begin(), len);
        IFFT(A.begin(), len);

        X.resize(maxLen);
        uint64_t ret = 0;

        len <<= 1;
        for (size_t i = 0; i < maxLen; ++i) {
            ret += Round(A[i].imag / double(len));
            X[i] = ret % base;
            ret /= base;
        }
        while (!X.back())
            X.pop_back();
        return X;
    }
    static std::string buffer; // buffer inside
    bool sign;

    /**
     * @brief Initialize from a string.
     *
     * @param str The string to initialize from.
     */
    void read(const std::string &str) {
        if (str.front() == '-')
            sign = true;
        else
            sign = false;
        clear(); // clear the previous data.
        uint64_t j = sign;
        while (str[j] == '0')
            ++j;       // clear prefix-0
        if (!str[j]) { // 0 case.
            sign = false;
            emplace_back(0);
            return;
        }
        uint64_t i = str.find('.');
        if (i == std::string::npos)
            i = str.size();
        reserve(1 + (i - j) / baseBit);
        uint64_t cnt = 0;
        uint64_t ret = 0;
        while (i-- != j) { // Loop in [sign,str.size())
            ret += unit[cnt] * (str[i] ^ '0');
            if (++cnt == baseBit) {
                emplace_back(ret);
                ret = cnt = 0;
            }
        }
        if (cnt)
            emplace_back(ret);
    }

    /* Print out inside number(Default by std::cout)*/
    void print(std::ostream &os = std::cout) const { os << (buffer = _TOSTRING()); }

    /* Instream input for an int2048 object. */
    friend std::istream &operator>>(std::istream &is, int2048 &dst) {
        is >> buffer;
        dst.resize(0);
        dst.read(buffer);
        return is;
    }

    /* Outstream output for an int2048 object. */
    friend std::ostream &operator<<(std::ostream &os, const int2048 &src) {
        src.print(os);
        return os;
    }

    /* Swap data with another int2048. */
    int2048 &operator=(int2048 &&tmp) {
        swap(tmp);
        sign = tmp.sign;
        return *this;
    }

    /* Copy data from another int2048. */
    int2048 &operator=(const int2048 &tmp) {
        copy(tmp);
        sign = tmp.sign;
        return *this;
    }

    /**
     * @brief Compare the absolute value of X and Y.
     *
     * @return -1 if abs(X) < abs(Y) ||
     *          0 if abs(X) = abs(Y) ||
     *          1 if abs(X) > abs(Y)
     */
    friend int32_t Abs_Compare(const int2048 &X, const int2048 &Y) {
        if (X.size() != Y.size()) {
            return judgeMap[X.size() < Y.size()];
        }

        // X.size() == Y.size()
        for (size_t i = X.size() - 1; i != -1ULL; --i) {
            if (X[i] != Y[i]) {
                return judgeMap[X[i] < Y[i]];
            }
        }

        // abs(X) == abs(Y)
        return 0;
    }

    /* Compare X and Y*/
    friend inline bool operator<(const int2048 &X, const int2048 &Y) {
        if (X.sign != Y.sign)
            return X.sign > Y.sign;
        else
            return Abs_Compare(Y, X) == judgeMap[X.sign];
    }
    /* Compare X and Y*/
    friend inline bool operator>(const int2048 &X, const int2048 &Y) {
        if (X.sign != Y.sign)
            return X.sign < Y.sign;
        else
            return Abs_Compare(X, Y) == judgeMap[X.sign];
    }
    /* Compare X and Y*/
    friend inline bool operator<=(const int2048 &X, const int2048 &Y) {
        if (X.sign != Y.sign)
            return X.sign > Y.sign;
        else
            return Abs_Compare(X, Y) != judgeMap[X.sign];
    }
    /* Compare X and Y*/
    friend inline bool operator>=(const int2048 &X, const int2048 &Y) {
        if (X.sign != Y.sign)
            return X.sign < Y.sign;
        else
            return Abs_Compare(Y, X) != judgeMap[X.sign];
    }
    /* Compare X and Y*/
    friend inline bool operator==(const int2048 &X, const int2048 &Y) {
        if (X.sign != Y.sign || X.size() != Y.size())
            return false;
        return !Abs_Compare(X, Y);
    }

    /* Compare X and Y*/
    friend inline bool operator!=(const int2048 &X, const int2048 &Y) {
        if (X.sign != Y.sign || X.size() != Y.size())
            return true;
        return Abs_Compare(X, Y);
    }

    /* Compare X and Y*/
    friend inline bool operator!(const int2048 &X) { return !X.back(); }

    friend int2048 operator<<(const int2048 &X, const int64_t Y) {
        int2048 ans = int2048(array<uint64_t>(Y, X), X.sign);
        if (ans.empty())
            return 0;
        else
            return ans;
    }

    friend int2048 operator>>(const int2048 &X, const int64_t Y) {
        int2048 ans = int2048(array<uint64_t>(-Y, X), X.sign);
        if (ans.empty())
            return 0;
        else
            return ans;
    }

    /* Add X's abs value by 1. */
    friend int2048 &SelfAdd(int2048 &X) {
        for (size_t i = 0; i != X.size(); ++i) {
            if (++X[i] < base)
                return X;
            X[i] = 0;
        }
        X.push_back(1);
        return X;
    }

    /* Subtract X's abs value by 1.
       Note that X != 0 */
    friend int2048 &SelfSub(int2048 &X) {
        for (size_t i = 0; i != X.size(); ++i) {
            if (--X[i] < base)
                break;
            X[i] = base;
        }
        while (!X.back())
            X.pop_back();
        return X;
    }

    /* X = X + 1 */
    friend int2048 &operator++(int2048 &X) {
        if (X.back() == 1 && X.size() == 1 && X.sign) { // -1 case
            X[0] = X.sign = false;
            return X;
        }
        return X.sign ? SelfSub(X) : SelfAdd(X);
    }

    /* X = X - 1*/
    friend int2048 &operator--(int2048 &X) {
        if (!X.back()) {
            X[0] = X.sign = 1;
            return X;
        } else if (X.back() == 1 && X.size() == 1 && !X.sign) {
            X[0] = X.sign = false;
            return X;
        } else
            return X.sign ? SelfAdd(X) : SelfSub(X);
    }

    /* Reverse a number's sign.It will return itself.
    Please use it instead of X *= (-1) or X = -X  */
    inline int2048 &reverse() {
        sign ^= 1;
        return *this;
    }

    inline int2048 &setSign(bool flag) {
        sign = flag;
        return *this;
    }

    friend inline int2048 abs(int2048 &&tmp) {
        tmp.sign = false;
        return tmp;
    }

    /* X * (-1) */
    friend inline int2048 operator-(int2048 &&X) { return X.reverse(); }

    /* X * (-1) */
    friend inline int2048 operator-(const int2048 &X) {
        int2048 ans(X);
        return ans.reverse();
    }

    /**
     * @brief Add abs(X) by abs(Y) from X's front bit.
     * Make Sure X.size() >= Y.size()
     *
     * @return X = sgn(X) * (abs(X) + abs(Y))
     */
    friend int2048 &Add(int2048 &X, const int2048 &Y) {
        bool ret = 0;
        size_t i = 0;
        for (/* i = 0 */; i != Y.size(); ++i) {
            X[i] += Y[i] + ret;
            ret = X[i] >= base;
            if (ret)
                X[i] -= base;
        }
        if (ret) {
            while (i != X.size()) {
                if (X[i] != base - 1) {
                    ++X[i];
                    return X;
                } else {
                    X[i++] = 0;
                }
            }
            X.push_back(1);
        }
        return X;
    }

    /**
     * @brief Subtract abs(Y) from abs(X) from X's front bit.
     * Make Sure abs(X) > abs(Y)
     *
     * @return X = sgn(x) * (abs(X) - abs(Y))
     */
    friend int2048 &Sub(int2048 &X, const int2048 &Y) {
        bool ret = 0;
        size_t i = 0;
        for (/* i = 0 */; i != Y.size(); ++i) {
            X[i] -= Y[i] + ret;
            ret = X[i] >> 63;
            if (ret)
                X[i] += base;
        }
        if (ret) {
            while (i != X.size()) {
                if (X[i]) {
                    --X[i];
                    break;
                } else {
                    X[i++] = base - 1;
                }
            }
        }
        while (!X.back())
            X.pop_back();
        return X;
    }

    /**
     * @brief Subtract abs(Y) from abs(X) from X's front bit.
     * Make Sure abs(X) < abs(Y)
     *
     * @return X = sgn(x) * (abs(Y) - abs(X))
     */
    friend int2048 &Sub(const int2048 &Y, int2048 &X) {
        X.expand_back(Y.size() - X.size());
        bool ret = 0;
        size_t i = 0;
        for (/* i = 0 */; i != X.size(); ++i) {
            X[i] = Y[i] - X[i] - ret;
            ret = X[i] >> 63;
            if (ret)
                X[i] += base;
        }
        if (ret) {
            while (i != X.size()) {
                if (Y[i]) {
                    X[i] = Y[i] - 1;
                    break;
                } else {
                    X[i++] = base - 1;
                }
            }
        }
        while (!X.back())
            X.pop_back();
        return X;
    }

    friend int2048 &Mult_Lo(int2048 &X, int64_t Y) {
        uint64_t ret = 0;
        for (size_t i = 0; i < X.size(); ++i) {
            ret += X[i] * Y;
            if (ret >= base) {
                X[i] = ret % base;
                ret /= base;
            } else {
                X[i] = ret;
                ret = 0;
            }
        }
        if (ret)
            X.push_back(ret);
        return X;
    }

    /* X = X + Y */
    friend int2048 &operator+=(int2048 &X, const int2048 &Y) {
        if (X.sign == Y.sign) {
            if (X.size() < Y.size()) {
                X.expand_back(Y.size() - X.size() + 1);
                X.pop_back();
            }
            return Add(X, Y);
        } else {
            int32_t cmp = Abs_Compare(X, Y);
            if (!cmp)
                return X = 0;
            else if (cmp == -1) {
                return Sub(Y, X).reverse();
            } else { /*cmp == 1*/
                return Sub(X, Y);
            }
        }
    }

    /* X = X - Y */
    friend int2048 &operator-=(int2048 &X, const int2048 &Y) {
        if (X.sign != Y.sign) {
            if (X.size() < Y.size()) {
                X.expand_back(Y.size() - X.size() + 1);
                X.pop_back();
            }
            return Add(X, Y);
        } else {
            int32_t cmp = Abs_Compare(X, Y);
            if (!cmp)
                return X = 0;
            else if (cmp == -1) {
                return Sub(Y, X).reverse();
            } else { /*cmp == 1*/
                return Sub(X, Y);
            }
        }
    }

    /* X + Y */
    friend int2048 operator+(const int2048 &X, const int2048 &Y) {
        if (X.size() < Y.size()) {
            int2048 ans(Y);
            return ans += X;
        } else {
            int2048 ans(X);
            return ans += Y;
        }
    }

    /* X - Y*/
    friend int2048 operator-(const int2048 &X, const int2048 &Y) {
        if (X.size() < Y.size()) {
            int2048 ans(Y);
            ans.reverse();
            return ans += X;
        } else {
            int2048 ans(X);
            return ans -= Y;
        }
    }

    friend int2048 &operator*=(int2048 &X, const int2048 &Y) {
        if (!X)
            return X;
        if (!Y)
            return X = 0;
        // TODO : Brute Force Mult
        return Mult_FT(X, Y);
    }

    friend int2048 operator*(const int2048 &X, const int2048 &Y) {
        // TODO : Brute Force Mult
        if (!X || !Y)
            return 0;
        int2048 ans = int2048(X, X.size() + Y.size());
        return Mult_FT(ans, Y);
    }

    friend int2048 operator/(const int2048 &X, const int2048 &Y) {
        int32_t cmp = Abs_Compare(X, Y);
        if (cmp == -1)
            return X.sign ^ Y.sign ? -1 : 0;
        if (cmp == 0)
            return X.sign ^ Y.sign ? -1 : 1;
        uint64_t dif = X.size() - Y.size() * 2;
        if (int64_t(dif) < 0)
            dif = 0;

        // Y.size() + dif is the new length of Y.
        int2048 ans = ((X << dif) * ~(Y << dif)) >> (2 * (dif + Y.size()));
        ans.sign = false;
        int2048 ret = (ans + 1) * Y;
        while (Abs_Compare(ret, X) != 1) {
            // tmp = (ans + 1) * Y
            SelfAdd(ans);
            ret += Y;
        }
        ret -= Y;
        while ((cmp = Abs_Compare(ret, X)) == 1) { // Ans *
            // tmp = ans * Y
            SelfSub(ans);
            ret -= Y;
        }
        ans.sign = X.sign ^ Y.sign;
        if (!cmp || !ans.sign)
            return ans;
        else
            return SelfAdd(ans);
    }

    friend int2048 &operator/=(int2048 &X, const int2048 &Y) { return X = X / Y; }

    friend int2048 operator%(const int2048 &X, const int2048 &Y) {
        int32_t cmp = Abs_Compare(X, Y);
        if (cmp == -1)
            return X.sign ^ Y.sign ? X + Y : X;
        if (cmp == 0)
            return 0;

        uint64_t dif = X.size() - Y.size() * 2;
        if (int64_t(dif) < 0)
            dif = 0;

        // Y.size() + dif is the new length of Y.
        int2048 ans = ((X << dif) * ~(Y << dif)) >> (2 * (dif + Y.size()));
        ans.sign = false;
        int2048 ret = (ans + 1) * Y;
        while (Abs_Compare(ret, X) != 1) {
            // tmp = (ans + 1) * Y
            SelfAdd(ans);
            ret += Y;
        }
        ret -= Y;
        while ((cmp = Abs_Compare(ret, X)) == 1) { // Ans *
            // tmp = ans * Y
            SelfSub(ans);
            ret -= Y;
        }
        ret.sign = X.sign;
        if (!cmp)
            return 0;
        return X.sign ^ Y.sign ? X - ret + Y : X - ret;
    }

    /* The reverser of Y in twice the size() of Y.
    In other words , 1 / Y =  (~Y) / base ^ (2 * Y.size()) */
    friend int2048 operator~(const int2048 &X) {
        if (X.size() == 1) {
            int2048 ans;
            ans.pop_back();
            uint64_t i = base * base / X[0];
            while (i) {
                ans.push_back(i % base);
                i /= base;
            }
            return ans;
        } else if (X.size() == 2) {
            int2048 ans;
            ans.pop_back();
            constexpr uint64_t N = base * base * base * base;
            uint64_t i = N / (X[0] + X[1] * base);
            while (i) {
                ans.push_back(i % base);
                i /= base;
            }
            return ans;
        }

        size_t hf = 1 + (X.size() >> 1);     // half of X.size()
        int2048 Y = ~(X >> (X.size() - hf)); // First half bits reverse
        int2048 tmp = Y << (X.size() - hf);
        Mult_Lo(tmp, 2);
        tmp.sign = false;
        // Newton's method Y1 = Y0 * (2 - X * Y0).
        // When multiply Y0 ,it should be Y0 >> (hf << 1)
        return tmp - abs(X * Y * Y >> (hf << 1));
    }

    /* Initialize from a long long object.(Default = 0)*/
    int2048(intmax_t tmp = 0) {
        if (!tmp) { // tmp = 0
            sign = false;
            emplace_back(0);
            return;
        } else {
            reserve(initLen);
            if (tmp < 0) {
                sign = true;
                tmp = -tmp;
            } else { // tmp > 0
                sign = false;
            }
            while (tmp) {
                push_back(tmp % base);
                tmp /= base;
            }
        }
    }

    /* Move construction. */
    int2048(int2048 &&tmp) {
        swap(tmp);
        sign = tmp.sign;
    }

    /* Copy construction. */
    int2048(const int2048 &tmp) {
        copy(tmp);
        sign = tmp.sign;
    }

    /* Initialize from std::string. */
    int2048(const std::string &str) { read(str); }

    /* */
    int2048(const int2048 &X, size_t len) {
        sign = X.sign;
        reserve(len);
        copy(X);
    }

    int2048(const array<uint64_t> &X, bool _sign) {
        sign = _sign;
        copy(X);
    }

    int2048(array<uint64_t> &&X, bool _sign) {
        sign = _sign;
        swap(X);
    }

    std::string _TOSTRING() const {
        buffer.clear();
        buffer.reserve(sign + size() * baseBit);
        if (sign)
            buffer.push_back('-');
        buffer.append(std::to_string(back()));
        for (size_t i = size() - 2; ~i; --i) {
            buffer.append(numMap[(*this)[i]]);
        }
        return buffer;
    }

    explicit operator size_t() const {
        size_t j = size() > 6 ? size() - 7 : -1ULL;
        size_t ret = 0;
        for (size_t i = size() - 1; i != j; --i) {
            ret = ret * base + this->operator[](i);
        }
        return ret;
    }

    explicit operator double() const {
        size_t j = size() > 6 ? size() - 7 : -1ULL;
        size_t ret = 0;
        for (size_t i = size() - 1; i != j; --i) {
            ret = ret * base + this->operator[](i);
        }
        double ans = ret;
        ++j;
        double pbase = base;
        while (j) {
            if (j & 1) {
                ans *= pbase;
            }
            pbase *= pbase;
            j >>= 1;
        }
        return sign ? -ans : ans;
    }

    explicit operator bool() const { return back(); }
    
};

} // namespace sjtu

#endif
