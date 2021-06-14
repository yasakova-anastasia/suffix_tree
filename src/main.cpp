#include <chrono>
#include "OneToOneCode.h"
#include "SuffixTree.h"

std::vector<std::string> input(int code_number) {
    std::vector<std::string> vec;
    for (int i = 0; i < code_number; ++i) {
        std::string tmp = "";
        std::cout << i << ": ";
        std::cin >> tmp;
        vec.push_back(tmp);
    }
    std::cout << std::endl;
    return vec;
}

int main() {
    OneToOneCode code;
    std::cout << "Select the option to get the code:\n   1 - input\n   2 - random\n   3 - ready sample\n==> ";
    int k = 0;
    std::cin >> k;
    std::cout << std::endl;
    if (k == 1) {
        std::cout << "alphabet power = ";
        int alphabet_power = 0;
        std::cin >> alphabet_power;
        std::cout << "number of code words = ";
        int code_number = 0;
        std::cin >> code_number;
        std::vector<std::string> vec = input(code_number);
        code.SetCode(vec, alphabet_power);
    }
    else if (k == 2) {
        std::cout << "alphabet power = ";
        int alphabet_power = 0;
        std::cin >> alphabet_power;
        std::cout << "number of code words = ";
        int code_number = 0;
        std::cin >> code_number;
        std::cout << "min code word length = ";
        int min = 0;
        std::cin >> min;
        std::cout << "max code word length = ";
        int max = 0;
        std::cin >> max;
        std::cout << std::endl;

        code.GenerateCode(code_number, alphabet_power, min, max);
    }
    else {
        std::vector<std::vector<std::string>> samples = { { "1", "010", "101" },
            { "1", "01", "100", "0100", "0000" },
            { "10", "12", "012", "101", "2100" },
            { "10", "01", "12", "012", "2100", "12011", "12010" },
            { "a", "ab", "cab", "baac" } };
        std::vector<int> alphabet_power = { 2, 2, 3, 3, 3 };

        for (int i = 0; i < samples.size(); ++i) {
            std::cout << i << ": { ";
            for (int j = 0; j < samples[i].size(); ++j) {
                if (j != 0) {
                    std::cout << ", ";
                }
                std::cout << samples[i][j];
            }
            std::cout << " }" << std::endl;
        }
        int n = -1;
        while (n < 0 && n > samples.size() - 1) {
            std::cout << "==> ";
            std::cin >> n;
        }
        std::cout << std::endl;
        code.SetCode(samples[n], alphabet_power[n]);
    }

    std::cout << "   1 - educational experiment\n   2 - experiment with time measurement\n==> ";
    int print = 0;
    std::cin >> print;
    std::cout << std::endl;
    if (print == 1) {
        code.PrintCode();
        std::cout << "Macmillan's Inequality: " << code.Macmillan_inequality() << " <= 1\n\n";
        std::cout << "Select an algorithm for finding partitioning of elementary code:\n"
            "   1 - Simple algorithm\n   2 - Knuth-Morris-Pratt algorithm\n" <<
            "   3 - Ukkonen's algorithm\n   4 - McCreight's algorithm\n==> ";
        std::cin >> k;
        if (code.MarkovAlgorithm(k - 1, true)) {
            std::cout << "The code is one-to-one\n";
        }
        else {
            std::cout << "The code is not one-to-one\n";
        }
    }
    else {
        bool res_0 = false, res_1 = false, res_2 = false, res_3 = false;
        auto start = std::chrono::high_resolution_clock::now();
        res_0 = code.MarkovAlgorithm(0, false);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Simple algorithm:\t" <<
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        res_1 = code.MarkovAlgorithm(1, false);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Knuth-Morris-Pratt algorithm:\t" <<
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        res_2 = code.MarkovAlgorithm(2, false);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Ukkonen's algorithm algorithm:\t" <<
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        start = std::chrono::high_resolution_clock::now();
        res_3 = code.MarkovAlgorithm(3, false);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "McCreight's algorithm algorithm:\t" <<
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

        std::cout << std::endl;

        if (res_0 == res_1 && res_0 == res_2 && res_0 == res_3) {
            std::cout << "The results match\n";
            if (res_0) {
                std::cout << "The code is one-to-one\n";
            }
            else {
                std::cout << "The code is not one-to-one\n";
            }
        }
        else {
            std::cout << "The results don't match\n";
        }
    }

    return 0;
}