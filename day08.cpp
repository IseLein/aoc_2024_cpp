
#include <bits/stdc++.h>

using namespace std;

void part1();
void part2();

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day08", "r", stdin);

    part2();
}

void print_vec(vector<pair<int, int>> v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << "(" << v[i].first << ", " << v[i].second << ")";
    }
    cout << "]\n";
}

void print_map(vector<vector<char>> map) {
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            cout << map[i][j];
        }
        cout << "\n";
    }
}

void print_antenna_locations(unordered_map<char, vector<pair<int, int>>> antenna_locations) {
    for (auto& i : antenna_locations) {
        cout << i.first << ": ";
        print_vec(i.second);
        cout << "\n";
    }
}

void part1() {
    set<pair<int, int>> anti_nodes;
    vector<vector<char>> map;
    unordered_map<char, vector<pair<int, int>>> antenna_locations;

    string line;
    int num = 0;
    while (getline(cin, line)) {
        vector<char> v;
        for (size_t i = 0; i < line.size(); i++) {
            v.push_back(line[i]);
            if (line[i] != '.') {
                if (antenna_locations.count(line[i]) == 0) {
                    antenna_locations[line[i]] = vector<pair<int, int>>();
                }
                antenna_locations[line[i]].push_back(make_pair(num, i));
            }
        }
        map.push_back(v);
        num++;
    }

    // print_map(map);
    // print_antenna_locations(antenna_locations);

    cout << map.size() << " x " << map[0].size() << "\n";

    for (auto& i : antenna_locations) {
        vector<pair<int, int>> v = i.second;

        for (size_t i = 0; i < v.size(); i++) {
            for (size_t j = i + 1; j < v.size(); j++) {
                pair<int, int> diff = make_pair(v[j].first - v[i].first, v[j].second - v[i].second);

                pair<int, int> a_node1 = make_pair(v[i].first - diff.first, v[i].second - diff.second);
                if (a_node1.first >= 0 && a_node1.first < (int)map.size() && a_node1.second >= 0 && a_node1.second < (int)map[0].size()) {
                    anti_nodes.insert(a_node1);
                }
                pair<int, int> a_node2 = make_pair(v[j].first + diff.first, v[j].second + diff.second);
                if (a_node2.first >= 0 && a_node2.first < (int)map.size() && a_node2.second >= 0 && a_node2.second < (int)map[0].size()) {
                    anti_nodes.insert(a_node2);
                }
            }
        }
    }
    cout << "Anti-nodes: " << anti_nodes.size() << "\n";
}

int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

void part2() {
    set<pair<int, int>> anti_nodes;
    vector<vector<char>> map;
    unordered_map<char, vector<pair<int, int>>> antenna_locations;

    string line;
    int num = 0;
    while (getline(cin, line)) {
        vector<char> v;
        for (size_t i = 0; i < line.size(); i++) {
            v.push_back(line[i]);
            if (line[i] != '.') {
                if (antenna_locations.count(line[i]) == 0) {
                    antenna_locations[line[i]] = vector<pair<int, int>>();
                }
                antenna_locations[line[i]].push_back(make_pair(num, i));
            }
        }
        map.push_back(v);
        num++;
    }

    // print_map(map);
    // print_antenna_locations(antenna_locations);

    cout << map.size() << " x " << map[0].size() << "\n";

    for (auto& i : antenna_locations) {
        vector<pair<int, int>> v = i.second;

        for (size_t i = 0; i < v.size(); i++) {
            for (size_t j = i + 1; j < v.size(); j++) {
                anti_nodes.insert(make_pair(v[i].first, v[i].second));
                anti_nodes.insert(make_pair(v[j].first, v[j].second));

                pair<int, int> diff = make_pair(v[j].first - v[i].first, v[j].second - v[i].second);

                int div = gcd(abs(diff.first), abs(diff.second));
                diff.first /= div;
                diff.second /= div;

                // go backwards from v[i] with diff
                pair<int, int> a_node1 = make_pair(v[i].first - diff.first, v[i].second - diff.second);
                while (a_node1.first >= 0 && a_node1.first < (int)map.size() && a_node1.second >= 0 && a_node1.second < (int)map[0].size()) {
                    anti_nodes.insert(a_node1);
                    a_node1 = make_pair(a_node1.first - diff.first, a_node1.second - diff.second);
                }

                // go forwards from v[i] with diff
                pair<int, int> a_node2 = make_pair(v[i].first + diff.first, v[i].second + diff.second);
                while (a_node2.first >= 0 && a_node2.first < (int)map.size() && a_node2.second >= 0 && a_node2.second < (int)map[0].size()) {
                    anti_nodes.insert(a_node2);
                    a_node2 = make_pair(a_node2.first + diff.first, a_node2.second + diff.second);
                }
            }
        }
    }
    cout << "Anti-nodes: " << anti_nodes.size() << "\n";
}
