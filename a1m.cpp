#include <iostream>
#include <vector>
#include <string>
#include <utility>

struct LcpResult {
    int cmp;
    size_t lcp;
};

LcpResult lcpCompare(const std::string& a, const std::string& b, size_t k) {
    size_t la = a.size();
    size_t lb = b.size();
    while (k < la && k < lb) {
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

void strMerge(std::vector<std::string>& v, std::vector<std::string>& buf,
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

void strMergeRec(std::vector<std::string>& v, std::vector<std::string>& buf,
                 std::vector<size_t>& kArr, std::vector<size_t>& kBuf, int lo, int hi) {
    if (hi - lo <= 1) {
        kArr[lo] = 0;
        return;
    }
    int mid = (lo + hi) / 2;
    strMergeRec(v, buf, kArr, kBuf, lo, mid);
    strMergeRec(v, buf, kArr, kBuf, mid, hi);
    strMerge(v, buf, kArr, kBuf, lo, mid, hi);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    std::vector<std::string> v(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> v[i];
    }
    std::vector<std::string> buf(n);
    std::vector<size_t> kArr(n, 0);
    std::vector<size_t> kBuf(n, 0);
    strMergeRec(v, buf, kArr, kBuf, 0, n);

    std::string out;
    for (auto& s : v) {
        out += s;
        out += '\n';
    }
    std::cout << out;
    return 0;
}
