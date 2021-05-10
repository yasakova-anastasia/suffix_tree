// Copyright 2021 Yasakova Anastasia

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>
#include <utility>

#include "SuffixTree.h"
#include "Node.h"
#include "Suffix.h"

SuffixTree::SuffixTree() {
    internal_node_ID = 0;
    current_end = new int(0);
    root = new Node(NULL, 1, current_end, internal_node_ID);
}

void SuffixTree::construct_1(std::string s) {
    length = s.length();
    tree_string = s;

    (*current_end)++;
    last_leaf_extension = new Node(root, 1, current_end, 1);
    root->add_child(*this, last_leaf_extension);

    for (int i = 1; i < length; i++) {
        Suffix previous_suffix(last_leaf_extension, *current_end);

        (*current_end)++;

        // Explicitly compute successive extensions
        // starting at j(i) + 1 where (i)
        // is the ID of the last leaf extension from the previous phase.
        for (int j = (last_leaf_extension->ID + 1); j <= i + 1; j++) {
            Rule rule_applied;
            int begin_index, end_index;
            Node* origin = previous_suffix.walk_up(begin_index, end_index);
            Suffix suffix = (origin == root ? get_suffix(root, j, i)
                : get_suffix(origin->suffix_link, begin_index, end_index));

            if (suffix.RULE2_conditions(*this, i + 1)) {
                RULE2(suffix, i + 1, j);
                rule_applied = RULE_2;
            } else {
                rule_applied = RULE_3;
            }

            if (previous_suffix.new_internal_node)
                previous_suffix.node->suffix_link = suffix.node;

            previous_suffix = suffix;

            if (rule_applied == RULE_3)
                break;
        }
    }
}

void SuffixTree::construct_2(std::string s) {
    length = s.length();
    tree_string = s;

    (*current_end)++;
    int* tmp = new int(0);
    (*tmp) = length;
    last_leaf_extension = new Node(root, 1, tmp, *current_end);
    root->add_child(*this, last_leaf_extension);

    Suffix previous_suffix(last_leaf_extension, 1);

    Node* node_v_j = root;
    Node* w;
    int b_start = root->begin_index;
    int b_end = *root->end_index;
    int g_start = last_leaf_extension->begin_index;
    int g_end = *last_leaf_extension->end_index;

    for (int j = 1; j < length; ++j) {
        Node* current_leafnode;
        int I = 1;
        if (node_v_j == root) {
            ++g_start;
            I = 0;
        } else if (node_v_j->parent == root) {
            if (b_start == b_end) {
                I = 0;
            } else {
                ++b_start;
            }
        }


        if (I == 0) {
            int p = 0;
            Node* new_node_v = walkdown(root, g_start, g_end, p);
            (*current_end)++;
            current_leafnode = new Node(root, j, tmp, *current_end);
            current_leafnode->parent = new_node_v;
            current_leafnode->begin_index = g_start;
            new_node_v->add_child(*this, current_leafnode);
            if (b_start == b_end && b_start != -1) {
                node_v_j->suffix_link = root;
            }
        } else {
            w = jumpdown(node_v_j->parent->suffix_link, b_start, b_end);
            (*current_end)++;
            current_leafnode = new Node(root, j, tmp, *current_end);
            node_v_j->suffix_link = w;

            if (w->children.size() < 2) {
                current_leafnode->begin_index = g_start;
                current_leafnode->parent = w;
                w->add_child(*this, current_leafnode);
            } else {
                int p = 0;
                Node* new_node_v;
                new_node_v = walkdown(w, g_start, g_end, p);
                current_leafnode->parent = new_node_v;
                current_leafnode->begin_index = g_start;
                new_node_v->add_child(*this, current_leafnode);
            }
        }
        g_start = current_leafnode->begin_index;
        g_end = *current_leafnode->end_index;
        b_start = current_leafnode->parent->begin_index;
        b_end = *current_leafnode->parent->end_index;
        node_v_j = current_leafnode->parent;
    }
}

