
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <utility>
#include <vector>

using namespace std;

void parseInput(ifstream &file, vector<vector<char>> &grid, pair<int, int> &start, pair<int, int> &end);
int visitNeighbours(vector<vector<char>> &grid, pair<int, int> start, pair<int, int> dir);
int findPathTiles(vector<vector<char>> &grid, pair<int, int> start, pair<int, int> dir);

void print(vector<vector<char>> v2) {
    for (auto v: v2) {
        for (auto c: v) {
            cout << c;
        }
        cout << "\n";
    }
}

string to_str(pair<int, int> p) {
    return "( " + to_string(p.first) + ", " + to_string(p.second) + " )";
}

void print(map<string, int> m) {
    for (auto p: m) {
        cout << p.first << " " << p.second << "\n";
    }
}

int main() {
    ifstream file("input/day16");
    vector<vector<char>> grid;
    pair<int, int> start;
    pair<int, int> end;
    parseInput(file, grid, start, end);

    set<pair<int, int>> visited;
    int lowestScore = visitNeighbours(grid, start, make_pair(1, 0));
    if (lowestScore == -1) {
        cout << "No path found\n";
        return 1;
    } else {
        cout << "Lowest score is " << lowestScore << "\n";
    }

    int numTiles = findPathTiles(grid, start, make_pair(1, 0));
    cout << numTiles << " tiles along the optimal paths\n";
}

typedef struct {
    pair<int, int> pos;
    pair<int, int> dir;
} NodeDetails;

typedef struct {
    int score;
    pair<int, int> pos;
    pair<int, int> dir;
} SNodeDetails;

struct SNodeDetailsGreater {
    bool operator()(const SNodeDetails &a, const SNodeDetails &b) const {
        return a.score > b.score;
    }
};

struct NodeDetailsLess {
    bool operator()(const NodeDetails &a, const NodeDetails &b) const {
        if (a.pos.first != b.pos.first) {
            return a.pos.first < b.pos.first;
        }
        if (a.pos.second != b.pos.second) {
            return a.pos.second < b.pos.second;
        }
        if (a.dir.first != b.dir.first) {
            return a.dir.first < b.dir.first;
        }
        return a.dir.second < b.dir.second;
    }
};

void print(SNodeDetails n) {
    cout << "(" << n.pos.first << ", " << n.pos.second << ") & (" << n.dir.first << ", " << n.dir.second << ") Score: " << n.score << "\n";
}

int visitNeighbours(vector<vector<char>> &grid, pair<int, int> start, pair<int, int> dir) {
    priority_queue<SNodeDetails, vector<SNodeDetails>, SNodeDetailsGreater> pQueue;
    set<NodeDetails, NodeDetailsLess> visited;

    pQueue.push({ 0, start, dir });
    visited.insert({ start, dir });

    while (pQueue.size() > 0) {
        SNodeDetails node = pQueue.top();
        pQueue.pop();
        visited.insert({ node.pos, node.dir });

        if (grid[node.pos.second][node.pos.first] == 'E') {
            return node.score;
        }

        vector<SNodeDetails> neighbours;
        neighbours.push_back({
            node.score + 1,
            make_pair(node.pos.first + node.dir.first, node.pos.second + node.dir.second),
            make_pair(node.dir.first, node.dir.second)
        });
        neighbours.push_back({
            node.score + 1000,
            make_pair(node.pos.first, node.pos.second),
            make_pair(-node.dir.second, node.dir.first)
        });
        neighbours.push_back({
            node.score + 1000,
            make_pair(node.pos.first, node.pos.second),
            make_pair(node.dir.second, -node.dir.first)
        });

        for (auto &n: neighbours) {
            if (grid[n.pos.second][n.pos.first] == '#') {
                continue;
            }
            if (visited.count({ n.pos, n.dir }) != 0) {
                continue;
            }
            pQueue.push(n);
        }
    }
    return -1;
}

typedef struct {
    int score;
    pair<int, int> pos;
    pair<int, int> dir;
    pair<int, int> prev_pos;
    pair<int, int> prev_dir;
} SNodeDetails2;

struct SNodeDetailsGreater2 {
    bool operator()(const SNodeDetails2 &a, const SNodeDetails2 &b) const {
        return a.score > b.score;
    }
};

