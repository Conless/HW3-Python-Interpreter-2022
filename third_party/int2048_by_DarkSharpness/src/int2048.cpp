#include "int2048/int2048.h"

namespace sjtu {

int2048 dto2048(double tmp) {
        std::string str = std::to_string(tmp);
        size_t idx = str.find('.');
        if (idx != std::string::npos)
            str.resize(idx);
        int2048 ans;
        ans.read(str);
        return ans;
    }

constexpr uint64_t int2048::DIV_minLen;
constexpr int32_t  int2048::judgeMap[2];
constexpr uint64_t int2048::MULT_minLen;
constexpr uint64_t int2048::unit[20];
constexpr uint64_t int2048::initLen;
constexpr uint64_t int2048::baseBit;
constexpr uint64_t int2048::base;
constexpr uint64_t int2048::FFT_maxBit;
constexpr uint64_t int2048::FFT_maxLen;
complex int2048::root[20];
std::string int2048::numMap[1000];
std::string int2048::buffer = "";

const size_t siz = sizeof(int2048);

struct int2048_initializer : private int2048{
    int2048_initializer() {
        FFT_init();
    }
}_THIS_OBJECT_IS_CONSTRUCTED_TO_INITIALIZE_;

}