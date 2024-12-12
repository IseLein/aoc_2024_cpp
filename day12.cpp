
#include "day12.h"

using namespace std;

int l_main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day12", "r", stdin);

    part1();
    return 0;
}

string print_grid(vector<vector<char>> &grid) {
    string s = "";
    for (auto line: grid) {
        for (auto ch: line) {
            s += ch;
        }
        s += "\n";
    }
    s += "\n";
    return s;
}

void get_region(pair<int, int> pos, set<pair<int, int>> &region, vector<vector<char>> &grid) {
    region.insert(make_pair(pos.first, pos.second));

    char plant_type = grid[pos.first][pos.second];

    pair<int, int> new_pos;
    // up
    if ((pos.first > 0) && (grid[pos.first - 1][pos.second] == plant_type)) {
        new_pos = make_pair(pos.first - 1, pos.second);
        if (region.count(new_pos) == 0) {
            get_region(new_pos, region, grid);
        }
    }
    // down
    if ((pos.first < (int)grid.size() - 1) && (grid[pos.first +  1][pos.second] == plant_type)) {
        new_pos = make_pair(pos.first + 1, pos.second);
        if (region.count(new_pos) == 0) {
            get_region(new_pos, region, grid);
        }
    }
    // left
    if ((pos.second > 0) && (grid[pos.first][pos.second - 1] == plant_type)) {
        new_pos = make_pair(pos.first, pos.second - 1);
        if (region.count(new_pos) == 0) {
            get_region(new_pos, region, grid);
        }
    }
    // right
    if ((pos.second < (int)grid[0].size() - 1) && (grid[pos.first][pos.second + 1] == plant_type)) {
        new_pos = make_pair(pos.first, pos.second + 1);
        if (region.count(new_pos) == 0) {
            get_region(new_pos, region, grid);
        }
    }
}

pair<int, int> fence_price(set<pair<int, int>> &region, vector<vector<char>> &grid) {
    int area = region.size();
    int perimeter = 0;
    for (auto p: region) {
        // up
        if ((p.first == 0) || (region.count(make_pair(p.first - 1, p.second)) == 0)) {
            perimeter += 1;
        }
        // down
        if ((p.first == (int)grid.size() - 1) || (region.count(make_pair(p.first + 1, p.second)) == 0)) {
            perimeter += 1;
        }
        // left
        if ((p.second == 0) || (region.count(make_pair(p.first, p.second - 1)) == 0)) {
            perimeter += 1;
        }
        // right
        if ((p.second == (int)grid[0].size() - 1) || (region.count(make_pair(p.first, p.second + 1)) == 0)) {
            perimeter += 1;
        }
    }

    return make_pair(area, perimeter);
}

pair<int, int> fence_price2(set<pair<int, int>> &region, vector<vector<char>> &grid) {
    int area = region.size();

    map<SIDE, vector<pair<int, int>>> p_sides;
    for (auto p: region) {
        // up
        if ((p.first == 0) || (region.count(make_pair(p.first - 1, p.second)) == 0)) {
            if (p_sides.count(UP) == 0) {
                vector<pair<int, int>> v;
                p_sides[UP] = v;
            }
            p_sides[UP].push_back(p);
        }
        // down
        if ((p.first == (int)grid.size() - 1) || (region.count(make_pair(p.first + 1, p.second)) == 0)) {
            if (p_sides.count(DOWN) == 0) {
                vector<pair<int, int>> v;
                p_sides[DOWN] = v;
            }
            p_sides[DOWN].push_back(p);
        }
        // left
        if ((p.second == 0) || (region.count(make_pair(p.first, p.second - 1)) == 0)) {
            if (p_sides.count(LEFT) == 0) {
                vector<pair<int, int>> v;
                p_sides[LEFT] = v;
            }
            p_sides[LEFT].push_back(p);
        }
        // right
        if ((p.second == (int)grid[0].size() - 1) || (region.count(make_pair(p.first, p.second + 1)) == 0)) {
            if (p_sides.count(RIGHT) == 0) {
                vector<pair<int, int>> v;
                p_sides[RIGHT] = v;
            }
            p_sides[RIGHT].push_back(p);
        }
    }

    int num_sides = 0;
    for (auto p: p_sides) {
        // sort the part sides nicely so that all sides end up 'properly arranged'
        // in the vector. might be completely unneccesary because of how the vector is
        // populated but a nice to have anyways
        sort(p.second.begin(), p.second.end(),
             [p](pair<int, int> a, pair<int, int> b) {
                if ((p.first == UP) || (p.first == DOWN)) {
                    if (a.first == b.first) {
                        return a.second < b.second;
                    }
                    return a.first < b.first;
                } else {
                    if (a.second == b.second) {
                        return a.first < b.first;
                    }
                    return a.second < b.second;
                }
             });

        vector<pair<int, int>> v = p.second;
        int s_line = -1;
        int s_pos = -1;
        int s_side_len = 0;
        for (int i = 0; i < (int)v.size(); i++) {
            int curr_line = (p.first == UP) || (p.first == DOWN) ? v[i].first : v[i].second;
            int curr_pos = (p.first == UP) || (p.first == DOWN) ? v[i].second : v[i].first;
            if ((s_side_len != 0) && ((s_line != curr_line) || (curr_pos != s_pos + 1))) {
                num_sides += 1;
                s_side_len = 1;
            } else {
                s_side_len += 1;
            }
            s_line = curr_line;
            s_pos = curr_pos;
        }
        if (s_side_len > 0) {
            // probably always true
            num_sides += 1;
        }
    }

    return make_pair(area, num_sides);
}

