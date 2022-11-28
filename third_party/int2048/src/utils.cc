#include "int2048/utils.h"

namespace fft {

void change(std::vector<complex> &a, int len) {
    for (int i = 1, j = len / 2; i < len - 1; i++) {
        if (i < j)
            std::swap(a[i], a[j]);
        int k = len / 2;
        while (j >= k) {
            j -= k;
            k >>= 1;
        }
        if (j < k)
            j += k;
    }
}

const double Pi = std::acos(-1);

void FFT(std::vector<complex> &a, int len, int opt) {
    change(a, len);
    for (int i = 2; i <= len; i <<= 1) {
        complex wn(cos(2 * Pi / i), opt * sin(2 * Pi / i));
        for (int j = 0; j < len; j += i) {
            complex w(1, 0);
            for (int k = j; k < j + i / 2; k++) {
                complex u = a[k], t = w * a[k + i / 2];
                a[k] = u + t;
                a[k + i / 2] = u - t;
                w = w * wn;
            }
        }
    }
    if (opt == -1) {
        for (int i = 0; i < len; i++)
            a[i].real(a[i].real() / len);
    }
}

std::vector<long long> mul(const std::vector<long long> &num_left, const std::vector<long long> &num_right) {
    int deg_left = num_left.size() - 1, deg_right = num_right.size() - 1;
    int deg_res = deg_left + deg_right;
    int deg_fft = 1, deg_lb = 0;
    while (deg_fft <= deg_res) {
        deg_fft <<= 1;
        deg_lb++;
    }
    static std::vector<complex> f, g;
    f.clear();
    f.resize(deg_fft);
    g.clear();
    g.resize(deg_fft);
    for (int i = 0; i <= deg_left; i++)
        f[i] = num_left[i];
    for (int i = deg_left + 1; i < deg_fft; i++)
        f[i] = 0;
    for (int i = 0; i <= deg_right; i++)
        g[i] = num_right[i];
    for (int i = deg_right + 1; i < deg_fft; i++)
        g[i] = 0;
    FFT(f, deg_fft, 1);
    FFT(g, deg_fft, 1);
    for (int i = 0; i < deg_fft; i++)
        f[i] = f[i] * g[i];
    FFT(f, deg_fft, -1);
    std::vector<long long> res;
    res.clear();
    res.resize(deg_res + 2);
    for (int i = 0; i <= deg_res; i++) {
        res[i] += static_cast<long long>(f[i].real() + 0.5);
    }
    return res;
}

} // namespace fft

namespace newton_iter {
void modify(const sjtu::int2048 &num_dividend, const sjtu::int2048 &num_divisor, sjtu::int2048 &num_res) {
    sjtu::int2048 rem = num_dividend - num_res * num_divisor;
    if (rem > num_divisor)
        num_res++;
    return;
}
} // namespace newton_iter