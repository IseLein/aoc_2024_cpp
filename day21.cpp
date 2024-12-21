
#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

#define ll long long
#define DEBUG false
#define IFD(x) if (DEBUG) { x; }

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

map<tuple<string, int>, ll> SEQUENCE_LENGTH_CACHE = {};

ll getDirections2(vector<vector<char>> keypad, string key, int level);

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

    ll total = 0;
    for (int i = 0; i < (int)keys.size(); i++) {
        string key = keys[i];
        int level = 26;
        ll length = getDirections2(KEYPAD, key, level);

        ll num = stoll(key.substr(0, 3));
        ll score = num * length;
        total += score;
        cout << key << ": " << num << " * " << length << " = " << score << "\n";
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

ll sequenceLength(string s, int level) {
    IFD(cout << "level: " << level << ": " << s << "\n");
    if (level == 0) {
        return s.size();
    }
    if (SEQUENCE_LENGTH_CACHE.count(make_pair(s, level)) != 0) {
        IFD(cout << "cache hit\n");
        return SEQUENCE_LENGTH_CACHE.at(make_pair(s, level));
    }

    pair<int, int> start;
    _findChar(DKEYPAD, 'A', start);
    ll total = 0;
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
        vector<ll> lengths;
        for (auto &arrangement: arrangements) {
            if (!_isValidDir(DKEYPAD, arrangement, start)) {
                continue;
            }
            arrangement += 'A';
            IFD(cout << "arrangement: " << arrangement << "\n");
            lengths.push_back(sequenceLength(arrangement, level - 1));
        }
        IFD(cout << "\n");
        sort(lengths.begin(), lengths.end());
        total += lengths[0];
        start = next;
    }
    SEQUENCE_LENGTH_CACHE.insert(make_pair(make_tuple(s, level), total));
    return total;
}

ll getDirections2(vector<vector<char>> keypad, string key, int level) {
    pair<int, int> start;
    _findChar(keypad, 'A', start);

    ll total = 0;
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
        vector<ll> lengths;
        for (auto &arrangement: arrangements) {
            if (!_isValidDir(keypad, arrangement, start)) {
                continue;
            }
            arrangement += 'A';
            IFD(cout << "arrangement: " << arrangement << "\n");
            lengths.push_back(sequenceLength(arrangement, level - 1));
        }
        IFD(cout << "\n");
        sort(lengths.begin(), lengths.end());
        total += lengths[0];
        start = next;
    }
    return total;
}
