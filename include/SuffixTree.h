// Copyright 2021 Yasakova Anastasia

#pragma once
#include <string>
#include <vector>

class Suffix;
class Node;
class SuffixTree {
 public:
    SuffixTree();
    void construct_1(std::string); // Ukkonen's algorithm
    void construct_2(std::string); // McCreight's algorithm
    std::string log_tree(); // for suffix tree print
    char get_char_at_index(int) const;
    std::vector<int> findsubstr(std::string str);

 private:
    std::string tree_string;
    std::string log_node(Node* parent);
    std::string get_substr(int, int);

    enum Rule { RULE_2, RULE_3 };

    Node* walkdown(Node*, int&, int&, int&); // for McCreight's algorithm
    Node* jumpdown(Node*, int&, int&); // for McCreight's algorithm

    Suffix get_suffix(Node*, int, int);

    void RULE2(Suffix&, int, int);

    Node* root;
    int internal_node_ID;
    int length;
    int* current_end;
    Node* last_leaf_extension;
};

void GenerateTextAndWord(int n, int m,
        std::string& text, std::string& word);
