
#include <bits/stdc++.h>
#include <unordered_map>

#define ull unsigned long long

using namespace std;

void part1();
void part1_strings();
void part1_rec();
ull num_stones(ull num, int depth, unordered_map<string, ull> &cache);

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day11", "r", stdin);

    // part1_strings();
    // part1();
    part1_rec();
}

string print_vec(vector<unsigned long long> v) {
    string s = "[";
    for (size_t i = 0; i < v.size(); i++) {
        s += to_string(v[i]);
        if (i != v.size() - 1) {
            s += ", ";
        }
    }
    s += "]";
    return s;
}

void part1() {
    vector<unsigned long long> v;

    unsigned long long n;
    while (cin >> n) {
        v.push_back(n);
    }

    cout << print_vec(v) << "\n";
    for (int i = 0; i < 45; i++) {
        size_t len = v.size();
        cout << "len @ " << i << ": " << len << "\n";
        for (size_t j = 0; j < v.size(); j++) {
            cout << " j : " << j  << "< " << v.size() << " @ " << i << "\n";
            if (v[j] == 0) {
                v[j] = 1;
            } else if ((to_string(v[j]).size() % 2) == 0) {
                string s = to_string(v[j]);
                v[j] = stoull(s.substr(0, s.size() / 2));
                v.insert(v.begin() + j + 1, stoull(s.substr(s.size() / 2)));
                j++;
            } else {
                v[j] *= 2024;
            }
        }
        // cout << print_vec(v) << "\n";
        cout << "Step " << i << ": " << v.size() << "\n";
    }

    cout << "Total: " << v.size() << "\n";
}

string print_vec(vector<string> v) {
    string s = "[";
    for (size_t i = 0; i < v.size(); i++) {
        s += v[i];
        if (i != v.size() - 1) {
            s += ", ";
        }
    }
    s += "]";
    return s;
}

void trim_left(string &s, char c) {
    size_t i = 0;
    while (s[i] == c) {
        i++;
    }
    s.erase(0, i);
}

void part1_strings() {
    vector<string> v;

    string s;
    while (cin >> s) {
        v.push_back(s);
    }

    // cout << print_vec(v) << "\n";
    for (int i = 0; i < 25; i++) {
        // size_t len = v.size();
        for (size_t j = 0; j < v.size(); j++) {
            if (v[j] == "" || v[j] == "0") {
                v[j] = "1";
            } else if ((v[j].size() % 2) == 0) {
                string s = v[j];
                v[j] = s.substr(0, s.size() / 2);
                v.insert(v.begin() + j + 1, s.substr(s.size() / 2));
                j++;
                trim_left(v[j], '0');
            } else {
                v[j] = to_string(stoull(v[j]) * 2024);
            }
        }
        // cout << print_vec(v) << "\n";
    }
    // cout << "Total: " << v.size() << "\n";
}

ull num_stones(ull num, int depth, unordered_map<string, ull> &cache) {
    if (depth == 0) {
        return 1;
    }
    string key = to_string(num) + "_" + to_string(depth);

    if (num == 0) {
        if (cache.count(key) == 0) {
            ull new_num = num_stones(1, depth - 1, cache);
            cache[key] = new_num;
            return new_num;
        }
        return cache[key];
    } else if (to_string(num).size() % 2 == 0) {
        if (cache.count(key) == 0) {
            string s = to_string(num);
            ull left = num_stones(stoull(s.substr(0, s.size() / 2)), depth - 1, cache);
            ull right = num_stones(stoull(s.substr(s.size() / 2)), depth - 1, cache);
            ull new_num = left + right;
            cache[key] = new_num;
            return new_num;
        }
        return cache[key];
    }
    if (cache.count(key) == 0) {
        ull new_num = num_stones(num * 2024, depth - 1, cache);
        cache[key] = new_num;
        return new_num;
    }
    return cache[key];
}

void print_cache(unordered_map<string, ull> &cache) {
    cout << "{";
    for (auto &i : cache) {
        cout << i.first << " -> " << i.second << ", ";
    }
    cout << "}";
}

void part1_rec() {
    vector<ull> v;

    ull n;
    while (cin >> n) {
        v.push_back(n);
    }

    unordered_map<string, ull> cache;

    ull total = 0;
    for (auto num : v) {
        ull stones = num_stones(num, 75, cache);
        total += stones;
    }
    cout << "Total: " << total << "\n";
}

