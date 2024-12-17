
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const map<char, pair<int, int>> DIR_MAP = {
    {'^', {0, -1}},
    {'v', {0, 1}},
    {'<', {-1, 0}},
    {'>', {1, 0}},
};

void parseInput(ifstream &file, vector<vector<char>> &grid,
                vector<char> &moves, pair<int, int> &r_pos);
void parseInput2(ifstream &file, vector<vector<char>> &grid,
                vector<char> &moves, pair<int, int> &r_pos);
void moveRobot(vector<vector<char>> &grid, vector<char> moves, pair<int, int> &r_pos);
void moveRobot2(vector<vector<char>> &grid, vector<char> moves, pair<int, int> &r_pos);
bool moveRoBox(vector<vector<char>> &grid, pair<int, int> pos, pair<int, int> dir);
void moveRobot2(vector<vector<char>> &grid, vector<char> moves, pair<int, int> &r_pos);
long totalBoxGPS(vector<vector<char>> grid);
long totalBoxGPS2(vector<vector<char>> grid);

void print(vector<char> v);
void print(vector<vector<char>> v2);
string to_str(pair<int, int> p);
