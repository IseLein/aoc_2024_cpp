
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

using namespace std;

ostream& operator<<(ostream& os, const pair<int, int>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

struct Cheat {
    pair<int, int> start;
    pair<int, int> end;
};

ostream& operator<<(ostream& os, const Cheat& c) {
    os << "(" << c.start << " , " << c.end << ")";
    return os;
}

bool operator<(const Cheat& c1, const Cheat& c2) {
    return (c1.start < c2.start) || ((c1.start == c2.start) && (c1.end < c2.end));
}

void getPath(vector<vector<char>> &grid, pair<int, int> start, pair<int, int> end, vector<pair<int, int>> &path);
void getProbableCheats(vector<vector<char>> &grid, vector<Cheat> &cheats);

void print(vector<vector<char>> &grid) {
    for (auto &v: grid) {
        for (auto &c: v) {
            cout << c;
        }
        cout << "\n";
    }
}

int main() {
    vector<vector<char>> grid;
    pair<int, int> start;
    pair<int, int> end;

    ifstream fin("input/day20");
    string line;
    int num = 0;
    while (getline(fin, line)) {
        vector<char> v;
        for (int i = 0; i < (int)line.size(); i++) {
            v.push_back(line[i]);
            if (line[i] == 'S') {
                start = make_pair(i, num);
            } else if (line[i] == 'E') {
                end = make_pair(i, num);
            }
        }
        num++;
        grid.push_back(v);
    }

    vector<pair<int, int>> path;
    getPath(grid, start, end, path);
    vector<Cheat> pCheats;
    getProbableCheats(grid, pCheats);

    vector<Cheat> cheats;
    map<Cheat, int> cheatGain;
    for (auto &cheat: pCheats) {
        int startC = -1;
        int endC = -1;
        for (int i = 0; i < (int)path.size(); i++) {
            if ((startC != -1) && (endC != -1)) {
                break;
            }
            if (cheat.start == path[i]) {
                startC = i;
            }
            if (cheat.end == path[i]) {
                endC = i;
            }
        }
        int gain = abs(endC - startC) - 2;
        if (gain > 0) {
            cheatGain[cheat] = gain;
            cheats.push_back(cheat);
        }
    }

    map<int, vector<Cheat>> cheatMap;
    for (auto &c: cheats) {
        cheatMap[cheatGain[c]].push_back(c);
    }

    int num100 = 0;
    for (auto &c: cheats) {
        if (cheatGain[c] >= 100) {
            num100++;
        }
    }
    cout << "Total cheats: " << cheats.size() << "\n";
    cout << "Saving up to 100 picos: " << num100 << "\n";

    // Part 2
    int desiredGain = 100;
    int numCheats = 0;
    int numP1 = 0;
    map<int, int> cheatGain2;
    for (int i = 0; i < (int)path.size(); i++) {
        for (int j = i + desiredGain; j < (int)path.size(); j++) {
            int cheatTime = abs(path[j].first - path[i].first) + abs(path[j].second - path[i].second);
            int gain = j - i - cheatTime;
            if ((cheatTime <= 20) && (gain >= desiredGain)) {
                cheatGain2[gain]++;
                numCheats++;
                if (cheatTime == 2) {
                    numP1++;
                }
            }
        }
    }

    cout << "P1: " << numP1 << "\n";
    cout << "Total cheats: " << numCheats << "\n";
}

void getProbableCheats(vector<vector<char>> &grid, vector<Cheat> &cheats) {
    for (int i = 1; i < (int)grid.size() - 1; i++) {
        for (int j = 1; j < (int)grid[0].size() - 1; j++) {
            if (grid[i][j] == '#') {
                vector<pair<int, int>> neighbours;
                if (grid[i - 1][j] != '#') {
                    neighbours.push_back(make_pair(j, i - 1));
                }
                if (grid[i + 1][j] != '#') {
                    neighbours.push_back(make_pair(j, i + 1));
                }
                if (grid[i][j - 1] != '#') {
                    neighbours.push_back(make_pair(j - 1, i));
                }
                if (grid[i][j + 1] != '#') {
                    neighbours.push_back(make_pair(j + 1, i));
                }
                for (int a = 0; a < (int)neighbours.size(); a++) {
                    for (int b = a + 1; b < (int)neighbours.size(); b++) {
                        Cheat newCheat = {
                            .start = make_pair(neighbours[a].first, neighbours[a].second),
                            .end = make_pair(neighbours[b].first, neighbours[b].second)
                        };
                        cheats.push_back(newCheat);
                    }
                }
            }
        }
    }
}

void getPath(vector<vector<char>> &grid, pair<int, int> start, pair<int, int> end, vector<pair<int, int>> &path) {
    set<pair<int, int>> visited;
    pair<int, int> curr = start;

    while(curr != end) {
        path.push_back(curr);
        visited.insert(curr);

        vector<pair<int, int>> neighbours;
        neighbours.push_back(make_pair(curr.first - 1, curr.second));
        neighbours.push_back(make_pair(curr.first + 1, curr.second));
        neighbours.push_back(make_pair(curr.first, curr.second - 1));
        neighbours.push_back(make_pair(curr.first, curr.second + 1));
        for (auto &n: neighbours) {
            if ((n.first >= 0) && (n.first < (int)grid[0].size()) && (n.second >= 0) && (n.second < (int)grid.size())) {
                if ((grid[n.second][n.first] != '#') && (visited.count(n) == 0)) {
                    curr = n;
                    break;
                }
            }
        }
    }
    path.push_back(curr);
}