Node* SuffixTree::walkdown(Node* node, int& g_start, int& g_end, int& p) {
    Node* current = node;
    Node* return_node = NULL;
    while (g_start <= g_end) {
        Node* child = NULL;
        int i = 0;
        for (auto& item : current->children) {
            child = item.second;
            if (item.second == NULL ||
                    tree_string[item.second->begin_index - 1]
                    == tree_string[g_start - 1]) {
                break;
            }
            ++i;
            child = NULL;
        }
        if (!child) {
            p = i;
            return_node = current;
            break;
        } else {
            int intermediate = g_start + (*child->end_index
                - child->begin_index);
            int dist = 0;
            while (tree_string[child->begin_index + dist - 1]
                    == tree_string[g_start - 1]
                    && g_start <= std::min(intermediate, g_end)) {
                ++dist;
                ++g_start;
            }
            if (g_start > std::min(intermediate, g_end)) {
                current = child;
            } else {
                int* tmp = new int(0);
                (*tmp) = child->begin_index + dist - 1;
                (*current_end)++;
                Node* new_node_v = new Node(current,
                    child->begin_index, tmp, *current_end);
                current->remove_child(*this, child);
                child->begin_index = child->begin_index + dist;
                child->parent = new_node_v;
                p = 1;
                new_node_v->add_child(*this, child);
                current->add_child(*this, new_node_v);
                return_node = new_node_v;
                break;
            }
        }
    }
    return return_node;
}

Node* SuffixTree::jumpdown(Node* node, int& b_start, int& b_end) {
    Node* current = node;
    while (b_start <= b_end) {
        int i = 0;
        Node* child = NULL;
        for (auto& item : current->children) {
            child = item.second;
            if (tree_string[item.second->begin_index - 1]
                    == tree_string[b_start - 1]) {
                break;
            }
            ++i;
        }
        current = child;
        b_start = b_start + (*current->end_index - current->begin_index) + 1;
    }
    if (b_start == b_end + 1) {
        return current;
    } else {
        (*current_end)++;
        int* tmp = new int(0);
        (*tmp) = *current->end_index - (b_start - b_end) + 1;
        Node* w = new Node(current->parent, current->begin_index,
            tmp, *current_end);
        current->parent->remove_child(*this, current);
        current->parent->add_child(*this, w);
        current->parent = w;
        current->begin_index = *w->end_index + 1;
        w->add_child(*this, current);
        return w;
    }
}

// The 'skip/count' trick for suffix tree traversal (Gusfield, 1997)
Suffix SuffixTree::get_suffix(Node* origin, int begin_index, int end_index) {
    int char_index = *origin->end_index;

    while (begin_index <= end_index) {
        origin = origin->get_child(*this, begin_index);
        if (origin->edge_length() < end_index - begin_index + 1) {
            char_index = *origin->end_index;
        } else {
            char_index = origin->begin_index + (end_index - begin_index);
        }
        begin_index += origin->edge_length();
    }
    return Suffix(origin, char_index);
}

std::string SuffixTree::get_substr(int start_pos, int end_pos) {
    if (start_pos > end_pos) {
        return std::string();
    }
    // This is 1-indexed to match the algorithm's original description in the
    // paper. For example, "foobar".get_substr(2, 4) == "oob".
    return tree_string.substr(start_pos - 1, end_pos - start_pos + 1);
}

char SuffixTree::get_char_at_index(int index) const {
    // Also 1-indexed. For example, "foobar".get_char_at_index(4) == 'b'
    return tree_string[index - 1];
}

void SuffixTree::RULE2(Suffix& suffix, int char_index, int new_leaf_ID) {
    if (!suffix.ends_at_node()) {  // eg. in case 2 (path ends inside an edge)
        suffix.node->split_edge(*this, suffix.char_index, --internal_node_ID);
        suffix.node = suffix.node->parent;
        suffix.new_internal_node = true;
    }
    Node* new_leaf = new Node(suffix.node, char_index,
        current_end, new_leaf_ID);
    suffix.node->add_child(*this, new_leaf);
    last_leaf_extension = new_leaf;
}

