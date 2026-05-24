#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>

#include "StringGenerator.cpp"
#include "StringSortTester.cpp"

struct Algo {
    std::string name;
    StringSortTester::SortFn fn;
};

struct Dataset {
    std::string name;
    const std::vector<std::string>* full;
};

int main() {
    StringGenerator gen(20260524);

    const int maxN = 3000;
    const int step = 100;
    const int runs = 3;

    auto arrRand = gen.randomArr(maxN);
    auto arrRev  = gen.reverseArr(maxN);
    auto arrAlm  = gen.almostArr(maxN);
    auto arrPref = gen.commonPrefixArr(maxN, 8, 10);

    std::vector<Algo> algos = {
        { "std_quick",    StringSortTester::standardQuickSort },
        { "std_merge",    StringSortTester::standardMergeSort },
        { "string_merge", StringSortTester::stringMergeSort   },
        { "string_quick", StringSortTester::stringQuickSort   },
        { "msd_radix",    StringSortTester::msdRadixSort      },
        { "msd_radix_qs", StringSortTester::msdRadixQuickSort },
    };

    std::vector<Dataset> datasets = {
        { "random",  &arrRand },
        { "reverse", &arrRev  },
        { "almost",  &arrAlm  },
        { "prefix",  &arrPref },
    };

    auto sample = gen.prefix(arrRand, 200);
    std::vector<std::string> ref = sample;
    std::sort(ref.begin(), ref.end());
    for (auto& a : algos) {
        auto v = sample;
        a.fn(v);
        if (v != ref) {
            std::cerr << "INCORRECT: " << a.name << "\n";
            return 1;
        }
    }
    std::cout << "sanity check ok\n";

    for (auto& ds : datasets) {
        std::string fileName = ds.name + "-results.txt";
        std::ofstream out(fileName);
        std::cout << "dataset = " << ds.name << "\n";
        for (int n = step; n <= maxN; n += step) {
            auto input = gen.prefix(*ds.full, n);
            for (auto& a : algos) {
                auto r = StringSortTester::measure(a.fn, input, runs);
                out << n << ", " << a.name << ", "
                    << std::fixed << std::setprecision(3) << r.avgTimeUs
                    << ", " << r.charCmp << "\n";
            }
        }
        out.close();
    }

    std::cout << "finish\n";
    return 0;
}
