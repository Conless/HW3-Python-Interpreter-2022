/**
 * @file tools.h
 * @author Conless Pan (conlesspan@outlook.com)
 * @brief
 * @version 0.1
 * @date 2022-11-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SJTU_BIGINTEGER_UTILS
#define SJTU_BIGINTEGER_UTILS

#include <complex>
#include <vector>

#include "int2048/int2048.h"

typedef std::complex<double> complex;

namespace fft {
void change(std::vector<complex> &, int);
void FFT(std::vector<complex> &, int, int);
std::vector<long long> mul(const std::vector<long long> &, const std::vector<long long> &);
} // namespace fft

namespace newton_iter {
void modify(const sjtu::int2048 &, const sjtu::int2048 &, sjtu::int2048 &);
}
#endif