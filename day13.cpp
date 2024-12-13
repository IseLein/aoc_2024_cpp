
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void part1();
bool parseNextLine(vector<long long> &a, vector<long long> &b, ifstream &file);

int main() {
    part1();
}

string print(vector<long long> v) {
    string s = "[";
    for (int i = 0; i < (int)v.size(); i++) {
        s += to_string(v[i]);
        if (i != (int)v.size() - 1) {
            s += ", ";
        }
    }
    s += "]";
    return s;
}

long long gcd(long long a, long long b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

void part1() {
    ifstream file("input/day13");

    long long total = 0;

    vector<long long> a = {0, 0, 0, 0};
    vector<long long> b = {0, 0};
    while (parseNextLine(a, b, file)) {
        // x = Inv(A) * b and Inv(A) = Adj(A) / Det(A)
        // I'm doing Adj(A) * b first then diving by Det(A) so I don't deal
        // with floats
        vector<long long> adj {a[3], -a[1], -a[2], a[0]};
        vector<long long> sol = {0, 0};
        sol[0] = adj[0] * b[0] + adj[1] * b[1];
        sol[1] = adj[2] * b[0] + adj[3] * b[1];

        long long det = (a[0] * a[3]) - (a[1] * a[2]);
        if (det == 0) {
            cout << "No solution\n";
            continue;
        }

        if ((gcd(sol[0], det) != det) || (gcd(sol[1], det) != det)) {
            // cout << "No integer solution\n";
            continue;
        }

        sol[0] /= det;
        sol[1] /= det;
        long long cost = (3 * sol[0]) + sol[1];
        total += cost;
    }
    cout << "Total: " << total << "\n";
}

bool parseNextLine(vector<long long> &a, vector<long long> &b, ifstream &file) {
    string line;
    string _unused;

    string x_val;
    string y_val;

    // button A
    if (!getline(file, line)) {
        return false;
    }
    istringstream iss_a(line);
    iss_a >> _unused >> _unused >> x_val >> y_val;
    a[0] = stol(x_val.substr(2, x_val.size() - 3));
    a[2] = stol(y_val.substr(2, y_val.size() - 2));

    // button B
    getline(file, line);
    istringstream iss_b(line);
    iss_b >> _unused >> _unused >> x_val >> y_val;
    a[1] = stol(x_val.substr(2, x_val.size() - 3));
    a[3] = stol(y_val.substr(2, y_val.size() - 2));

    // prize
    getline(file, line);
    istringstream iss_c(line);
    iss_c >> _unused >> x_val >> y_val;
    b[0] = stol(x_val.substr(2, x_val.size() - 3)) + 10000000000000;
    b[1] = stol(y_val.substr(2, y_val.size() - 2)) + 10000000000000;

    // empty line
    getline(file, line);
    return true;
}
