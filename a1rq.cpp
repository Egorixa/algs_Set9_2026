#include <iostream>
#include <vector>
#include <string>
#include <utility>

constexpr int RADIX = 257;

int charAt(const std::string& s, size_t d) {
    if (d < s.size()) {
        return static_cast<unsigned char>(s[d]);
    }
    return -1;
}

void tqSort(std::vector<std::string>& v, int lo, int hi, size_t d) {
    if (hi - lo <= 1) {
        return;
    }
    int lt = lo;
    int gt = hi - 1;
    int pivot = charAt(v[lo], d);
    int i = lo + 1;
    while (i <= gt) {
        int t = charAt(v[i], d);
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

void msdRec(std::vector<std::string>& v, std::vector<std::string>& aux,
            int lo, int hi, size_t d) {
    int n = hi - lo;
    if (n <= 1) {
        return;
    }
    int alph_size = 74;
    if (n < alph_size) {
        tqSort(v, lo, hi, d);
        return;
    }
    std::vector<int> count(RADIX + 1, 0);
    for (int i = lo; i < hi; ++i) {
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
            msdRec(v, aux, lo + start, lo + end, d + 1);
        }
        prev = end;
    }
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
    std::vector<std::string> aux(n);
    msdRec(v, aux, 0, n, 0);

    std::string out;
    for (auto& s : v) {
        out += s;
        out += '\n';
    }
    std::cout << out;
    return 0;
}
