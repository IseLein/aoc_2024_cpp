
#include <bits/stdc++.h>

using namespace std;

void part1();
void part2();
void split(string str, vector<string> &v, char delim);

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day05", "r", stdin);

    part2();
}

void split(string str, vector<string> &v, char delim) {
    string token = "";
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == delim) {
            v.push_back(token);
            token = "";
            continue;
        }
        token += str[i];
    }
    v.push_back(token);
}

void part1() {
    unordered_map<int, vector<int>> a_map;
    unordered_map<int, vector<int>> b_map;
    string line;
    int a, b;

    getline(cin, line);
    while (line != "") {
        size_t pos = line.find('|');
        a = stoi(line.substr(0, pos));
        b = stoi(line.substr(pos + 1));

        if (a_map.count(a) > 0) {
            a_map[a].push_back(b);
        } else {
            vector<int> v;
            v.push_back(b);
            a_map[a] = v;
        }
        if (b_map.count(b) > 0) {
            b_map[b].push_back(a);
        } else {
            vector<int> v;
            v.push_back(a);
            b_map[b] = v;
        }
        getline(cin, line);
    }

    int total = 0;
    while(getline(cin, line)) {
        vector<string> v;
        split(line, v, ',');

        vector<int> vi;
        for (size_t i = 0; i < v.size(); i++) {
            vi.push_back(stoi(v[i]));
        }

        bool is_fine = true;
        for (size_t i = 0; i < vi.size(); i++) {
            if (!is_fine) {
                break;
            }
            for (size_t j = i + 1; j < vi.size(); j++) {
                if (a_map.count(vi[j]) > 0) {
                    if (count(a_map[vi[j]].begin(), a_map[vi[j]].end(), vi[i]) > 0) {
                        is_fine = false;
                        break;
                    }
                }
            }
        }
        if (is_fine) {
            size_t num = floor(vi.size() / 2.0);
            // cout << "Line: " << line << "; adding " << vi[num] << "\n";
            total += vi[num];
        }
    }
    cout << "Total: " << total << "\n";
}

pair<int, int> check_order(vector<int> vi, unordered_map<int, vector<int>> &a_map) {
    for (size_t i = 0; i < vi.size(); i++) {
        for (size_t j = i + 1; j < vi.size(); j++) {
            if (a_map.count(vi[j]) > 0) {
                if (count(a_map[vi[j]].begin(), a_map[vi[j]].end(), vi[i]) > 0) {
                    return make_pair(i, j);
                }
            }
        }
    }
    return make_pair(-1, -1);
}

void part2() {
    unordered_map<int, vector<int>> a_map;
    unordered_map<int, vector<int>> b_map;
    string line;
    int a, b;

    getline(cin, line);
    while (line != "") {
        size_t pos = line.find('|');
        a = stoi(line.substr(0, pos));
        b = stoi(line.substr(pos + 1));

        if (a_map.count(a) > 0) {
            a_map[a].push_back(b);
        } else {
            vector<int> v;
            v.push_back(b);
            a_map[a] = v;
        }
        if (b_map.count(b) > 0) {
            b_map[b].push_back(a);
        } else {
            vector<int> v;
            v.push_back(a);
            b_map[b] = v;
        }
        getline(cin, line);
    }

    int total = 0;
    while(getline(cin, line)) {
        vector<string> v;
        split(line, v, ',');

        vector<int> vi;
        for (size_t i = 0; i < v.size(); i++) {
            vi.push_back(stoi(v[i]));
        }

        pair<int, int> failing = check_order(vi, a_map);
        if(check_order(vi, a_map).first == -1) {
            continue;
        }
        while(failing.first != -1) {
            swap(vi[failing.first], vi[failing.second]);
            failing = check_order(vi, a_map);
        }
        size_t num = floor(vi.size() / 2.0);
        // cout << "Line: " << line << "; adding " << vi[num] << "\n";
        total += vi[num];
    }
    cout << "Total: " << total << "\n";
}