int findPathTiles(vector<vector<char>> &grid, pair<int, int> start, pair<int, int> dir) {
    priority_queue<SNodeDetails2, vector<SNodeDetails2>, SNodeDetailsGreater2> pQueue;
    map<NodeDetails, int, NodeDetailsLess> lowestScores;
    map<NodeDetails, set<NodeDetails, NodeDetailsLess>, NodeDetailsLess> backtrack;
    set<NodeDetails, NodeDetailsLess> end_states;
    int lowestScore = -1;

    pQueue.push({ 0, start, dir, { -1, -1 }, { -1, -1 } });
    lowestScores[{ start, dir }] = 0;

    while (pQueue.size() > 0) {
        SNodeDetails2 node = pQueue.top();
        pQueue.pop();
        NodeDetails n_node = { 
            make_pair(node.pos.first, node.pos.second),
            make_pair(node.dir.first, node.dir.second)
        };

        if ((lowestScores.count(n_node) != 0) && (node.score > lowestScores.at(n_node))) {
            continue;
        }
        lowestScores[n_node] = node.score;

        if (grid[node.pos.second][node.pos.first] == 'E') {
            if ((lowestScore != -1) && (node.score > lowestScore)) {
                break;
            }
            lowestScore = node.score;
            end_states.insert(n_node);
        }

        if (backtrack.count(n_node) == 0) {
            set<NodeDetails, NodeDetailsLess> s;
            backtrack[n_node] = s;
        }
        NodeDetails p_node = { 
            make_pair(node.prev_pos.first, node.prev_pos.second),
            make_pair(node.prev_dir.first, node.prev_dir.second)
        };
        backtrack[n_node].insert(p_node);

        vector<SNodeDetails2> neighbours;
        neighbours.push_back({
            node.score + 1,
            make_pair(node.pos.first + node.dir.first, node.pos.second + node.dir.second),
            make_pair(node.dir.first, node.dir.second),
            make_pair(node.pos.first, node.pos.second),
            make_pair(node.dir.first, node.dir.second)
        });
        neighbours.push_back({
            node.score + 1000,
            make_pair(node.pos.first, node.pos.second),
            make_pair(-node.dir.second, node.dir.first),
            make_pair(node.pos.first, node.pos.second),
            make_pair(node.dir.first, node.dir.second)
        });
        neighbours.push_back({
            node.score + 1000,
            make_pair(node.pos.first, node.pos.second),
            make_pair(node.dir.second, -node.dir.first),
            make_pair(node.pos.first, node.pos.second),
            make_pair(node.dir.first, node.dir.second)
        });

        for (auto &n: neighbours) {
            if (grid[n.pos.second][n.pos.first] == '#') {
                continue;
            }
            if ((lowestScores.count(n_node) != 0) && (node.score > lowestScores.at(n_node))) {
                continue;
            }
            pQueue.push(n);
        }
    }

    set<NodeDetails, NodeDetailsLess> visited(end_states);
    queue<NodeDetails> states;
    for (auto &es: end_states) {
        states.push(es);
    }

    // first state doesn't have a previous state
    NodeDetails s_start = { make_pair(start.first, start.second), make_pair(dir.first, dir.second) };
    backtrack[s_start] = set<NodeDetails, NodeDetailsLess>();

    while (states.size() > 0) {
        NodeDetails n = states.front();
        states.pop();
        for (auto &b: backtrack.at(n)) {
            if (visited.count(b) != 0) {
                continue;
            }
            visited.insert(b);
            states.push(b);
        }
    }

    set<pair<int, int>> path;
    for (auto &n: visited) {
        path.insert(n.pos);
    }

    return path.size();
}

void parseInput(ifstream &file, vector<vector<char>> &grid, pair<int, int> &start, pair<int, int> &end) {
    string line;
    int num = 0;
    while (getline(file, line)) {
        vector<char> v;
        for (int i = 0; i < (int)line.size(); i++) {
            v.push_back(line[i]);
            if (line[i] == 'S') {
                start = make_pair(i, num);
            }
            if (line[i] == 'E') {
                end = make_pair(i, num);
            }
        }
        grid.push_back(v);
        num++;
    }
}
