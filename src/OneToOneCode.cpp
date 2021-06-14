// Copyright 2021 Yasakova Anastasia

#include "OneToOneCode.h"
#include "SuffixTree.h"

#include <algorithm>
#include <stack>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>


void OneToOneCode::GenerateCode(int code_number, int symbol_number, int min, int max) {
    alphabet_power = symbol_number;
    std::srand(24);
    while(code.size() < code_number) {
        int count = std::rand() % (max - min + 1) + min;
        std::string str = "";
        for (int j = 0; j < count; ++j) {
            str += std::to_string(std::rand() % symbol_number);
        }
        if (checking_for_matches(str) && Macmillan_inequality(str) <= 1) {
            code.push_back(str);
        }
    }
}

void OneToOneCode::SetCode(std::vector<std::string> code_, int symbol_number) {
    alphabet_power = symbol_number;
    code = code_;
}

void OneToOneCode::PrintCode() {
    std::cout << "V = { ";
    for (int i = 0; i < code.size(); ++i) {
        if (i != 0) {
            std::cout << ", ";
        }
        std::cout << code[i];
    }
    std::cout << " }" << std::endl << std::endl;
}

bool OneToOneCode::checking_for_matches(std::string str) {
    for (int i = 0; i < code.size(); ++i) {
        if (code[i] == str) {
            return false;
        }
    }
    return true;
}

float OneToOneCode::Macmillan_inequality(std::string str) {
    float sum = 0.0;
    for (int i = 0; i < code.size(); ++i) {
        sum += pow(alphabet_power, (-1) * static_cast<int>(code[i].size()));
    }
    sum += pow(alphabet_power, (-1) * static_cast<int>(str.size()));
    return sum;
}

float OneToOneCode::Macmillan_inequality() {
    float sum = 0.0;
    for (int i = 0; i < code.size(); ++i) {
        sum += pow(alphabet_power, (-1) * static_cast<int>(code[i].size()));
    };
    return sum;
}

std::vector<int> OneToOneCode::SimpleAlg(std::string word, std::string text) {
    int n = word.length(),
        m = text.length();
    std::vector <int> f(m, 0);
    int s;
    for (int i = 0; i < m; i++) {
        s = 0;
        while (word[s] == text[i + s] && s < n) {
            s++;
        }
        if (s == n) {
            f[i + n - 1] = n;
        }
    }
    return f;
}

std::vector<int> OneToOneCode::KMP(std::string word) {
    int n = word.length();
    std::vector <int> f(n, -1);
    int i = 0;
    int j = 0;
    while (i < n - 1) {
        j = f[i];
        while (word[i + 1] != word[j + 1] && j > -1) {
            j = f[j];
        }
        if (word[i + 1] == word[j + 1]) {
            f[i + 1] = j + 1;
        }
        i++;
    }
    return f;
}

std::vector<int> OneToOneCode::SFT_KMP(std::string word, std::string text) {
    int count = 0;
    int n = word.length(),
        m = text.length();
    std::vector <int> f(m, -1);
    std::vector <int> fword = KMP(word);
    int i = 0, j;
    if (word[0] == text[0]) {
        f[0] = 0;
    }
    while (i < m - 1) {
        j = f[i];
        if (j == n - 1) {
            j = fword[j];
        }
        while (word[j + 1] != text[i + 1] && j > -1) {
            j = fword[j];
        }
        if (word[j + 1] == text[i + 1]) {
            f[i + 1] = j + 1;
            if (f[i + 1] == n - 1) {
                count++;
            }
        }
        i++;
    }
    return f;
}

bool OneToOneCode::MarkovAlgorithm(int search_method, bool print) {
    VertexSearch();
    if (print) {
        std::cout << "Graph vertices: S = { ";
        for (int i = 0; i < vertex.size(); ++i) {
            if (i != 0) {
                std::cout << ", ";
            }
            if (vertex[i] == "") {
                std::cout << "''";
            } else {
                std::cout << vertex[i];
            }
        }
        std::cout << " }\n\n";
    }

    BuildGraph(search_method);
    ReBuildGraph();

    return !IsCycle(print);
}

