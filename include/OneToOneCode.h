// Copyright 2021 Yasakova Anastasia

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <utility>

typedef std::vector<std::pair<std::string, std::vector<int>>> overlap_str;

class OneToOneCode {
 private:
    std::vector<std::string> code;
    std::vector<std::string> vertex;
    std::vector<std::vector<std::string>> comb;
    int alphabet_power;

 public:
    OneToOneCode() : alphabet_power(0) {}
    void GenerateCode(int code_number, int symbol_number, int min, int max);
    // code_number is number of words
    // symbol_number is number of symbols in alphabet
    void SetCode(std::vector<std::string> code_, int symbol_number);
    void PrintCode();

    bool checking_for_matches(std::string str);
    float Macmillan_inequality(std::string str);
    float Macmillan_inequality();

    bool MarkovAlgorithm(int search_method, bool print = false);  // true - code is one-to-one

    void VertexSearch();
    void BuildGraph(int search_method);  // search for edges in a graph
    overlap_str GetOverlapSTU(std::string str);
    overlap_str GetOverlapSTMK(std::string str);
    overlap_str GetOverlapKMP(std::string str);
    overlap_str GetOverlap(std::string str);
    std::vector<int> SimpleAlg(std::string word, std::string text);
    std::vector<int> KMP(std::string word);
    std::vector<int> SFT_KMP(std::string word, std::string text);

    std::vector<std::string> GetVertex();

    void ReBuildGraph();
    bool IsCycle(bool print = false);

    std::string get_edge(std::string v1, std::string v2);
    std::vector<std::string> get_words(std::string str);
};

