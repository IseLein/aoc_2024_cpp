
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

void find_three_connect(map<string, vector<string>> connections, set<set<string>> &three_connect);
set<set<string>> bron_kerbosch(set<string> r, set<string> p, set<string> x, map<string, vector<string>> &connections);

int main() {
    fstream fin("input/day23");

    map<string, vector<string>> connections;
    string line;
    while (getline(fin, line)) {
        int idx = line.find('-');
        string a = line.substr(0, idx);
        string b = line.substr(idx + 1);

        if (connections.count(a) == 0) {
            connections[a] = vector<string>();
        }
        if (connections.count(b) == 0) {
            connections[b] = vector<string>();
        }
        connections[a].push_back(b);
        connections[b].push_back(a);
    }

    // Part 1
    set<set<string>> three_connect;
    find_three_connect(connections, three_connect);
    int count = 0;
    for (auto &three_conn : three_connect) {
        for (auto &node : three_conn) {
            if (node[0] == 't') {
                count++;
                break;
            }
        }
    }
    cout << "Count: " << count << "\n";

    cout << "\n";
    // Part 2
    set<string> r;
    set<string> p;
    for (auto &[a, _] : connections) {
        p.insert(a);
    }
    set<string> x;
    set<set<string>> cliques = bron_kerbosch(r, p, x, connections);
    int largest_clique_size = 0;
    set<string> largest_clique;
    for (auto &clique : cliques) {
        if (largest_clique_size < (int)clique.size()) {
            largest_clique_size = (int)clique.size();
            largest_clique = clique;
        }
    }

    string largest_clique_str;
    for (auto &node : largest_clique) {
        largest_clique_str += node;
        largest_clique_str += ",";
    }
    largest_clique_str.pop_back();
    cout << "Largest clique: " << largest_clique_str << "\n";

    return 0;
}

set<set<string>> bron_kerbosch(set<string> r, set<string> p, set<string> x, map<string, vector<string>> &connections) {
    if ((p.size() == 0) && (x.size() == 0)) {
        set<set<string>> r1;
        r1.insert(r);
        return r1;
    }
    set<set<string>> cliques;
    vector<string> p_v = vector<string>(p.begin(), p.end());
    for (auto &v: p_v) {
        set<string> r1(r);
        r1.insert(v);
        set<string> p1;
        for (auto &w: connections[v]) {
            if (p.count(w) != 0) {
                p1.insert(w);
            }
        }
        set<string> x1;
        for (auto &w: connections[v]) {
            if (x.count(w) != 0) {
                x1.insert(w);
            }
        }

        set<set<string>> cliques1 = bron_kerbosch(r1, p1, x1, connections);
        cliques.insert(cliques1.begin(), cliques1.end());

        p.erase(v);
        x.insert(v);
    }
    return cliques;
}

void find_three_connect(map<string, vector<string>> connections, set<set<string>> &three_connect) {
    for (auto &[a, a_conn] : connections) {
        for (int i = 0; i < (int)a_conn.size(); i++) {
            for (int j = i + 1; j < (int)a_conn.size(); j++) {
                set<string> three_conn = {a, a_conn[i], a_conn[j]};
                if (three_connect.count(three_conn) != 0) {
                    continue;
                }
                vector<string> b_conn = connections[a_conn[i]];
                if (find(b_conn.begin(), b_conn.end(), a_conn[j]) != b_conn.end()) {
                    three_connect.insert(three_conn);
                }
            }
        }
    }
}
