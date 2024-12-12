
#include <bits/stdc++.h>

using namespace std;

void part1();
void part2();

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day04", "r", stdin);

    part2();
}

void part1() {
    vector<vector<char>> v;
    string line;
    int total = 0;
    while (getline(cin, line)) {
        vector<char> vi;
        for (size_t i = 0; i < line.size(); i++) {
            vi.push_back(line[i]);
        }
        v.push_back(vi);
    }

    for (int i = 0; i < (int)v.size(); i++) {
        for (int j = 0; j < (int)v[i].size(); j++) {
            if (v[i][j] == 'X') {
                // look down
                if ((i + 3 < v.size()) && (v[i + 1][j] == 'M') && (v[i + 2][j] == 'A') && (v[i + 3][j] == 'S')) {
                    total++;
                }
                // look up
                if ((i - 3 >= 0) && (v[i - 1][j] == 'M') && (v[i - 2][j] == 'A') && (v[i - 3][j] == 'S')) {
                    total++;
                }
                // look right
                if ((j + 3 < v[i].size()) && (v[i][j + 1] == 'M') && (v[i][j + 2] == 'A') && (v[i][j + 3] == 'S')) {
                    total++;
                }
                // look left
                if ((j - 3 >= 0) && (v[i][j - 1] == 'M') && (v[i][j - 2] == 'A') && (v[i][j - 3] == 'S')) {
                    total++;
                }
                // look diagonally
                if ((i + 3 < v.size()) && (j + 3 < v[i].size()) && (v[i + 1][j + 1] == 'M') && (v[i + 2][j + 2] == 'A') && (v[i + 3][j + 3] == 'S')) {
                    total++;
                }
                if ((i - 3 >= 0) && (j - 3 >= 0) && (v[i - 1][j - 1] == 'M') && (v[i - 2][j - 2] == 'A') && (v[i - 3][j - 3] == 'S')) {
                    total++;
                }
                // reverse diagonally
                if ((i + 3 < v.size()) && (j - 3 >= 0) && (v[i + 1][j - 1] == 'M') && (v[i + 2][j - 2] == 'A') && (v[i + 3][j - 3] == 'S')) {
                    total++;
                }
                if ((i - 3 >= 0) && (j + 3 < v[i].size()) && (v[i - 1][j + 1] == 'M') && (v[i - 2][j + 2] == 'A') && (v[i - 3][j + 3] == 'S')) {
                    total++;
                }
            }
        }
    }
    cout << "Total: " << total << "\n";
}

void part2() {
    vector<vector<char>> v;
    string line;
    int total = 0;
    while (getline(cin, line)) {
        vector<char> vi;
        for (size_t i = 0; i < line.size(); i++) {
            vi.push_back(line[i]);
        }
        v.push_back(vi);
    }

    for (int i = 0; i < (int)v.size(); i++) {
        for (int j = 0; j < (int)v[i].size(); j++) {
            if (v[i][j] == 'A') {
                bool diag_1a = (i - 1 >= 0) && (j - 1 >= 0) && (v[i - 1][j - 1] == 'M') && (i + 1 < v.size()) && (j + 1 < v[i].size()) && (v[i + 1][j + 1] == 'S');
                bool diag_1b = (i - 1 >= 0) && (j - 1 >= 0) && (v[i - 1][j - 1] == 'S') && (i + 1 < v.size()) && (j + 1 < v[i].size()) && (v[i + 1][j + 1] == 'M');
                bool diag_2a = (i - 1 >= 0) && (j + 1 < v[i].size()) && (v[i - 1][j + 1] == 'M') && (i + 1 < v.size()) && (j - 1 >= 0) && (v[i + 1][j - 1] == 'S');
                bool diag_2b = (i - 1 >= 0) && (j + 1 < v[i].size()) && (v[i - 1][j + 1] == 'S') && (i + 1 < v.size()) && (j - 1 >= 0) && (v[i + 1][j - 1] == 'M');

                if ((diag_1a || diag_1b) && (diag_2a || diag_2b)) {
                    total++;
                }
            }
        }
    }
    cout << "Total: " << total << "\n";
}

