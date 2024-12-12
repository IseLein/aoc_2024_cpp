
#include <bits/stdc++.h>
#include <string>

using namespace std;

void part1();
void part2();

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day06", "r", stdin);

    part2();
}

void print_vec(vector<pair<int, int>> v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << "(" << v[i].first << ", " << v[i].second << ")";
    }
    cout << "]\n";
}

void part1(vector<vector<char>> &mmap, set<pair<int, int>> &loop_set, pair<int, int> &sstart) {
    vector<vector<char>> map;
    pair<int, int> start;

    string line;
    int num = 0;
    while (getline(cin, line)) {
        vector<char> v;
        for (size_t i = 0; i < line.size(); i++) {
            v.push_back(line[i]);
            if (line[i] == '^') {
                start = make_pair(num, i);
            }
        }
        map.push_back(v);
        num++;
    }
    mmap.assign(map.begin(), map.end());
    sstart = make_pair(start.first, start.second);

    cout << "start: " << start.first << ", " << start.second << "\n";

    pair<int, int> dir = make_pair(-1, 0);
    while (true) {
        int new_x = start.second + dir.second;
        int new_y = start.first + dir.first;
        if (new_x < 0 || new_x >= (int)map[0].size() || new_y < 0 || new_y >= (int)map.size()) {
            loop_set.insert(start);
            break;
        }
        if (map[new_y][new_x] == '#') {
            // turn right 90 degrees
            dir = make_pair(dir.second, -dir.first);
            continue;
        }
        loop_set.insert(start);
        start = make_pair(new_y, new_x);
    }

    loop_set.insert(start);

    cout << "Num points: " << loop_set.size() << "\n\n";
}

int get_axis_pos(pair<int, int> position, pair<int, int> direction) {
    if (direction.first == 0) {
        return position.first;
    }
    return position.second;
}

void print_map(vector<vector<char>> map) {
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            cout << map[i][j];
        }
        cout << "\n";
    }
}

void part2() {
    vector<vector<char>> map;
    set<pair<int, int>> loop_set;
    pair<int, int> init_pos;
    part1(map, loop_set, init_pos);

    int total = 0;
    vector<pair<int, int>> obs_pos;
    set<pair<int, int>> obs_set;


    for (auto& p : loop_set) {
        map[p.first][p.second] = '#';

        pair<int, int> curr_pos = make_pair(init_pos.first, init_pos.second);
        set<tuple<string, int, int>> seen_points;

        pair<int, int> dir = make_pair(-1, 0);
        while (true) {
            tuple<string, int, int> curr_point = make_tuple(to_string(dir.first) + "_" + to_string(dir.second), curr_pos.first, curr_pos.second);
            if (seen_points.count(curr_point) > 0) {
                total++;
                obs_pos.push_back(make_pair(p.first, p.second));
                obs_set.insert(make_pair(p.first, p.second));
                break;
            }
            seen_points.insert(curr_point);

            // mark as visited
            map[curr_pos.first][curr_pos.second] = 'X';

            // next position
            int new_y = curr_pos.first + dir.first;
            int new_x = curr_pos.second + dir.second;

            // next position out of bounds
            if (new_x < 0 || new_x >= (int)map[0].size() || new_y < 0 || new_y >= (int)map.size()) {
                break;
            }

            // obstacle
            if (map[new_y][new_x] == '#') {
                // turn right 90 degrees
                dir = make_pair(dir.second, -dir.first);
                continue;
            }


            curr_pos = make_pair(new_y, new_x);
        }

        map[p.first][p.second] = '.';
    }

    cout << "Total: " << total << ";\n";
    cout << "Vector: " << obs_pos.size() << ";\n";
    cout << "Set: " << obs_set.size() << ";\n";
}
