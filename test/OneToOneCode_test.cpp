// Copyright 2021 Yasakova Anastasia

#include <gtest/gtest.h>
// #include <chrono>
#include "OneToOneCode.h"

TEST(TestOneToOneCode, Test_Vertex_Search) {
    std::vector<std::string> target_vertex = { "0", "01", "10", "" };
    std::vector<std::string> words = { "1", "010", "101" };
    OneToOneCode code;
    code.SetCode(words);

    code.VertexSearch();
    std::vector<std::string> vertex = code.GetVertex();

    EXPECT_EQ(target_vertex, vertex);
}

TEST(TestOneToOneCode, Test_with_KMP_1) {
    std::vector<std::string> words = { "10", "01", "12", "012",
                                       "2100", "12011", "12010" };
    OneToOneCode code;
    code.SetCode(words);

    EXPECT_FALSE(code.MarkovAlgorithm(0));
}

TEST(TestOneToOneCode, Test_with_ST_Ukkonen_1) {
    std::vector<std::string> words = { "10", "01", "12", "012",
                                       "2100", "12011", "12010" };
    OneToOneCode code;
    code.SetCode(words);

    EXPECT_FALSE(code.MarkovAlgorithm(1));
}

TEST(TestOneToOneCode, Test_with_ST_McCreight_1) {
    std::vector<std::string> words = { "10", "01", "12", "012",
                                       "2100", "12011", "12010" };
    OneToOneCode code;
    code.SetCode(words);

    EXPECT_FALSE(code.MarkovAlgorithm(2));
}

TEST(TestOneToOneCode, Test_with_KMP_2) {
    std::vector<std::string> words = { "1", "01", "100", "0100", "0000" };
    OneToOneCode code;
    code.SetCode(words);

    EXPECT_TRUE(code.MarkovAlgorithm(0));
}

TEST(TestOneToOneCode, Test_with_ST_Ukkonen_2) {
    std::vector<std::string> words = { "1", "01", "100", "0100", "0000" };
    OneToOneCode code;
    code.SetCode(words);

    EXPECT_TRUE(code.MarkovAlgorithm(1));
}

TEST(TestOneToOneCode, Test_with_ST_McCreight_2) {
    std::vector<std::string> words = { "1", "01", "100", "0100", "0000" };
    OneToOneCode code;
    code.SetCode(words);

    EXPECT_TRUE(code.MarkovAlgorithm(2));
}

// Otrher samples:
// "1", "010", "101" - false
// "10", "12", "012", "101", "2100" - false
// "a", "ab", "cab", "baac" - false

TEST(TestOneToOneCode, Test_with_KMP_and_ST_Ukkonen) {
    OneToOneCode code;
    code.GenerateCode(10, 5);

    EXPECT_EQ(code.MarkovAlgorithm(0), code.MarkovAlgorithm(1));
}

TEST(TestOneToOneCode, Test_with_KMP_and_ST_McCreight) {
    OneToOneCode code;
    code.GenerateCode(10, 5);

    EXPECT_EQ(code.MarkovAlgorithm(0), code.MarkovAlgorithm(2));
}

TEST(TestOneToOneCode, Test_with_ST_Ukkonen_and_ST_McCreight) {
    OneToOneCode code;
    code.GenerateCode(10, 5);

    EXPECT_EQ(code.MarkovAlgorithm(1), code.MarkovAlgorithm(2));
}

/*TEST(TestOneToOneCode, Test_on_time) {
    OneToOneCode code;
    code.GenerateCode(50, 2);

    auto start = std::chrono::high_resolution_clock::now();
    code.MarkovAlgorithm(0);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout <<"KMP algorithm:\t" <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    code.MarkovAlgorithm(1);
    end = std::chrono::high_resolution_clock::now();
    std::cout <<"Ukkonen's algorithm:\t" <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    code.MarkovAlgorithm(2);
    end = std::chrono::high_resolution_clock::now();
    std::cout <<"McCreight's algorithm:\t" <<
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

    EXPECT_TRUE(true);
}*/
