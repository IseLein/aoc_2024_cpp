
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

#define ll long long

using namespace std;

void parseInput(ifstream &fin, vector<string> &patterns, vector<string> &designs);
int numPossibleDesigns(vector<string> &patterns, vector<string> &designs);
ll numTotalDesigns(vector<string> &patterns, vector<string> &designs);

void printPatterns(vector<string> &patterns) {
    cout << "patterns: [";
    for (int i = 0; i < (int)patterns.size(); i++) {
        cout << patterns[i];
        if (i < (int)patterns.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]\n";
}

int main() {
    ifstream fin("input/day19");
    vector<string> patterns;
    vector<string> designs;
    parseInput(fin, patterns, designs);

    // int nDesigns = numPossibleDesigns(patterns, designs);
    // cout << "numPossibleDesigns: " << nDesigns << "\n";

    ll nTotalDesigns = numTotalDesigns(patterns, designs);
    cout << "numTotalDesigns: " << nTotalDesigns << "\n";

    return 0;
}

void _startsWith(string &design, vector<string> &patterns, vector<int> &startsWith) {
    for (int i = 0; i < (int)patterns.size(); i++) {
        bool startsWithPattern = true;
        if (design.size() < patterns[i].size()) {
            startsWithPattern = false;
            continue;
        }
        for (int j = 0; j < (int)patterns[i].size(); j++) {
            if (design[j] != patterns[i][j]) {
                startsWithPattern = false;
                break;
            }
        }
        if (startsWithPattern) {
            startsWith.push_back(i);
        }
    }
}

ll _numTotalDesigns(string &design, vector<string> &patterns, map<string, ll> &cache) {
    if (design.size() == 0) {
        return 1;
    }
    if (cache.count(design) != 0) {
        return cache[design];
    }
    ll totalDesigns = 0;
    vector<int> startsWith;
    _startsWith(design, patterns, startsWith);
    for (int i = 0; i < (int)startsWith.size(); i++) {
        string pattern = patterns[startsWith[i]];
        string nPattern = design.substr(pattern.size());
        totalDesigns += _numTotalDesigns(nPattern, patterns, cache);
    }
    cache[design] = totalDesigns;
    return totalDesigns;
}

ll numTotalDesigns(vector<string> &patterns, vector<string> &designs) {
    ll totalDesigns = 0;
    ll numPossibleDesigns = 0;
    for (int i = 0; i < (int)designs.size(); i++) {
        string design = designs[i];
        map<string, ll> cache;
        ll nDesigns = _numTotalDesigns(design, patterns, cache);
        totalDesigns += nDesigns;
        if (nDesigns != 0) {
            numPossibleDesigns++;
        }
    }
    return totalDesigns;
}

struct Node {
    int numTowels;
    string design;
    vector<string> towels;

    bool operator<(const Node &other) const {
        return numTowels > other.numTowels;
    }
};

int numPossibleDesigns(vector<string> &patterns, vector<string> &designs) {
    int numPossibleDesigns = 0;
    for (int i = 0; i < (int)designs.size(); i++) {
        string design = designs[i];
        priority_queue<Node> pQueue;
        set<string> checked;

        vector<string> towels;
        pQueue.push({ 0, design, towels });

        while (!pQueue.empty()) {
            Node node = pQueue.top();
            pQueue.pop();

            if (node.design.size() == 0) {
                numPossibleDesigns++;
                break;
            }

            vector<int> startsWith;
            _startsWith(node.design, patterns, startsWith);
            for (int i = 0; i < (int)startsWith.size(); i++) {
                string pattern = patterns[startsWith[i]];
                vector<string> nTowels(node.towels);
                nTowels.push_back(pattern);
                string nPattern = node.design.substr(pattern.size());
                if (checked.count(nPattern) == 0) {
                    pQueue.push({ node.numTowels + 1, nPattern, nTowels });
                    checked.insert(nPattern);
                }
            }
        }
    }
    return numPossibleDesigns;
}

void parseInput(ifstream &fin, vector<string> &patterns, vector<string> &designs) {
    string line;
    getline(fin, line);

    string tok;
    int m = 0;
    while (true) {
        int n = m;
        m = line.find(',', n);
        if (m == -1) {
            tok = line.substr(n);
            patterns.push_back(tok);
            break;
        }
        tok = line.substr(n, m - n);
        patterns.push_back(tok);
        m += 2;
    }

    getline(fin, line);

    while (getline(fin, line)) {
        designs.push_back(line);
    }
}
