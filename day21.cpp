
#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

#define DEBUG 1

using namespace std;

const vector<vector<char>> KEYPAD = {
    { '7', '8', '9' },
    { '4', '5', '6' },
    { '1', '2', '3' },
    { '#', '0', 'A' }
};
const vector<vector<char>> DKEYPAD = {
    { '#', '^', 'A' },
    { '<', 'v', '>' },
};

map<tuple<int, int, int, pair<int, int>>, string> BEST_DIRECTIONS_CACHE = {};

map<tuple<string, int>, long> SEQUENCE_LENGTH_CACHE = {};

string getDirections(vector<vector<char>> keypad, string key, int level);
long getDirections2(vector<vector<char>> keypad, string key, int level);

void printKeypad(vector<vector<char>> keypad) {
    for (auto& row : keypad) {
        for (auto& key : row) {
            cout << key;
        }
        cout << "\n";
    }
}


int main() {
    ifstream fin("input/day21");
    vector<string> keys;
    string line;
    while (getline(fin, line)) {
        keys.push_back(line);
    }

    /*for (auto &key: keys) {*/
    /*    cout << key << "\n";*/
    /*    int level = 2;*/
    /*    string dir = getDirections(KEYPAD, key, level);*/
    /*    cout << level << ": " << dir << "\n";*/
    /*    level--;*/
    /*    for (int i = level; i >= 0; i--) {*/
    /*        dir = getDirections(DKEYPAD, dir, i);*/
    /*        cout << i << ": " << dir << "\n";*/
    /*    }*/
    /**/
    /*    long length = dir.size();*/
    /*    long num = stol(key.substr(0, 3));*/
    /*    long score = num * length;*/
    /*    total += score;*/
    /*    cout << key << ": " << num << " * " << length << " = " << score << "\n";*/
    /*}*/
    /*cout  << "Total: " << total << "\n";*/

    long total = 0;
    for (int i = 0; i < (int)keys.size(); i++) {
        string key = keys[i];
        int level = 3;
        long length = getDirections2(KEYPAD, key, level);
        cout << key << ": " << length << "\n";

        long num = stol(key.substr(0, 3));
        long score = num * length;
        total += score;
        // cout << key << ": " << num << " * " << length << " = " << score << "\n";
    }
    cout << "Total: " << total << "\n";
}

void _findChar(vector<vector<char>> keypad, char c, pair<int, int> &pos) {
    for (int i = 0; i < (int)keypad.size(); i++) {
        for (int j = 0; j < (int)keypad[0].size(); j++) {
            if (keypad[i][j] == c) {
                pos = make_pair(i, j);
                return;
            }
        }
    }
    cout << "not found\n";
}

bool _isValidDir(vector<vector<char>> keypad, string dir, pair<int, int> pos) {
    for (auto &c: dir) {
        pair<int, int> next;

        if (c == '^') {
            next = make_pair(pos.first - 1, pos.second);
        } else if (c == 'v') {
            next = make_pair(pos.first + 1, pos.second);
        } else if (c == '>') {
            next = make_pair(pos.first, pos.second + 1);
        } else if (c == '<') {
            next = make_pair(pos.first, pos.second - 1);
        }
        if (keypad[next.first][next.second] == '#') {
            return false;
        }
        pos = next;
    }
    return true;
}

string _getDir(string key) {
    pair<int, int> start;
    _findChar(DKEYPAD, 'A', start);

    key += "A";

    string directions = "";
    for (auto &c: key) {
        pair<int, int> next;
        _findChar(DKEYPAD, c, next);

        int yDiff = next.first - start.first;
        if (yDiff > 0) {
            directions += string(yDiff, 'v');
        } else if (yDiff < 0) {
            directions += string(-yDiff, '^');
        }
        int xDiff = next.second - start.second;
        if (xDiff > 0) {
            directions += string(xDiff, '>');
        } else if (xDiff < 0) {
            directions += string(-xDiff, '<');
        }
        directions += 'A';
        start = next;
    }
    return directions;
}

vector<string> _findArrangements(int numX, int numY, char charX, char charY) {
    vector<string> arr;
    if (numX == 0) {
        string s = string(numY, charY);
        arr.push_back(s);
        return arr;
    }
    if (numY == 0) {
        string s = string(numX, charX);
        arr.push_back(s);
        return arr;
    }

    vector<string> arrX = _findArrangements(numX - 1, numY, charX, charY);
    for (int i = 0; i < (int)arrX.size(); i++) {
        string s = charX + arrX[i];
        arr.push_back(s);
    }
    vector<string> arrY = _findArrangements(numX, numY - 1, charX, charY);
    for (int i = 0; i < (int)arrY.size(); i++) {
        string s = charY + arrY[i];
        arr.push_back(s);
    }
    return arr;
}

