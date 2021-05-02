// Copyright 2021 Yasakova Anastasia

#pragma once
#include <string>
#include <vector>
#include <map>

class SuffixTree;

class Node {
    friend class SuffixTree;
    friend class Suffix;

 private:
    Node(Node* parent, int begin_index, int* end_index, int ID);
    int edge_length() { return *end_index - begin_index + 1; }

    void add_child(const SuffixTree&, Node*);
    void remove_child(const SuffixTree&, Node*);
    Node* get_child(const SuffixTree&, int char_index);

    bool is_leaf() { return children.empty(); }
    void split_edge(const SuffixTree&, int, int);

    int get_key(const SuffixTree&, Node*, int) const;

    Node* parent;
    std::map<int, Node*> children;
    std::vector<int> labels;
    Node* suffix_link;
    int begin_index;
    int* end_index;
    int ID;
};
