#include <iostream>
#include <vector>
#include <string>
#include <utility>

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

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    std::vector<std::string> v(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> v[i];
    }
    tqSort(v, 0, n, 0);

    std::string out;
    for (auto& s : v) {
        out += s;
        out += '\n';
    }
    std::cout << out;
    return 0;
}