tuple<int, int, int, vector<vector<Fence>>> fence_price_viz(set<pair<int, int>> &region, vector<vector<char>> &grid) {
    int area = region.size();

    map<SIDE, vector<pair<int, int>>> p_sides;
    for (auto p: region) {
        // up
        if ((p.first == 0) || (region.count(make_pair(p.first - 1, p.second)) == 0)) {
            if (p_sides.count(UP) == 0) {
                vector<pair<int, int>> v;
                p_sides[UP] = v;
            }
            p_sides[UP].push_back(p);
        }
        // down
        if ((p.first == (int)grid.size() - 1) || (region.count(make_pair(p.first + 1, p.second)) == 0)) {
            if (p_sides.count(DOWN) == 0) {
                vector<pair<int, int>> v;
                p_sides[DOWN] = v;
            }
            p_sides[DOWN].push_back(p);
        }
        // left
        if ((p.second == 0) || (region.count(make_pair(p.first, p.second - 1)) == 0)) {
            if (p_sides.count(LEFT) == 0) {
                vector<pair<int, int>> v;
                p_sides[LEFT] = v;
            }
            p_sides[LEFT].push_back(p);
        }
        // right
        if ((p.second == (int)grid[0].size() - 1) || (region.count(make_pair(p.first, p.second + 1)) == 0)) {
            if (p_sides.count(RIGHT) == 0) {
                vector<pair<int, int>> v;
                p_sides[RIGHT] = v;
            }
            p_sides[RIGHT].push_back(p);
        }
    }

    int num_sides = 0;
    int num_fences = 0;
    vector<vector<Fence>> fences;
    for (auto p: p_sides) {
        // sort the part sides nicely so that all sides end up 'properly arranged'
        // in the vector. might be completely unneccesary because of how the vector is
        // populated but a nice to have anyways
        sort(p.second.begin(), p.second.end(),
             [p](pair<int, int> a, pair<int, int> b) {
                if ((p.first == UP) || (p.first == DOWN)) {
                    if (a.first == b.first) {
                        return a.second < b.second;
                    }
                    return a.first < b.first;
                } else {
                    if (a.second == b.second) {
                        return a.first < b.first;
                    }
                    return a.second < b.second;
                }
             });

        vector<pair<int, int>> v = p.second;
        int s_line = -1;
        int s_pos = -1;
        int s_side_len = 0;
        vector<Fence> f;
        for (int i = 0; i < (int)v.size(); i++) {
            int curr_line = (p.first == UP) || (p.first == DOWN) ? v[i].first : v[i].second;
            int curr_pos = (p.first == UP) || (p.first == DOWN) ? v[i].second : v[i].first;
            if ((s_side_len != 0) && ((s_line != curr_line) || (curr_pos != s_pos + 1))) {
                num_sides += 1;
                s_side_len = 1;
                fences.push_back(f);
                f.assign(1, { p.first, { v[i].first, v[i].second } });
            } else {
                s_side_len += 1;
                f.push_back({ p.first, { v[i].first, v[i].second } });
            }
            s_line = curr_line;
            s_pos = curr_pos;
            num_fences += 1;
        }
        if (s_side_len > 0) {
            // probably always true
            num_sides += 1;
            fences.push_back(f);
        }
    }

    return make_tuple(area, num_sides, num_fences, fences);
}

vector<set<pair<int, int>>> get_regions(vector<vector<char>> &grid) {
    vector<set<pair<int, int>>> regions;
    set<pair<int, int>> seen_plots;
    for (int i = 0; i < (int)grid.size(); i++) {
        for (int j = 0; j < (int)grid[0].size(); j++) {
            pair<int, int> pos = make_pair(i, j);
            if (seen_plots.count(pos) == 0) {
                set<pair<int, int>> region;
                get_region(pos, region, grid);
                regions.push_back(set(region));
                seen_plots.merge(region);
            }
        }
    }
    return regions;
}

void part1() {
    vector<vector<char>> grid;

    string line;
    while (getline(cin, line)) {
        vector<char> row;
        for (auto i: line) {
            row.push_back(i);
        }
        grid.push_back(row);
    }

    // cout << "Grid: \n" << print_grid(grid);

    int total_price = 0;
    set<pair<int, int>> seen_plots;
    for (int i = 0; i < (int)grid.size(); i++) {
        for (int j = 0; j < (int)grid[0].size(); j++) {
            pair<int, int> pos = make_pair(i, j);
            if (seen_plots.count(pos) == 0) {
                set<pair<int, int>> region;
                get_region(pos, region, grid);

                pair<int, int> area_perimeter = fence_price2(region, grid);
                int price = area_perimeter.first * area_perimeter.second;
                total_price += price;
                seen_plots.merge(region);
            }
        }
    }

    cout << "Total price: " << total_price;
}
