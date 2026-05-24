#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>

class StringGenerator {
private:
    const std::string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-.";;
    static const size_t MIN_STRING_LENGTH = 10;
    static const size_t MAX_STRING_LENGTH = 200;
    static const size_t MIN_ARRAY_SIZE = 100;
    static const size_t MAX_ARRAY_SIZE = 3000;
    static const size_t ARRAY_STEP = 100;

    std::mt19937 rng;
    std::uniform_int_distribution<size_t> length_dist;
    std::uniform_int_distribution<size_t> char_dist;
    std::uniform_int_distribution<size_t> swap_dist;

    std::string generateRandomString() {
        size_t length = length_dist(rng);
        std::string result;
        result.reserve(length);

        for (size_t i = 0; i < length; ++i) {
            result += ALPHABET[char_dist(rng)];
        }

        return result;
    }

    std::vector<std::string> generateMaxRandomArray() {
        std::vector<std::string> array;
        array.reserve(MAX_ARRAY_SIZE);

        for (size_t i = 0; i < MAX_ARRAY_SIZE; ++i) {
            array.push_back(generateRandomString());
        }

        return array;
    }


public:
    StringGenerator(unsigned int seed = std::random_device{}())
        : rng(seed),
          length_dist(MIN_STRING_LENGTH, MAX_STRING_LENGTH),
          char_dist(0, ALPHABET.size() - 1),
          swap_dist(0, MAX_ARRAY_SIZE - 1) {}

    static std::vector<std::string> getSubarray(const std::vector<std::string>& maxArray, size_t size) {
        if (size > maxArray.size()) {
            throw std::invalid_argument("");
        }

        return std::vector<std::string>(maxArray.begin(), maxArray.begin() + size);
    }

    std::vector<std::string> generateRandomArray(size_t size) {
        auto maxArray = generateMaxRandomArray();
        return getSubarray(maxArray, size);
    }

    std::vector<std::string> generateReverseSortedArray(size_t size) {
        auto maxArray = generateMaxRandomArray();

        std::sort(maxArray.begin(), maxArray.end());
        std::reverse(maxArray.begin(), maxArray.end());

        return getSubarray(maxArray, size);
    }

    std::vector<std::string> generateNearlySortedArray(size_t size, size_t swapsCount = 10) {
        auto maxArray = generateMaxRandomArray();

        std::sort(maxArray.begin(), maxArray.end());

        std::uniform_int_distribution<size_t> index_dist(0, maxArray.size() - 1);

        for (size_t i = 0; i < swapsCount; ++i) {
            size_t idx1 = index_dist(rng);
            size_t idx2 = index_dist(rng);

            if (idx1 != idx2) {
                std::swap(maxArray[idx1], maxArray[idx2]);
            }
        }

        return getSubarray(maxArray, size);
    }

    struct TestArrays {
        std::vector<std::string> random;
        std::vector<std::string> reverseSorted;
        std::vector<std::string> nearlySorted;
    };

    TestArrays generateAllTestArrays(size_t size, size_t swapsForNearlySorted = 10) {
        TestArrays result;
        result.random = generateRandomArray(size);
        result.reverseSorted = generateReverseSortedArray(size);
        result.nearlySorted = generateNearlySortedArray(size, swapsForNearlySorted);
        return result;
    }

    static size_t getMinArraySize() { return MIN_ARRAY_SIZE; }
    static size_t getMaxArraySize() { return MAX_ARRAY_SIZE; }
    static size_t getArrayStep() { return ARRAY_STEP; }
    static size_t getMinStringLength() { return MIN_STRING_LENGTH; }
    static size_t getMaxStringLength() { return MAX_STRING_LENGTH; }
};