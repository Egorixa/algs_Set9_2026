#include <vector>
#include <string>
#include <chrono>
#include <cstdint>
#include <algorithm>
#include <functional>
#include <utility>

struct Result {
    double avgTimeUs;
    uint64_t charCmp;
};

struct LcpResult {
    int cmp;
    size_t lcp;
};

class StringSortTester {
public:
    static uint64_t charCmp;

    static int fullCompare(const std::string& a, const std::string& b) {
        size_t la = a.size();
        size_t lb = b.size();
        size_t k = 0;
        while (k < la && k < lb) {
            ++charCmp;
            if (a[k] != b[k]) {
                if (static_cast<unsigned char>(a[k]) < static_cast<unsigned char>(b[k])) {
                    return -1;
                }
                return 1;
            }
            ++k;
        }
        if (la == lb) {
            return 0;
        }
        if (la < lb) {
            return -1;
        }
        return 1;
    }

    static LcpResult lcpCompare(const std::string& a, const std::string& b, size_t k) {
        size_t la = a.size();
        size_t lb = b.size();
        while (k < la && k < lb) {
            ++charCmp;
            if (a[k] != b[k]) {
                int c = 1;
                if (static_cast<unsigned char>(a[k]) < static_cast<unsigned char>(b[k])) {
                    c = -1;
                }
                return { c, k };
            }
            ++k;
        }
        if (la == lb) {
            return { 0, k };
        }
        if (la < lb) {
            return { -1, k };
        }
        return { 1, k };
    }

    static void standardQuickSort(std::vector<std::string>& v) {
        if (v.size() <= 1) {
            return;
        }
        stdQuickRec(v, 0, static_cast<int>(v.size()) - 1);
    }

    static void standardMergeSort(std::vector<std::string>& v) {
        if (v.size() <= 1) {
            return;
        }
        std::vector<std::string> buf(v.size());
        stdMergeRec(v, buf, 0, static_cast<int>(v.size()));
    }

    static void stringMergeSort(std::vector<std::string>& v) {
        size_t n = v.size();
        if (n <= 1) {
            return;
        }
        std::vector<std::string> buf(n);
        std::vector<size_t> kArr(n, 0);
        std::vector<size_t> kBuf(n, 0);
        strMergeRec(v, buf, kArr, kBuf, 0, static_cast<int>(n));
    }

    static void stringQuickSort(std::vector<std::string>& v) {
        if (v.size() <= 1) {
            return;
        }
        tqSort(v, 0, static_cast<int>(v.size()), 0);
    }

    static void msdRadixSort(std::vector<std::string>& v) {
        size_t n = v.size();
        if (n <= 1) {
            return;
        }
        std::vector<std::string> aux(n);
        msdRec(v, aux, 0, static_cast<int>(n), 0, false);
    }

    static void msdRadixQuickSort(std::vector<std::string>& v) {
        size_t n = v.size();
        if (n <= 1) {
            return;
        }
        std::vector<std::string> aux(n);
        msdRec(v, aux, 0, static_cast<int>(n), 0, true);
    }

    using SortFn = std::function<void(std::vector<std::string>&)>;

    static Result measure(SortFn sort, const std::vector<std::string>& input, int runs = 3) {
        double totalUs = 0.0;
        uint64_t cmp = 0;
        for (int r = 0; r < runs; ++r) {
            std::vector<std::string> work = input;
            charCmp = 0;
            auto t0 = std::chrono::steady_clock::now();
            sort(work);
            auto t1 = std::chrono::steady_clock::now();
            totalUs += std::chrono::duration<double, std::micro>(t1 - t0).count();
            if (r == 0) {
                cmp = charCmp;
            }
        }
        return { totalUs / runs, cmp };
    }

private:
    static constexpr int RADIX = 257;

    static void stdQuickRec(std::vector<std::string>& v, int lo, int hi) {
        if (lo >= hi) {
            return;
        }
        std::swap(v[lo], v[hi]);
        int store = lo;
        for (int j = lo; j < hi; ++j) {
            if (fullCompare(v[j], v[hi]) < 0) {
                std::swap(v[store], v[j]);
                ++store;
            }
        }
        std::swap(v[store], v[hi]);
        stdQuickRec(v, lo, store - 1);
        stdQuickRec(v, store + 1, hi);
    }

    static void stdMergeRec(std::vector<std::string>& v, std::vector<std::string>& buf,
                            int lo, int hi) {
        if (hi - lo <= 1) {
            return;
        }
        int mid = (lo + hi) / 2;
        stdMergeRec(v, buf, lo, mid);
        stdMergeRec(v, buf, mid, hi);
        int i = lo;
        int j = mid;
        int k = lo;
        while (i < mid && j < hi) {
            if (fullCompare(v[i], v[j]) <= 0) {
                buf[k] = std::move(v[i]);
                ++i;
            } else {
                buf[k] = std::move(v[j]);
                ++j;
            }
            ++k;
        }
        while (i < mid) {
            buf[k] = std::move(v[i]);
            ++i;
            ++k;
        }
        while (j < hi) {
            buf[k] = std::move(v[j]);
            ++j;
            ++k;
        }
        for (int t = lo; t < hi; ++t) {
            v[t] = std::move(buf[t]);
        }
    }

