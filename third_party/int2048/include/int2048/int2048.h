/**
 * @file int2048.h
 * @author Conless Pan (conlesspan@outlook.com)
 * @brief The header file of sjtu::int2048
 * @version 0.1
 * @date 2022-11-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// 实现一个有符号的大整数类，只需支持简单的加减

// Integer 2:
// 实现一个有符号的大整数类，支持加减乘除，并重载相关运算符

// 请不要使用除了以下头文件之外的其它头文件
#include <complex>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stack>
#include <vector>

// 请不要使用 using namespace std;

namespace sjtu {

class int2048 {
  private:
    int sgn;
    std::vector<long long> num;

  public:
    /**
     * @brief Construct a new int2048::int2048 object
     * @details The constructor and copy constructor of a new int2048 object
     */
    int2048();
    int2048(const int2048 &);

    /**
     * @brief Construct a new int2048 object
     * @details The converter of int2048, from a string (usually a big number), or a long long (usually a smaller number)
     */
    int2048(const std::string &);
    int2048(long long x);

  public:
    /**
     * @brief Input and output a int2048
     * @details The read function is the same as converter (std::string), and the output is by printf()
     */
    void read(const std::string &);
    // output a big int
    void print() const;

    /**
     * @brief The custom input and output operator of int2048
     * @details Using overloaded function to allow the I/O of int2048 in C++ style
     * @return std::istream& , std::ostream&
     */
    friend std::istream &operator>>(std::istream &, int2048 &);
    friend std::ostream &operator<<(std::ostream &, const int2048 &);

    /**
     * @brief To simplify a int2048 number
     * @details Deleting the suffix zero or resize the num vector. It's suggested to use this function after every operation, in
     * order to avoid some unknown errors
     */
    void reduce();

    /**
     * @brief To get the absolute value of a int2048 number
     * @return int2048
     */
    friend int2048 abs(const int2048 &);

  public:
    /**
     * @brief The compare function of int2048 numbers
     */
    friend bool operator==(const int2048 &, const int2048 &);
    friend bool operator!=(const int2048 &, const int2048 &);
    friend bool operator<(const int2048 &, const int2048 &);
    friend bool operator>(const int2048 &, const int2048 &);
    friend bool operator<=(const int2048 &, const int2048 &);
    friend bool operator>=(const int2048 &, const int2048 &);

    /**
     * @brief The self addition function of int2048
     * @details It contains the main operation of the addition of int2048, which costs O(n) time
     * @return int2048& (Editing the result of self-add is equivalent to editing the left value of self-add)
     */
    int2048 &add(int2048);

    /**
     * @brief The addition function of int2048
     * @return int2048(&)
     */
    friend int2048 add(int2048, const int2048 &);
    int2048 &operator+=(int2048);
    int2048 &operator++();
    int2048 &operator++(int);
    friend int2048 operator+(int2048, const int2048 &);

    /**
     * @brief The self subtraction function of int2048
     * @details It contains the main operation of the subtraction of int2048, which costs O(n) time
     * @return int2048& (Editing the result of self-minus is equivalent to editing the left value of self-minus)
     */
    int2048 &minus(int2048);
    /**
     * @brief The subtracting function of int2048
     * @return int2048(&)
     */
    friend int2048 minus(int2048, const int2048 &);
    int2048 &operator-=(int2048);
    int2048 &operator--();
    int2048 &operator--(int);
    friend int2048 operator-(int2048, const int2048 &);

    /**
     * @brief The self multiplication function of int2048
     * @details It contains the main operation of the multiplication of int2048, which costs O(nlogn) time
     * @return int2048& (Editing the result of self-mul is equivalent to editing the left value of self-mul)
     */
    int2048 &operator*=(int2048);
    friend int2048 operator*(int2048, const int2048 &);

    /**
     * @brief Some tools using in the division of int2048
     * @details The functions of moving the digits of int2048 and getting the inverse of a int2048 number
     */
  private:
    void move_digit(int);
    int2048 div(int2048);
    int2048 inv();
    /**
     * @brief The self division function of int2048
     * @details It contains the main operation of the division of int2048, which costs O(nlogn) time
     * @return int2048& (Editing the result of self-div is equivalent to editing the left value of self-div)
     */
  public:
    int2048 &operator/=(int2048);
    friend int2048 operator/(int2048, const int2048 &);
    int2048 &operator%=(int2048);
    friend int2048 operator%(int2048, const int2048 &);
};

} // namespace sjtu

#endif