void OneToOneCode::VertexSearch() {
    for (size_t i = 0; i < code.size(); ++i) {
        if (code[i].size() > 1 && std::find(vertex.cbegin(),
                vertex.cend(), code[i]) == vertex.cend()) {
            for (size_t j = 0; j < code[i].size() - 1; ++j) {
                std::string v1 = code[i].substr(0, j + 1);
                if (std::find(vertex.cbegin(),
                        vertex.cend(), v1) == vertex.cend() &&
                    std::find(code.cbegin(), code.cend(), v1) == code.cend()) {
                    for (size_t k = 0; k < code.size(); ++k) {
                        if (code[k].size() > v1.size()) {
                            std::string v2 = code[k].substr(code[k].size()
                                - v1.size(), code[k].size());
                            if (v1 == v2) {
                                vertex.push_back(v1);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    vertex.push_back("");
}

void OneToOneCode::BuildGraph(int search_method) {
    for (size_t i = 0; i < code.size(); ++i) {
        overlap_str f;
        if (search_method == 0) {
            f = GetOverlap(code[i]);
        } else if (search_method == 1) {
            f = GetOverlapKMP(code[i]);
        } else if (search_method == 2) {
            f = GetOverlapSTU(code[i]);
        } else {
            f = GetOverlapSTMK(code[i]);
        }

        for (size_t j = 0; j < code[i].size(); ++j) {
            std::string v1 = code[i].substr(0, j);
            if (std::find(vertex.cbegin(),
                    vertex.cend(), v1) == vertex.cend()) {
                continue;
            }
            for (size_t k = 0; k < code[i].size() - j + 1; ++k) {
                std::string v2 = code[i].substr(code[i].size() - k, k);
                if (std::find(vertex.cbegin(),
                        vertex.cend(), v2) == vertex.cend()) {
                    continue;
                }
                std::string preproc = code[i].substr(j, code[i].size() - k - j);
                std::vector<std::vector<std::string>> overlap(preproc.size());
                for (size_t l = 0; l < preproc.size(); ++l) {
                    for (size_t m = 0; m < f.size(); ++m) {
                        if (preproc.size() - l >= f[m].first.size()
                            && j + l + f[m].first.size() - 1
                                < f[m].second.size()
                            && f[m].second[j + l + f[m].first.size() - 1]
                                == static_cast<int>(f[m].first.size() - 1)) {
                            overlap[l].push_back(f[m].first);
                        }
                    }
                    if (overlap[l].empty()) {
                        break;
                    }
                }
                std::stack<std::pair<int, std::vector<std::string>>> stack;
                std::vector<std::string> v_;
                stack.push(std::make_pair(0, v_));
                while (!stack.empty()) {
                    auto pair = stack.top();
                    stack.pop();
                    if (pair.first == static_cast<int>(preproc.size())) {
                        std::vector<std::string> tmp;
                        tmp.push_back(v1);
                        for (size_t l = 0; l < pair.second.size(); ++l) {
                            tmp.push_back(pair.second[l]);
                        }
                        tmp.push_back(v2);
                        comb.push_back(tmp);
                        continue;
                    }
                    for (size_t l = 0; l < overlap[pair.first].size(); ++l) {
                        std::vector<std::string> v_ = pair.second;
                        v_.push_back(overlap[pair.first][l]);
                        stack.push(std::make_pair(pair.first +
                            overlap[pair.first][l].size(), v_));
                    }
                }
            }
        }
    }
}

overlap_str OneToOneCode::GetOverlapSTU(std::string str) {
    overlap_str overlap;
    SuffixTree st;
    bool isbuild = false;
    for (size_t j = 0; j < code.size(); ++j) {
        if (str.size() > code[j].size()) {
            if (!isbuild) {
                st.construct_1(str + "$");
                isbuild = true;
            }
            std::vector<int> findstr = st.findsubstr(code[j]);
            findstr.pop_back();
            overlap.push_back(std::make_pair(code[j], findstr));
        }
    }
    return overlap;
}

overlap_str OneToOneCode::GetOverlapSTMK(std::string str) {
    overlap_str overlap;
    SuffixTree st;
    bool isbuild = false;
    for (size_t j = 0; j < code.size(); ++j) {
        if (str.size() > code[j].size()) {
            if (!isbuild) {
                st.construct_2(str + "$");
                isbuild = true;
            }
            std::vector<int> findstr = st.findsubstr(code[j]);
            findstr.pop_back();
            overlap.push_back(std::make_pair(code[j], findstr));
        }
    }
    return overlap;
}

overlap_str OneToOneCode::GetOverlapKMP(std::string str) {
    overlap_str overlap;
    for (size_t j = 0; j < code.size(); ++j) {
        if (str.size() > code[j].size()) {
            overlap.push_back(std::make_pair(code[j], SFT_KMP(code[j], str)));
        }
    }
    return overlap;
}

overlap_str OneToOneCode::GetOverlap(std::string str) {
    overlap_str overlap;
    for (size_t j = 0; j < code.size(); ++j) {
        if (str.size() > code[j].size()) {
            overlap.push_back(std::make_pair(code[j], SimpleAlg(code[j], str)));
        }
    }
    return overlap;
}

bool OneToOneCode::IsCycle(bool print) {
    bool res = false;
    std::stack<std::string> stack_for_find, stack_for_cycle;
    std::map<std::string, int> vertex_color;
    for (size_t i = 0; i < vertex.size(); ++i) {
        vertex_color.insert(std::pair<std::string, int>(vertex[i], -1));
    }

    stack_for_find.push("");
    while (!stack_for_find.empty()) {
        std::string curr_vertex = stack_for_find.top();
        stack_for_find.pop();
        if (vertex_color[curr_vertex] == -1) {
            vertex_color[curr_vertex] = 0;
            stack_for_cycle.push(curr_vertex);
        } else if (vertex_color[curr_vertex] == 0) {
            vertex_color[curr_vertex] = 1;
            if (curr_vertex == "") {
                if (print) {
                    std::vector<std::string> cycle;
                    std::string prev = "";
                    while (!stack_for_cycle.empty()) {
                        std::string curr = stack_for_cycle.top();
                        std::string edge = get_edge(curr, prev);
                        if (edge != "$") {
                            cycle.push_back(edge);
                            if (curr != "") {
                                cycle.push_back(curr);
                            }
                        }
                        if (curr == "") {
                            break;
                        }
                        prev = curr;
                        stack_for_cycle.pop();
                    }
                    std::reverse(cycle.begin(), cycle.end());
                    std::string str = "";
                    std::cout << "Cycle: ";
                    for (int i = 0; i < cycle.size(); ++i) {
                        if (cycle[i] == "") {
                            std::cout << "''  ";
                        } else {
                            std::cout << cycle[i] << "  ";
                        }
                        str += cycle[i];
                    }
                    std::cout << std::endl;
                    std::vector<std::string> words = get_words(str);
                    std::cout << "Possible combinations: " << std::endl;
                    for (int i = 0; i < words.size(); ++i) {
                        std::cout << "   " << words[i] << std::endl;
                    }
                    std::cout << std::endl;
                }
                
                res = true;
                break;
            }
        }
        for (size_t j = 0; j < comb.size(); j++) {
            if (curr_vertex == comb[j][0] && vertex_color[comb[j][2]] != 1) {
                stack_for_find.push(comb[j][2]);
            }
        }
    }
    return res;
}

void OneToOneCode::ReBuildGraph() {
    std::vector<std::vector<std::string>> re_comb;
    for (size_t i = 0; i < comb.size(); ++i) {
        if (comb[i].size() == 3) {
            if (comb[i][0] != comb[i][2])
                re_comb.push_back(comb[i]);
        } else if (comb[i].size() == 2) {
            std::vector<std::string> tmp = { comb[i][0], "", comb[i][1] };
            if (comb[i][0] != comb[i][1])
                re_comb.push_back(tmp);
        } else {
            std::vector<std::string> tmp1 = { comb[i][0], comb[i][1], "" };
            if (comb[i][0] != "")
                re_comb.push_back(tmp1);
            std::vector<std::string> tmp3 = { "", comb[i][comb[i].size() - 2],
                comb[i][comb[i].size() - 1] };
            if ("" != comb[i][comb[i].size() - 1])
                re_comb.push_back(tmp3);
        }
    }
    comb = re_comb;
}

std::string OneToOneCode::get_edge(std::string v1, std::string v2) {
    for (int i = 0; i < comb.size(); ++i) {
        if (comb[i][0] == v1 && comb[i][2] == v2) {
            return comb[i][1];
        }
    }
    return "$";
}

std::vector<std::string> OneToOneCode::get_words(std::string str) {
    std::vector<std::string> words;
    overlap_str f = GetOverlapKMP(str);
    std::vector<std::vector<std::string>> overlap(str.size());
    for (size_t l = 0; l < str.size(); ++l) {
        for (size_t m = 0; m < f.size(); ++m) {
            if (str.size() - l >= f[m].first.size()
                && l + f[m].first.size() - 1
                < f[m].second.size()
                && f[m].second[l + f[m].first.size() - 1]
                == static_cast<int>(f[m].first.size() - 1)) {
                overlap[l].push_back(f[m].first);
            }
        }
    }
    std::stack<std::pair<int, std::vector<std::string>>> stack;
    std::vector<std::string> v_;
    stack.push(std::make_pair(0, v_));
    while (!stack.empty()) {
        auto pair = stack.top();
        stack.pop();
        if (pair.first == static_cast<int>(str.size())) {
            std::string tmp = "";
            for (size_t l = 0; l < pair.second.size(); ++l) {
                if (l != 0) {
                    tmp += "  ";
                }
                tmp += pair.second[l];
            }
            words.push_back(tmp);
            continue;
        }
        for (size_t l = 0; l < overlap[pair.first].size(); ++l) {
            std::vector<std::string> v_ = pair.second;
            v_.push_back(overlap[pair.first][l]);
            stack.push(std::make_pair(pair.first +
                overlap[pair.first][l].size(), v_));
        }
    }
    return words;
}

std::vector<std::string> OneToOneCode::GetVertex() {
    return vertex;
}
