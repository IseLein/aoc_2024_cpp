
#include <bits/stdc++.h>

using namespace std;

void part1();
void part2();

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day10_2t", "r", stdin);

    part2();
}

string key(int x, int y) {
    return to_string(x) + "_" + to_string(y);
}

void print_dp(unordered_map<string, set<pair<int, int>>> dp, int x, int y, vector<vector<int>> map) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            cout << map[i][j] << ":";
            cout << " {";
            for (auto& p : dp[key(i, j)]) {
                cout << "(" << p.first << "," << p.second << ")";
            }
            cout << "} ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void part1() {
    vector<vector<int>> map;
    vector<pair<int, int>> trailheads;
    vector<pair<int, int>> peaks;

    unordered_map<string, set<pair<int, int>>> dp;

    string line;
    int num = 0;
    while (getline(cin, line)) {
        vector<int> row;
        for (int i = 0; i < (int)line.size(); i++) {
            int c = line[i] - '0';
            row.push_back(c);
            if (c == 9) {
                peaks.push_back(make_pair(num, i));
            } else if (c == 0) {
                trailheads.push_back(make_pair(num, i));
            }
            dp[key(num, i)] = set<pair<int, int>>();
        }
        num++;
        map.push_back(row);
    }

    for (auto& peak : peaks) {
        dp[key(peak.first, peak.second)].insert(peak);
    }

    for (int c = 8; c >= 0; c--) {
        for (int i = 0; i < (int)map.size(); i++) {
            for (int j = 0; j < (int)map[i].size(); j++) {
                if (map[i][j] == c) {
                    set<pair<int, int>> temp;
                    if ((i > 0) && (map[i - 1][j] == c + 1)) {
                        temp.insert(dp[key(i - 1, j)].begin(), dp[key(i - 1, j)].end());
                    }
                    if ((j > 0) && (map[i][j - 1] == c + 1)) {
                        temp.insert(dp[key(i, j - 1)].begin(), dp[key(i, j - 1)].end());
                    }
                    if ((i < (int)map.size() - 1) && (map[i + 1][j] == c + 1)) {
                        temp.insert(dp[key(i + 1, j)].begin(), dp[key(i + 1, j)].end());
                    }
                    if ((j < (int)map[i].size() - 1) && (map[i][j + 1] == c + 1)) {
                        temp.insert(dp[key(i, j + 1)].begin(), dp[key(i, j + 1)].end());
                    }
                    dp[key(i, j)] = temp;
                }
            }
        }
        // print_dp(dp, map.size(), map[0].size(), map);
    }

    int count = 0;
    for (auto& trailhead : trailheads) {
        // cout << "Reachable 9's from (" << trailhead.first << "," << trailhead.second << "): ";
        // for (auto& p : dp[key(trailhead.first, trailhead.second)]) {
        //     cout << "(" << p.first << "," << p.second << ") ";
        // }
        // cout << "\n";
        // cout << "Count: " << dp[key(trailhead.first, trailhead.second)].size() << "\n";
        count += dp[key(trailhead.first, trailhead.second)].size();
    }
    cout << "Total count: " << count << "\n";
}

void part2() {
    vector<vector<int>> map;
    vector<pair<int, int>> trailheads;
    vector<pair<int, int>> peaks;

    unordered_map<string, int> dp;

    string line;
    int num = 0;
    while (getline(cin, line)) {
        vector<int> row;
        for (int i = 0; i < (int)line.size(); i++) {
            int c = line[i] - '0';
            row.push_back(c);
            if (c == 9) {
                peaks.push_back(make_pair(num, i));
            } else if (c == 0) {
                trailheads.push_back(make_pair(num, i));
            }
            dp[key(num, i)] = 0;
        }
        num++;
        map.push_back(row);
    }

    for (auto& peak : peaks) {
        dp[key(peak.first, peak.second)] = 1;
    }

    for (int c = 8; c >= 0; c--) {
        for (int i = 0; i < (int)map.size(); i++) {
            for (int j = 0; j < (int)map[i].size(); j++) {
                if (map[i][j] == c) {
                    int temp = 0;
                    if ((i > 0) && (map[i - 1][j] == c + 1)) {
                        temp += dp[key(i - 1, j)];
                    }
                    if ((j > 0) && (map[i][j - 1] == c + 1)) {
                        temp += dp[key(i, j - 1)];
                    }
                    if ((i < (int)map.size() - 1) && (map[i + 1][j] == c + 1)) {
                        temp += dp[key(i + 1, j)];
                    }
                    if ((j < (int)map[i].size() - 1) && (map[i][j + 1] == c + 1)) {
                        temp += dp[key(i, j + 1)];
                    }
                    dp[key(i, j)] = temp;
                }
            }
        }
    }

    int count = 0;
    for (auto& trailhead : trailheads) {
        count += dp[key(trailhead.first, trailhead.second)];
    }
    cout << "Total count: " << count << "\n";
}