std::string SuffixTree::log_tree() {
    return "digraph g{\n" + log_node(root) + "}";
}

std::string SuffixTree::log_node(Node* parent) {
    std::stringstream buffer;

    // Internal nodes (nodes with ID <= 0) are unlabelled points, leaves
    // (nodes with ID > 0) show the ID as plaintext.
    buffer << parent->ID << "[shape="
        << ((parent->ID <= 0) ? "point" : "plaintext") << "];\n";

    for (auto it = parent->children.begin();
            it != parent->children.end(); ++it) {
        // Child nodes are stored on the parent node in a map of integers
        // (it->first) to Node pointers (it->second).
        Node* child_node = it->second;
        buffer << parent->ID << "->" << child_node->ID << " [label = \""
            << get_substr(child_node->begin_index, *(child_node->end_index))
            << "\"];\n" << log_node(child_node);
    }

    // Print the suffx link, if there is one.
    Node* suffix_link = parent->suffix_link;
    if (suffix_link) {
        buffer << "\"" << parent->ID << "\" -> " << "\""
            << suffix_link->ID << "\" [style=dashed arrowhead=otriangle];\n";
    }

    return buffer.str();
}

std::vector<int> SuffixTree::findsubstr(std::string str) {
    std::vector<int> f(tree_string.size(), -1);
    size_t count = 0;
    Node* curr = root;
    bool flag = true;
    std::string prev_str = "";
    while (curr != NULL && flag) {
        if (curr != root) {
            prev_str += get_substr(curr->begin_index, *curr->end_index);
        }
        if (curr->children.size() > 0) {
            for (auto it = curr->children.begin();
                    it != curr->children.end(); it++) {
                Node* child = it->second;
                std::string child_str = get_substr(child->begin_index,
                    *child->end_index);
                size_t j = count;
                for (size_t i = 0; i < std::min(str.size(),
                        child_str.size()); ++i) {
                    if (str[i + count] == child_str[i]) {
                        ++j;
                    } else {
                        break;
                    }
                }
                if (j > count) {
                    count = j;
                    curr = child;
                    if (count == str.size()) {
                        flag = false;
                    }
                    break;
                }
            }
            break;
        } else {
            break;
        }
    }

    if (count == str.size()) {
        std::stack<std::pair<Node*, std::string>> stack;
        std::vector<std::string> matching_str;
        std::string curr_str = prev_str;
        stack.push(std::make_pair(curr, curr_str));
        while (!stack.empty()) {
            std::pair<Node*, std::string> curr_pair = stack.top();
            stack.pop();
            curr = curr_pair.first;
            curr_str = curr_pair.second +
                get_substr(curr->begin_index, *curr->end_index);
            if (curr->children.size() > 0) {
                for (auto it = curr->children.begin();
                        it != curr->children.end(); it++) {
                    Node* child = it->second;
                    stack.push(std::make_pair(child, curr_str));
                }
            } else {
                matching_str.push_back(curr_str);
            }
        }

        for (size_t i = 0; i < matching_str.size(); ++i) {
            int index = tree_string.size() - matching_str[i].size();
            int counter = 0;
            for (size_t j = index; j < index + str.size(); ++j) {
                f[j] = counter;
                ++counter;
            }
        }
    }

    return f;
}

void GenerateTextAndWord(int n, int m,
        std::string& text, std::string& word) {
    std::srand(24);
    for (int i = 0; i < n; ++i) {
        int tmp = std::rand() % 57 + 65;
        text += static_cast<char>(tmp);
    }
    for (int i = 0; i < m; ++i) {
        int tmp = std::rand() % 57 + 65;
        word += static_cast<char>(tmp);
    }
}