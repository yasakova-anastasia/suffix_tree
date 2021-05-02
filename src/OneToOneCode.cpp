// Copyright 2021 Yasakova Anastasia

#include "OneToOneCode.h"
#include "SuffixTree.h"

#include <algorithm>
#include <stack>
#include <map>
#include <cstdlib>
#include <ctime>

void OneToOneCode::GenerateCode(int code_number, int symbol_number) {
    code.resize(code_number);
    std::srand(24);
    for (int i = 0; i < code_number; ++i) {
        int count = std::rand() % 9 + 1;
        std::string str = "";
        for (int j = 0; j < count; ++j) {
            str += std::to_string(std::rand() % symbol_number);
        }
        code[i] = str;
    }
}

void OneToOneCode::SetCode(std::vector<std::string> code_) {
    code = code_;
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
    /*std::cout << "KMP: Count = " << count << std::endl;
    std::cout << "fword\n";
    for (int i = 0; i < fword.size(); ++i) {
        std::cout << fword[i] << "  ";
    }
    std::cout << std::endl;
    std::cout << "f\n";
    for (int i = 0; i < f.size(); ++i) {
        std::cout << f[i] << "  ";
    }
    std::cout << std::endl;*/
    return f;
}

bool OneToOneCode::MarkovAlgorithm(int search_method) {
    VertexSearch();
    /*std::cout << "VertexSearch\n";
    for (int i = 0; i < vertex.size(); ++i) {
        std::cout << vertex[i] << " ";
    }
    std::cout << std::endl;*/

    BuildGraph(search_method);
    /*std::cout << "Build_Graph\n";
    for (int i = 0; i < comb.size(); ++i) {
        for (int j = 0; j < comb[i].size(); ++j) {
            if (comb[i][j] == "") {
                std::cout << "'' ";
            }
            else {
                std::cout << comb[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }*/
    ReBuildGraph();
    /*std::cout << "ReBuildGraph\n";
    for (int i = 0; i < comb.size(); ++i) {
        for (int j = 0; j < comb[i].size(); ++j) {
            if (comb[i][j] == "") {
                std::cout << "'' ";
            }
            else {
                std::cout << comb[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }*/

    return !IsCycle();
}

void OneToOneCode::VertexSearch() {
    for (int i = 0; i < code.size(); ++i) {
        if (code[i].size() > 1 && std::find(vertex.cbegin(),
                vertex.cend(), code[i]) == vertex.cend()) {
            for (int j = 0; j < code[i].size() - 1; ++j) {
                std::string v1 = code[i].substr(0, j + 1);
                if (std::find(vertex.cbegin(),
                        vertex.cend(), v1) == vertex.cend() &&
                    std::find(code.cbegin(), code.cend(), v1) == code.cend()) {
                    for (int k = 0; k < code.size(); ++k) {
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
    for (int i = 0; i < code.size(); ++i) {
        overlap_str f;
        if (search_method == 0) {
            f = GetOverlapKMP(code[i]);
        } else if (search_method == 1) {
            f = GetOverlapSTU(code[i]);
        } else {
            f = GetOverlapSTMK(code[i]);
        }

        for (int j = 0; j < code[i].size(); ++j) {
            std::string v1 = code[i].substr(0, j);
            if (std::find(vertex.cbegin(),
                    vertex.cend(), v1) == vertex.cend()) {
                continue;
            }
            for (int k = 0; k < code[i].size() - j + 1; ++k) {
                std::string v2 = code[i].substr(code[i].size() - k, k);
                if (std::find(vertex.cbegin(),
                        vertex.cend(), v2) == vertex.cend()) {
                    continue;
                }
                std::string preproc = code[i].substr(j, code[i].size() - k - j);
                std::vector<std::vector<std::string>> overlap(preproc.size());
                for (int l = 0; l < preproc.size(); ++l) {
                    for (int m = 0; m < f.size(); ++m) {
                        if (preproc.size() - l >= f[m].first.size()
                            && j + l + f[m].first.size() - 1
                                < f[m].second.size()
                            && f[m].second[j + l + f[m].first.size() - 1]
                                == f[m].first.size() - 1) {
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
                    if (pair.first == preproc.size()) {
                        std::vector<std::string> tmp;
                        tmp.push_back(v1);
                        for (int l = 0; l < pair.second.size(); ++l) {
                            tmp.push_back(pair.second[l]);
                        }
                        tmp.push_back(v2);
                        comb.push_back(tmp);
                        continue;
                    }
                    for (int l = 0; l < overlap[pair.first].size(); ++l) {
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
    for (int j = 0; j < code.size(); ++j) {
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
    for (int j = 0; j < code.size(); ++j) {
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
    for (int j = 0; j < code.size(); ++j) {
        if (str.size() > code[j].size()) {
            overlap.push_back(std::make_pair(code[j], SFT_KMP(code[j], str)));
        }
    }
    return overlap;
}

bool OneToOneCode::IsCycle() {
    bool res = false;
    std::stack<std::string> stack;
    std::map<std::string, int> vertex_color;
    std::string res_ = "";
    for (int i = 0; i < vertex.size(); ++i) {
        vertex_color.insert(std::pair<std::string, int>(vertex[i], -1));
    }

    stack.push("");
    while (!stack.empty()) {
        std::string curr_vertex = stack.top();
        stack.pop();
        if (vertex_color[curr_vertex] == -1) {
            vertex_color[curr_vertex] = 0;
        } else if (vertex_color[curr_vertex] == 0) {
            vertex_color[curr_vertex] = 1;
            if (curr_vertex == "") {
                res = true;
                res_ = curr_vertex;
                break;
            }
        }
        for (int j = 0; j < comb.size(); j++) {
            if (curr_vertex == comb[j][0] && vertex_color[comb[j][2]] != 1) {
                stack.push(comb[j][2]);
            }
        }
    }
    return res;
}

void OneToOneCode::ReBuildGraph() {
    std::vector<std::vector<std::string>> re_comb;
    for (int i = 0; i < comb.size(); ++i) {
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

std::vector<std::string> OneToOneCode::GetVertex() {
    return vertex;
}
