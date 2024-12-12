
#include <bits/stdc++.h>

using namespace std;

void part1();
void part2();
bool is_safe(vector<int> vi);
void split(string str, vector<string> &v, char delim);

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day02", "r", stdin);

    // part1();
    part2();
}

bool is_safe(vector<int> vi) {
    bool is_safe = true;
    bool is_increasing = vi[1] > vi[0];
    for (size_t i = 0; i < vi.size() - 1; i++) {
        int diff = vi[i + 1] - vi[i];
        if (is_increasing && ((diff < 1) || (diff > 3))) {
            is_safe = false;
            break;
        }
        if (!is_increasing && ((diff > -1) || (diff < -3))) {
            is_safe = false;
            break;
        }
    }
    return is_safe;
}

void part1() {
    int total = 0;
    int num_lines = 0;
    string line;
    while (getline(cin, line)) {
        vector<string> v;
        split(line, v, ' ');
        vector<int> vi;
        num_lines++;
        for (size_t i = 0; i < v.size(); i++) {
            vi.push_back(stoi(v[i]));
        }

        cout << num_lines << ". Line: " << line << "; Values: ";
        for (size_t i = 0; i < vi.size(); i++) {
            cout << vi[i] << ", ";
        }
        cout << "\n";
        bool safe = is_safe(vi);
        if (safe) {
            total++;
        }
        // cout << "Line: " << line << "; Safety: " << is_safe << ".\n";
    }
    cout << "Total: " << total << "\n";
}

void part2() {
    int total = 0;
    int num_lines = 0;
    string line;
    while (getline(cin, line)) {
        vector<string> v;
        split(line, v, ' ');
        vector<int> vi;
        num_lines++;
        for (size_t i = 0; i < v.size(); i++) {
            vi.push_back(stoi(v[i]));
        }

        // cout << num_lines << ". Line: " << line << "; Values: ";
        // for (int i = 0; i < vi.size(); i++) {
        //     cout << vi[i] << ", ";
        // }
        // cout << "\n";
        bool safe = is_safe(vi);
        if (!safe) {
            bool safe2 = false;
            for (size_t i = 0; i < vi.size(); i++) {
                vector<int> vi2(vi);
                vi2.erase(vi2.begin() + i);
                safe2 = is_safe(vi2);
                if (safe2) {
                    break;
                }
            }
            if (safe2) {
                safe = true;
            }
        }
        if (safe) {
            total++;
        }
        // cout << "Line: " << line << "; Safety: " << safe << ".\n";
    }
    cout << "Total: " << total << "\n";
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
