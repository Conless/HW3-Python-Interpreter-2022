/**
 * @file int2048.cc
 * @author Conless
 * @brief The implementation of int2048.h
 * @version 0.1
 * @date 2022-11-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "int2048/int2048.h"
#include "int2048/utils.h"

namespace sjtu {

const int kDigitLen = 3;
const long long kDigit[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

const long long kMaximumNum = kDigit[kDigitLen];
const int2048 kMaximumNum_int2048 = kMaximumNum;

int2048::int2048() {
    sgn = 1;
    num.clear();
    num.push_back(0);
}

int2048::int2048(const int2048 &org_int2048_num) {
    sgn = org_int2048_num.sgn;
    num = org_int2048_num.num;
    reduce();
}

int2048::int2048(const std::string &org_str_num) {
    int num_len = org_str_num.length();
    int las_digit = num_len;
    if (org_str_num[0] == '-') {
        sgn = -1;
        las_digit = num_len - 1;
    } else {
        sgn = 1;
    }
    num.clear();
    for (int i = 0; i < las_digit; i++) {
        if (i % kDigitLen == 0) {
            num.push_back(0);
        }
        num.back() += (org_str_num[num_len - 1 - i] - '0') * kDigit[i % kDigitLen];
    }
    reduce();
}

int2048::int2048(long long org_ll_num) {
    num.clear();
    if (org_ll_num < 0) {
        org_ll_num = -org_ll_num;
        sgn = -1;
    } else {
        sgn = 1;
    }
    while (org_ll_num) {
        num.push_back(org_ll_num % kMaximumNum);
        org_ll_num /= kMaximumNum;
    }
    reduce();
}

void int2048::read(const std::string &rd_str_num) {
    *this = int2048(rd_str_num);
    return;
}

void int2048::print() const {
    int num_len = num.size();
    if (sgn == -1)
        putchar('-');
    printf("%lld", num[num_len - 1]);
    for (int i = num_len - 2; i >= 0; i--) {
        printf("%03lld", num[i]);
    }
    return;
}

std::istream &operator>>(std::istream &in, int2048 &input_num) {
    std::string input_str;
    in >> input_str;
    input_num = input_str;
    return in;
}

std::ostream &operator<<(std::ostream &out, const int2048 &output_num) {
    int len = output_num.num.size();
    if (output_num.sgn == -1)
        putchar('-');
    out << output_num.num[len - 1];
    for (int i = len - 2; i >= 0; i--) {
        out << std::setw(kDigitLen) << std::setfill('0') << output_num.num[i];
    }
    return out;
}

void int2048::reduce() {
    while (!num.empty() && !num.back())
        num.pop_back();
    if (num.empty()) {
        sgn = 1;
        num.push_back(0);
    }
}

int2048 abs(const int2048 &org_num) {
    int2048 abs_num = org_num;
    abs_num.sgn = 1;
    return abs_num;
}

bool operator==(const int2048 &num_left, const int2048 &num_right) {
    if (num_left.sgn != num_right.sgn)
        return false;
    int lenx = num_left.num.size(), leny = num_right.num.size();
    if (lenx != leny)
        return false;
    for (int i = 0; i < lenx; i++)
        if (num_left.num[i] != num_right.num[i])
            return false;
    return true;
}
bool operator!=(const int2048 &num_left, const int2048 &num_right) { return (num_left == num_right) ^ 1; }

bool operator<(const int2048 &num_left, const int2048 &num_right) {
    if (num_left.sgn != num_right.sgn)
        return num_left.sgn < num_right.sgn;
    if (num_left.sgn == -1)
        return abs(num_left) > abs(num_right);
    int lenx = num_left.num.size(), leny = num_right.num.size();
    if (lenx != leny)
        return lenx < leny;
    for (int i = lenx - 1; i >= 0; i--)
        if (num_left.num[i] != num_right.num[i])
            return num_left.num[i] < num_right.num[i];
    return false;
}
bool operator>(const int2048 &num_left, const int2048 &num_right) {
    if (num_left.sgn != num_right.sgn)
        return num_left.sgn > num_right.sgn;
    if (num_left.sgn == -1)
        return abs(num_left) < abs(num_right);
    int lenx = num_left.num.size(), leny = num_right.num.size();
    if (lenx != leny)
        return lenx > leny;
    for (int i = lenx - 1; i >= 0; i--)
        if (num_left.num[i] != num_right.num[i])
            return num_left.num[i] > num_right.num[i];
    return false;
}
bool operator<=(const int2048 &x, const int2048 &y) { return (x > y) ^ 1; }
bool operator>=(const int2048 &x, const int2048 &y) { return (x < y) ^ 1; }

int2048 &int2048::add(int2048 num_right) {
    int flag = 1;
    if (sgn == 1) {
        if (num_right.sgn == -1) {
            num_right.sgn = 1;
            minus(num_right);
            return *this;
        }
    } else {
        if (num_right.sgn == 1) {
            num_right.minus(abs(*this));
            *this = num_right;
            return *this;
        } else {
            sgn = num_right.sgn = 1;
            flag = -1;
        }
    }
    int len_left = num.size(), len_right = num_right.num.size();
    if (len_left > len_right) {
        for (int i = 0; i < len_right; i++) {
            num[i] += num_right.num[i];
            if (num[i] >= kMaximumNum) {
                num[i + 1] += num[i] / kMaximumNum;
                num[i] %= kMaximumNum;
            }
        }
    } else {
        for (int i = 0; i < len_left - 1; i++) {
            num[i] += num_right.num[i];
            if (num[i] >= kMaximumNum) {
                num[i + 1] += num[i] / kMaximumNum;
                num[i] %= kMaximumNum;
            }
        }
        num[len_left - 1] += num_right.num[len_left - 1];
        if (num[len_left - 1] >= kMaximumNum) {
            num.push_back(num[len_left - 1] / kMaximumNum);
            num[len_left - 1] %= kMaximumNum;
        } else if (len_right > len_left)
            num.push_back(0);
        for (int i = len_left; i < len_right; i++) {
            num[i] += num_right.num[i];
            num.push_back(0);
            if (num[i] >= kMaximumNum) {
                num[i + 1] += num[i] / kMaximumNum;
                num[i] %= kMaximumNum;
            }
        }
        if (num.back() == 0)
            num.pop_back();
    }
    sgn = flag;
    reduce();
    return *this;
}

int2048 add(int2048 num_left, const int2048 &num_right) { return num_left.add(num_right); }
int2048 &int2048::operator+=(int2048 num_right) { return add(num_right); }
int2048 &int2048::operator++() { return add(1); }
int2048 &int2048::operator++(int) { return add(1); }
int2048 operator+(int2048 num_left, const int2048 &num_right) { return num_left.add(num_right); }

int2048 &int2048::minus(int2048 num_right) {
    int flag = 1;
    if (sgn == 1) {
        if (num_right.sgn == 1) {
            if (*this < num_right) {
                int2048 res = num_right.minus(*this);
                res.sgn = -1;
                *this = res;
                return *this;
            }
        } else {
            num_right.sgn = 1;
            add(num_right);
            return *this;
        }
    } else {
        if (num_right.sgn == 1) {
            num_right.sgn = -1;
            add(num_right);
            return *this;
        } else {
            if (abs(*this) < abs(num_right)) {
                sgn = num_right.sgn = 1;
                num_right.minus(*this);
                *this = num_right;
                return *this;
            } else {
                sgn = num_right.sgn = 1;
                flag = -1;
            }
        }
    }
    int len_left = num.size(), len_right = num_right.num.size();
    for (int i = 0; i < len_right; i++) {
        num[i] -= num_right.num[i];
        if (num[i] < 0) {
            num[i + 1]--;
            num[i] += kMaximumNum;
        }
    }
    for (int i = len_right; i < len_left; i++) {
        if (num[i] < 0) {
            num[i + 1]--;
            num[i] += kMaximumNum;
        }
    }
    sgn = flag;
    reduce();
    return *this;
}

int2048 minus(int2048 num_left, const int2048 &num_right) { return num_left.minus(num_right); }
int2048 &int2048::operator-=(int2048 num_right) { return minus(num_right); }
int2048 &int2048::operator--() { return minus(1); }
int2048 &int2048::operator--(int) { return minus(1); }
int2048 operator-(int2048 num_left, const int2048 &num_right) { return num_left.minus(num_right); }

int2048 &int2048::operator*=(int2048 num_right) {
    sgn *= num_right.sgn;
    num = fft::mul(num, num_right.num);
    int len = num.size() - 1;

    for (int i = 0; i < len; i++) {
        if (num[i] >= kMaximumNum) {
            num[i + 1] += num[i] / kMaximumNum;
            num[i] %= kMaximumNum;
        }
    }
    reduce();
    return *this;
}

int2048 operator*(int2048 num_left, const int2048 &num_right) { return num_left *= num_right; }

void int2048::move_digit(int len) {
    if (len >= 0) {
        int n = num.size();
        for (int i = 0; i < len; i++)
            num.push_back(0);
        for (int i = n - 1; i >= 0; i--)
            num[i + len] = num[i];
        for (int i = 0; i < len; i++)
            num[i] = 0;
    } else {
        len = -len;
        int n = num.size();
        for (int i = 0; i < n - len; i++)
            num[i] = num[i + len];
        while (len--)
            num.pop_back();
    }
    reduce();
}

int2048 int2048::inv() {
    if (num.size() <= 10) {
        int2048 res;
        int siz = num.size() << 1 | 1;
        res.num.resize(siz);
        res.num[res.num.size() - 1] = 1;
        res.div(*this);
        return res;
    } else {
        int2048 pre, inv_pre;
        int len = num.size(), pre_len = (num.size() + 5) >> 1;
        pre.num.clear();
        pre.num.resize(pre_len);
        for (int i = pre_len - 1, j = len - 1; i >= 0; i--, j--)
            pre.num[i] = num[j];
        inv_pre = pre.inv();
        int2048 part1 = *this * inv_pre * inv_pre;
        part1.move_digit(-2 * pre_len);
        int2048 part2 = 2 * inv_pre;
        part2.move_digit(len - pre_len);
        int2048 res = part1 - part2 - 1;
        int2048 num_one;
        num_one.num.clear();
        num_one.num.resize(len << 1 | 1);
        num_one.num[len << 1] = 1;
        newton_iter::modify(num_one, *this, res);
        res.reduce();
        return res;
    }
}

int2048 int2048::div(int2048 num_right) {
    int2048 ans, num_divisor = num_right;
    ans.sgn = sgn * num_right.sgn;
    ans.num.clear();
    ans.num.resize(num.size());
    std::stack<int2048> st;
    st.push(num_divisor);
    int cnt_digit = 0;
    while (*this > num_divisor) {
        num_divisor *= kMaximumNum_int2048;
        st.push(num_divisor);
        cnt_digit++;
    }
    while (*this >= num_divisor) {
        while (st.top() > *this) {
            st.pop();
            cnt_digit--;
        }
        num_divisor = st.top();
        int l = 1, r = kMaximumNum - 1, res = 0;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (num_divisor * mid <= *this) {
                l = mid + 1;
                res = mid;
            } else {
                r = mid - 1;
            }
        }
        ans.num[cnt_digit] = res;
        *this -= num_divisor * res;
    }
    ans.reduce();
    *this = ans;
    return *this;
}

int2048 &int2048::operator/=(int2048 num_right) {
    if (*this < num_right)
        return *this = 0;
    int2048 num_left = *this;
    int len_left = num_left.num.size(), len_right = num_right.num.size();
    if (len_left > len_right * 2) {
        int len_move = len_left - len_right * 2;
        num_left.move_digit(len_move);
        num_right.move_digit(len_move);
        len_right += len_move;
        len_left -= len_move;
    }
    int2048 inv_num_right = num_right.inv();
    int2048 res = len_left * inv_num_right;
    res.move_digit(-(len_right * 2));
    newton_iter::modify(*this, num_right, res);
    res.reduce();
    return *this = res;
}
int2048 operator/(int2048 num_left, const int2048 &num_right) { return num_left /= num_right; }

} // namespace sjtu
