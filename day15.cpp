
#include "day15.h"

void print(vector<char> v) {
    cout << "[ ";
    for (auto &c: v) {
        cout << c << ", ";
    }
    cout << "]\n";
}

void print(vector<vector<char>> v2) {
    for (auto &v: v2) {
        for (auto &c: v) {
            cout << c;
        }
        cout << "\n";
    }
}

string to_str(pair<int, int> p) {
    return "( " + to_string(p.first) + ", " + to_string(p.second) + " )";
}

// commented out lines are for Part 1
// or cout's
int l_main() {
    ifstream file("input/day15");

    vector<vector<char>> grid;
    vector<char> moves;
    pair<int, int> r_pos;
    // parseInput(file, grid, moves, r_pos);
    parseInput2(file, grid, moves, r_pos);

    // print(grid);
    // cout << "Robot at " << to_str(r_pos) << ";\n";

    // moveRobot(grid, moves, r_pos);
    moveRobot2(grid, moves, r_pos);

    // print(grid);
    // cout << "Robot at " << to_str(r_pos) << ";\n";

    // cout << "Total Box GPS: " << totalBoxGPS(grid) << ";\n";
    cout << "Total Box GPS: " << totalBoxGPS2(grid) << ";\n";
    return 0;
}

bool moveRoBox(vector<vector<char>> &grid, pair<int, int> pos, pair<int, int> dir) {
    // expect pos to be in bounds because of the explicit '#' boundary
    pair<int, int> new_pos = make_pair(pos.first + dir.first, pos.second + dir.second);
    if (grid[new_pos.second][new_pos.first] == '.') {
        swap(grid[new_pos.second][new_pos.first], grid[pos.second][pos.first]);
        return true;
    } else if (grid[new_pos.second][new_pos.first] == '#') {
        return false;
    }

    if (moveRoBox(grid, new_pos, dir)) {
        swap(grid[new_pos.second][new_pos.first], grid[pos.second][pos.first]);
        return true;
    }
    return false;
}

void moveRobot(vector<vector<char>> &grid, vector<char> moves, pair<int, int> &r_pos) {
    for (auto &move: moves) {
        pair<int, int> dir = DIR_MAP.at(move);
        if(moveRoBox(grid, r_pos, dir)) {
            r_pos = make_pair(r_pos.first + dir.first, r_pos.second + dir.second);
        }
    }
}

bool moveRoBox2(vector<vector<char>> &grid, pair<int, int> pos, pair<int, int> dir) {
    // expect pos to be in bounds because of the explicit '##' boundary
    pair<int, int> new_pos = make_pair(pos.first + dir.first, pos.second + dir.second);
    if (grid[new_pos.second][new_pos.first] == '.') {
        swap(grid[new_pos.second][new_pos.first], grid[pos.second][pos.first]);
        return true;
    } else if (grid[new_pos.second][new_pos.first] == '#') {
        return false;
    }

    // moving horizontally
    if (dir.second == 0) {
        new_pos = make_pair(pos.first + (2 * dir.first), pos.second);
        if (moveRoBox(grid, new_pos, dir)) {
            // '.[]' <-> '[].'
            swap(grid[new_pos.second][new_pos.first], grid[pos.second][pos.first + dir.first]);
            swap(grid[pos.second][pos.first + dir.first], grid[pos.second][pos.first]);
            return true;
        }
    }

    // moving vertically
    if (dir.first == 0) {
        vector<vector<char>> b_grid(grid);
        pair<int, int> new_pos2;
        if (grid[new_pos.second][new_pos.first] == '[') {
            new_pos2 = make_pair(new_pos.first + 1, new_pos.second);
        } else {
            new_pos2 = make_pair(new_pos.first - 1, new_pos.second);
        }
        if (moveRoBox2(grid, new_pos, dir) && moveRoBox2(grid, new_pos2, dir)) {
            swap(grid[new_pos.second][new_pos.first], grid[pos.second][pos.first]);
            swap(grid[new_pos2.second][new_pos2.first], grid[new_pos2.second][new_pos2.first - dir.first]);
            return true;
        } else {
            grid = b_grid;
        }
    }

    return false;
}

void moveRobot2(vector<vector<char>> &grid, vector<char> moves, pair<int, int> &r_pos) {
    for (auto &move: moves) {
        pair<int, int> dir = DIR_MAP.at(move);
        if(moveRoBox2(grid, r_pos, dir)) {
            r_pos = make_pair(r_pos.first + dir.first, r_pos.second + dir.second);
        }
    }
}

long totalBoxGPS(vector<vector<char>> grid) {
    long totalGPS = 0;
    for (int i = 0; i < (int)grid.size(); i++) {
        for(int j = 0; j < (int)grid[0].size(); j++) {
            if (grid[i][j] == 'O') {
                totalGPS += (100 * i) + j;
            }
        }
    }
    return totalGPS;
}

long totalBoxGPS2(vector<vector<char>> grid) {
    long totalGPS = 0;
    for (int i = 0; i < (int)grid.size(); i++) {
        for(int j = 0; j < (int)grid[0].size(); j++) {
            if (grid[i][j] == '[') {
                totalGPS += (100 * i) + j;
            }
        }
    }
    return totalGPS;
}


void parseInput(ifstream &file, vector<vector<char>> &grid,
                vector<char> &moves, pair<int, int> &r_pos) {
    string line;

    getline(file, line);
    int num = 0;
    while (line != "") {
        vector<char> v;
        for (int i = 0; i < (int)line.size(); i++) {
            v.push_back(line[i]);
            if (line[i] == '@') {
                r_pos = make_pair(i, num);
            }
        }
        grid.push_back(v);

        getline(file, line);
        num++;
    }

    while(getline(file, line)) {
        for (auto &c: line) {
            moves.push_back(c);
        }
    }
}

void parseInput2(ifstream &file, vector<vector<char>> &grid,
                vector<char> &moves, pair<int, int> &r_pos) {
    string line;

    getline(file, line);
    int num = 0;
    while (line != "") {
        vector<char> v;
        for (int i = 0; i < (int)line.size(); i++) {
            if (line[i] == '@') {
                r_pos = make_pair(i * 2, num);
                v.push_back('@');
                v.push_back('.');
            } else if (line[i] == 'O') {
                v.push_back('[');
                v.push_back(']');
            } else {
                v.push_back(line[i]);
                v.push_back(line[i]);
            }
        }
        grid.push_back(v);

        getline(file, line);
        num++;
    }

    while(getline(file, line)) {
        for (auto &c: line) {
            moves.push_back(c);
        }
    }
}