    static void strMergeRec(std::vector<std::string>& v, std::vector<std::string>& buf,
                            std::vector<size_t>& kArr, std::vector<size_t>& kBuf,
                            int lo, int hi) {
        if (hi - lo <= 1) {
            kArr[lo] = 0;
            return;
        }
        int mid = (lo + hi) / 2;
        strMergeRec(v, buf, kArr, kBuf, lo, mid);
        strMergeRec(v, buf, kArr, kBuf, mid, hi);
        strMerge(v, buf, kArr, kBuf, lo, mid, hi);
    }

    static void strMerge(std::vector<std::string>& v, std::vector<std::string>& buf,
                         std::vector<size_t>& kArr, std::vector<size_t>& kBuf,
                         int lo, int mid, int hi) {
        int i = lo;
        int j = mid;
        int t = lo;
        size_t ki = 0;
        size_t pj = 0;
        while (i < mid && j < hi) {
            if (ki > pj) {
                buf[t] = std::move(v[i]);
                kBuf[t] = ki;
                ++i;
                ++t;
                ki = 0;
                if (i < mid) {
                    ki = kArr[i];
                }
            } else if (ki < pj) {
                buf[t] = std::move(v[j]);
                kBuf[t] = pj;
                ++j;
                ++t;
                pj = 0;
                if (j < hi) {
                    pj = kArr[j];
                }
            } else {
                LcpResult r = lcpCompare(v[i], v[j], ki);
                if (r.cmp <= 0) {
                    buf[t] = std::move(v[i]);
                    kBuf[t] = ki;
                    ++i;
                    ++t;
                    pj = r.lcp;
                    ki = 0;
                    if (i < mid) {
                        ki = kArr[i];
                    }
                } else {
                    buf[t] = std::move(v[j]);
                    kBuf[t] = pj;
                    ++j;
                    ++t;
                    ki = r.lcp;
                    pj = 0;
                    if (j < hi) {
                        pj = kArr[j];
                    }
                }
            }
        }
        while (i < mid) {
            buf[t] = std::move(v[i]);
            kBuf[t] = ki;
            ++i;
            ++t;
            ki = 0;
            if (i < mid) {
                ki = kArr[i];
            }
        }
        while (j < hi) {
            buf[t] = std::move(v[j]);
            kBuf[t] = pj;
            ++j;
            ++t;
            pj = 0;
            if (j < hi) {
                pj = kArr[j];
            }
        }
        for (int s = lo; s < hi; ++s) {
            v[s] = std::move(buf[s]);
            kArr[s] = kBuf[s];
        }
        kArr[lo] = 0;
    }

    static int charAtCmp(const std::string& s, size_t d) {
        if (d < s.size()) {
            ++charCmp;
            return static_cast<unsigned char>(s[d]);
        }
        return -1;
    }

    static int charAt(const std::string& s, size_t d) {
        if (d < s.size()) {
            return static_cast<unsigned char>(s[d]);
        }
        return -1;
    }

    static void tqSort(std::vector<std::string>& v, int lo, int hi, size_t d) {
        if (hi - lo <= 1) {
            return;
        }
        int lt = lo;
        int gt = hi - 1;
        int pivot = charAtCmp(v[lo], d);
        int i = lo + 1;
        while (i <= gt) {
            int t = charAtCmp(v[i], d);
            if (t < pivot) {
                std::swap(v[lt], v[i]);
                ++lt;
                ++i;
            } else if (t > pivot) {
                std::swap(v[i], v[gt]);
                --gt;
            } else {
                ++i;
            }
        }
        tqSort(v, lo, lt, d);
        if (pivot >= 0) {
            tqSort(v, lt, gt + 1, d + 1);
        }
        tqSort(v, gt + 1, hi, d);
    }

    static void msdRec(std::vector<std::string>& v, std::vector<std::string>& aux,
                       int lo, int hi, size_t d, bool hybrid) {
        int n = hi - lo;
        if (n <= 1) {
            return;
        }
        if (hybrid && n < StringGenerator::ALPHABET_SIZE) {
            tqSort(v, lo, hi, d);
            return;
        }
        int count[RADIX + 1] = { 0 };
        for (int i = lo; i < hi; ++i) {
            ++charCmp;
            int c = charAt(v[i], d) + 1;
            ++count[c + 1];
        }
        for (int r = 0; r < RADIX; ++r) {
            count[r + 1] += count[r];
        }
        for (int i = lo; i < hi; ++i) {
            int c = charAt(v[i], d) + 1;
            aux[lo + count[c]] = std::move(v[i]);
            ++count[c];
        }
        for (int i = lo; i < hi; ++i) {
            v[i] = std::move(aux[i]);
        }
        int prev = 0;
        for (int r = 0; r < RADIX; ++r) {
            int start = prev;
            int end = count[r];
            if (r > 0 && end - start > 1) {
                msdRec(v, aux, lo + start, lo + end, d + 1, hybrid);
            }
            prev = end;
        }
    }
};

uint64_t StringSortTester::charCmp = 0;
