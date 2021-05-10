// Copyright 2021 Yasakova Anastasia

#include <gtest/gtest.h>
#include <chrono>
#include "OneToOneCode.h"
#include "SuffixTree.h"



TEST(TestSuffixTree, Test_on_time) {
    SuffixTree st_1, st_2;
    std::string text, word;
    GenerateTextAndWord(10, 3, text, word);

    std::cout << text << std::endl;
    std::cout << word << std::endl;

    // auto start = std::chrono::high_resolution_clock::now();
    // st_1.construct_1(text);
    // auto end = std::chrono::high_resolution_clock::now();
    // std::cout <<"Ukkonen's algorithm:\t" <<
    //     std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    st_2.construct_2(text);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout <<"McCreight's algorithm:\t" <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

    EXPECT_TRUE(true);
}
