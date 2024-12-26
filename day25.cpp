
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void parseInput(ifstream &fin, vector<vector<int>> &locks, vector<vector<int>> &keys);

int main() {
    ifstream fin("input/day25");
    vector<vector<int>> locks;
    vector<vector<int>> keys;
    parseInput(fin, locks, keys);
    fin.close();

    int num_fit = 0;
    for (auto &lock : locks) {
        for (auto &key : keys) {
            bool fit = true;
            for (int i = 0; i < 5; i++) {
                if (lock[i] + key[i] > 5) {
                    fit = false;
                    break;
                }
            }
            num_fit += fit ? 1 : 0;
        }
    }
    cout << "Number of fit: " << num_fit << "\n";

    return 0;
}

void parseInput(ifstream &fin, vector<vector<int>> &locks, vector<vector<int>> &keys) {
    while(true) {
        string line;
        getline(fin, line);
        char c = line[0];

        vector<string> grid;
        for (int i = 0; i < 6; i++) {
            string line;
            getline(fin, line);
            grid.push_back(line);
        }
        if (c == '#') {
            vector<int> lock;
            for (int i = 0; i < 5; i++) {
                int num = 0;
                while ((num < 6) && (grid[num][i] == '#')) {
                    num++;
                }
                lock.push_back(num);
            }
            locks.push_back(lock);
        }
        if (c == '.') {
            vector<int> key;
            for (int i = 0; i < 5; i++) {
                int num = 5;
                while ((num >= 0) && (grid[num][i] == '#')) {
                    num--;
                }
                key.push_back(5 - num - 1);
            }
            keys.push_back(key);
        }

        if (!getline(fin, line)) {
            break;
        }
    }
}
