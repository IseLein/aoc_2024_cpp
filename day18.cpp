
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

#define HEIGHT 70
#define WIDTH 70

using namespace std;

void parseInput(ifstream &fin, vector<pair<int, int>> &bytePositions);
int shortestDistance(vector<vector<char>> &grid);

void print(vector<vector<char>> grid) {
    for (auto row: grid) {
        for (auto c: row) {
            cout << c;
        }
        cout << "\n";
    }
}

int main() {
    ifstream fin("input/day18");
    vector<pair<int, int>> bytePositions;
    parseInput(fin, bytePositions);

    vector<vector<char>> grid;
    for(int i = 0; i <= HEIGHT; i++) {
        vector<char> v;
        for (int j = 0; j <= WIDTH; j++) {
            v.push_back('.');
        }
        grid.push_back(v);
    }

    int numFallen = 1024;
    for (int i = 0; i < numFallen; i++) {
        pair<int, int> p = bytePositions[i];
        grid[p.second][p.first] = '#';
    }


    int sd = shortestDistance(grid);
    cout << "Shortest Distance after falling " << numFallen << " bytes: " << sd << "\n";

    for (int i = numFallen; i < (int)bytePositions.size(); i++) {
        pair<int, int> p = bytePositions[i];
        grid[p.second][p.first] = '#';
        sd = shortestDistance(grid);
        if (sd == -1) {
            cout << "No path found after falling " << i + numFallen << " bytes\n";
            cout << "Byte position: " << p.first << ", " << p.second << "\n";
            break;
        }
        cout << "Shortest Distance after falling " << i + numFallen << " bytes: " << sd << "\n";
    }
    // cout << "Shortest Distance: " << sd << "\n";
}

struct Node {
    int score;
    pair<int, int> position;
};

struct NodeLess {
    bool operator()(const Node &a, const Node &b) {
        return a.score > b.score;
    }
};

int shortestDistance(vector<vector<char>> &grid) {
    priority_queue<Node, vector<Node>, NodeLess> pQueue;
    set<pair<int, int>> visited;

    pQueue.push({ 0, { 0, 0 }});

    while (pQueue.size() > 0) {
        Node currNode = pQueue.top();
        pQueue.pop();
        pair<int, int> currPos = currNode.position;
        if (visited.count(currPos) != 0) {
            continue;
        }
        visited.insert(currPos);

        if ((currPos.first == WIDTH) && (currPos.second == HEIGHT)) {
            return currNode.score;
        }

        vector<pair<int, int>> neighbours;
        // up
        if (currPos.second > 0) {
            pair<int, int> newPos = make_pair(currPos.first, currPos.second - 1);
            neighbours.push_back(newPos);
        }
        // down
        if (currPos.second < HEIGHT) {
            pair<int, int> newPos = make_pair(currPos.first, currPos.second + 1);
            neighbours.push_back(newPos);
        }
        // left
        if (currPos.first > 0) {
            pair<int, int> newPos = make_pair(currPos.first - 1, currPos.second);
            neighbours.push_back(newPos);
        }
        // right
        if (currPos.first < WIDTH) {
            pair<int, int> newPos = make_pair(currPos.first + 1, currPos.second);
            neighbours.push_back(newPos);
        }
        for (auto &n: neighbours) {
            if (grid[n.second][n.first] == '#') {
                continue;
            }
            if (visited.count(n) != 0) {
                continue;
            }
            pQueue.push({ currNode.score + 1, n });
        }
    }
    return -1;
}

void parseInput(ifstream& fin, vector<pair<int, int>> &bytePositions) {
    string line;
    while(getline(fin, line)) {
        int m = line.find(',');
        pair<int, int> p;
        p.first = stoi(line.substr(0, m));
        p.second = stoi(line.substr(m + 1));
        bytePositions.push_back(p);
    }
}
