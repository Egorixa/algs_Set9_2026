#include <vector>
#include <string>
#include <random>
#include <algorithm>

class StringGenerator {
private:
    std::string chars;
    std::mt19937 rnd;

    std::string makeOne(int minLen, int maxLen) {
        std::uniform_int_distribution<int> lenDist(minLen, maxLen);
        std::uniform_int_distribution<int> chDist(0, static_cast<int>(chars.size()) - 1);
        int len = lenDist(rnd);
        std::string res;
        res.reserve(len);
        for (int i = 0; i < len; ++i) {
            res += chars[chDist(rnd)];
        }
        return res;
    }

public:
    static constexpr int MIN_LEN = 10;
    static constexpr int MAX_LEN = 200;
    static constexpr int ALPHABET_SIZE = 74;

    StringGenerator(uint32_t seed = 19082006) {
        chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-.";
        rnd.seed(seed);
    }

    std::vector<std::string> randomArr(size_t n) {
        std::vector<std::string> v;
        v.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            v.push_back(makeOne(MIN_LEN, MAX_LEN));
        }
        return v;
    }

    std::vector<std::string> reverseArr(size_t n) {
        auto v = randomArr(n);
        std::sort(v.begin(), v.end(), std::greater<std::string>());
        return v;
    }

    std::vector<std::string> almostArr(size_t n) {
        auto v = randomArr(n);
        std::sort(v.begin(), v.end());
        size_t swaps = n / 50;
        if (swaps < 1) {
            swaps = 1;
        }
        std::uniform_int_distribution<size_t> idx(0, n - 1);
        for (size_t k = 0; k < swaps; ++k) {
            size_t a = idx(rnd);
            size_t b = idx(rnd);
            std::swap(v[a], v[b]);
        }
        return v;
    }

    std::vector<std::string> prefix(const std::vector<std::string>& full, size_t n) {
        return std::vector<std::string>(full.begin(), full.begin() + n);
    }

    std::vector<std::string> commonPrefixArr(size_t n, int prefixLen = 8, int groups = 10) {
        std::uniform_int_distribution<int> chDist(0, static_cast<int>(chars.size()) - 1);
        std::vector<std::string> prefixes;
        prefixes.reserve(groups);
        for (int g = 0; g < groups; ++g) {
            std::string p;
            p.reserve(prefixLen);
            for (int i = 0; i < prefixLen; ++i) {
                p += chars[chDist(rnd)];
            }
            prefixes.push_back(p);
        }
        std::uniform_int_distribution<int> groupDist(0, groups - 1);
        std::vector<std::string> v;
        v.reserve(n);
        int minTail = MIN_LEN - prefixLen;
        int maxTail = MAX_LEN - prefixLen;
        if (minTail < 1) {
            minTail = 1;
        }
        for (size_t i = 0; i < n; ++i) {
            std::string s = prefixes[groupDist(rnd)] + makeOne(minTail, maxTail);
            v.push_back(s);
        }
        return v;
    }
};