string _bestDirection(int xDiff, int yDiff, pair<int, int> pos, vector<vector<char>> keypad, int level) {
    tuple<int, int, int, pair<int, int>> key = make_tuple(xDiff, yDiff, level, pos);
    if (BEST_DIRECTIONS_CACHE.count(key) != 0) {
        return BEST_DIRECTIONS_CACHE.at(key);
    }
    if (xDiff == 0 && yDiff == 0) {
        return "";
    }
    // unimportant defaults as they'll be assigned below
    int numX = 0;
    int numY = 0;
    char charX = 'A';
    char charY = 'A';
    if (yDiff > 0) {
        numY = yDiff;
        charY = 'v';
    } else if (yDiff < 0) {
        numY = -yDiff;
        charY = '^';
    }
    if (xDiff > 0) {
        numX = xDiff;
        charX = '>';
    } else if (xDiff < 0) {
        numX = -xDiff;
        charX = '<';
    }
    vector<string> arrangements = _findArrangements(numX, numY, charX, charY);
    map<int, vector<string>> bestDir;
    map<string, string> nextDir2Arr;
    int bestScore = INT_MAX;
    for (auto &arrangement: arrangements) {
        if (!_isValidDir(keypad, arrangement, pos)) {
            continue;
        }
        string nextLevelDir = _getDir(arrangement);
        int score = nextLevelDir.size();
        if (bestDir.count(score) == 0) {
            bestDir[score] = vector<string>();
        }
        bestDir[score].push_back(nextLevelDir);
        if (score < bestScore) {
            bestScore = score;
        }
        nextDir2Arr[nextLevelDir] = arrangement;
        cout << "arrangement: " << arrangement << ": " << nextLevelDir << "\n";
    }


    // best next levels
    vector<string> bestDirs = bestDir[bestScore];
    if (bestDirs.size() == 1) {
        string dir = nextDir2Arr[bestDirs[0]];
        BEST_DIRECTIONS_CACHE.insert(make_pair(key, dir));
        return dir;
        return nextDir2Arr[bestDirs[0]];
    }

    level--;
    while ((level > 0) && (bestDirs.size() > 1)) {
        map<int, vector<string>> bestDir2;
        map<string, string> nextDir2Arr2;
        int bestScore = INT_MAX;
        for (auto &dir: bestDirs) {
            string nextLevelDir = _getDir(dir);
            int score = nextLevelDir.size();
            if (bestDir2.count(score) == 0) {
                bestDir2[score] = vector<string>();
            }
            bestDir2[score].push_back(nextLevelDir);
            if (score < bestScore) {
                bestScore = score;
            }
            nextDir2Arr2[nextLevelDir] = nextDir2Arr[dir];
            cout << "level " << level << ": " << nextDir2Arr[dir] << " -> " << dir << " -> " << nextLevelDir << "\n";
        }
        bestDirs = bestDir2[bestScore];
        nextDir2Arr = nextDir2Arr2;
        level--;
    }

    string dir = nextDir2Arr[bestDirs[0]];
    BEST_DIRECTIONS_CACHE.insert(make_pair(key, dir));
    return dir;
}

string getDirections(vector<vector<char>> keypad, string key, int level) {
    pair<int, int> start;
    _findChar(keypad, 'A', start);
    // cout << "start at " << start.first << ", " << start.second << "\n";

    string directions = "";
    for (auto &c: key) {
        pair<int, int> next;
        _findChar(keypad, c, next);

        // cout << "going to " << c << " at " << next.first << ", " << next.second << "\n";

        int yDiff = next.first - start.first;
        int xDiff = next.second - start.second;
        directions += _bestDirection(xDiff, yDiff, start, keypad, level);
        directions += 'A';
        start = next;
    }
    return directions;
}

long sequenceLength(string s, int level) {
    cout << "level: " << level << ": " << s << "\n";
    if (level == 0) {
        return s.size();
    }
    if (SEQUENCE_LENGTH_CACHE.count(make_pair(s, level)) != 0) {
        cout << "cache hit\n";
        return SEQUENCE_LENGTH_CACHE.at(make_pair(s, level));
    }

    pair<int, int> start;
    _findChar(DKEYPAD, 'A', start);
    long total = 0;
    for (auto &c: s) {
        pair<int, int> next;
        _findChar(DKEYPAD, c, next);

        int yDiff = next.first - start.first;
        int xDiff = next.second - start.second;
        int numX, numY;
        char charX, charY;
        if (yDiff > 0) {
            numY = yDiff;
            charY = 'v';
        } else {
            numY = -yDiff;
            charY = '^';
        }
        if (xDiff > 0) {
            numX = xDiff;
            charX = '>';
        } else {
            numX = -xDiff;
            charX = '<';
        }
        vector<string> arrangements = _findArrangements(numX, numY, charX, charY);
        vector<int> lengths;
        for (auto &arrangement: arrangements) {
            if (!_isValidDir(DKEYPAD, arrangement, start)) {
                continue;
            }
            arrangement += 'A';
            cout << "arrangement: " << arrangement << "\n";
            lengths.push_back(sequenceLength(arrangement, level - 1));
        }
        cout << "\n";
        sort(lengths.begin(), lengths.end());
        total += lengths[0];
        start = next;
    }
    return total;
}

long getDirections2(vector<vector<char>> keypad, string key, int level) {
    pair<int, int> start;
    _findChar(keypad, 'A', start);

    int total = 0;
    for (auto &c: key) {
        pair<int, int> next;
        _findChar(keypad, c, next);

        int yDiff = next.first - start.first;
        int xDiff = next.second - start.second;

        int numX, numY;
        char charX, charY;
        if (yDiff > 0) {
            numY = yDiff;
            charY = 'v';
        } else {
            numY = -yDiff;
            charY = '^';
        }
        if (xDiff > 0) {
            numX = xDiff;
            charX = '>';
        } else {
            numX = -xDiff;
            charX = '<';
        }
        vector<string> arrangements = _findArrangements(numX, numY, charX, charY);
        vector<int> lengths;
        for (auto &arrangement: arrangements) {
            if (!_isValidDir(keypad, arrangement, start)) {
                continue;
            }
            arrangement += 'A';
            cout << "arrangement: " << arrangement << "\n";
            lengths.push_back(sequenceLength(arrangement, level - 1));
        }
        cout << "\n";
        sort(lengths.begin(), lengths.end());
        total += lengths[0];
        start = next;
    }
    return total;
}
