#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <stdexcept>
#include "StringGenerator.h"

inline size_t comparisonCount = 0;

inline bool compareStrings(const std::string& a, const std::string& b) {
    const size_t minLen = std::min(a.length(), b.length());
    for (size_t i = 0; i < minLen; ++i) {
        comparisonCount++;
        if (a[i] != b[i]) return a[i] < b[i];
    }
    return a.length() < b.length();
}

struct SortResult {
    size_t size;
    double time_us;
    size_t comparisons;
};

class StringSortTester {
public:
    using SortFn = void (*)(std::vector<std::string>&, size_t, size_t);

    explicit StringSortTester(const int repeats = 5) : repeats_(repeats) {}

    SortResult measure(const std::vector<std::string>& base, const size_t sz, const SortFn fn) const {
        if (sz > base.size()) {
            throw std::invalid_argument("");
        }

        double totalTime  = 0.0;
        size_t totalComps = 0;

        for (int r = 0; r < repeats_; ++r) {
            std::vector arr(base.begin(), base.begin() + sz);
            comparisonCount = 0;

            auto t0 = std::chrono::high_resolution_clock::now();
            fn(arr, 0, sz - 1);
            auto t1 = std::chrono::high_resolution_clock::now();

            totalTime  += std::chrono::duration<double, std::micro>(t1 - t0).count();
            totalComps += comparisonCount;
        }

        return { sz,
                 totalTime  / repeats_,
                 totalComps / static_cast<size_t>(repeats_) };
    }

    std::vector<SortResult> runSuite(const std::vector<std::string>& base, const SortFn fn) const {
        std::vector<SortResult> results;
        const size_t maxSz = std::min(base.size(), StringGenerator::getMaxArraySize());
        for (size_t sz = StringGenerator::getMinArraySize();
             sz <= maxSz;
             sz += StringGenerator::getArrayStep())
            results.push_back(measure(base, sz, fn));
        return results;
    }

    static void writeToCsv(const std::string& path, const std::vector<SortResult>& results) {
        std::ofstream f(path);
        if (!f) throw std::runtime_error("Cannot open: " + path);
        f << "size,time_us,comparisons\n";
        for (auto& r : results)
            f << r.size << "," << r.time_us << "," << r.comparisons << "\n";
    }

private:
    int repeats_;
};