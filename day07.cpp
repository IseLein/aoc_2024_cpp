
#include <bits/stdc++.h>

using namespace std;

typedef unsigned long long ull;

void part1();
void part2();

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day07", "r", stdin);

    part2();
}

/*
* Only works for base < 10
*/
string base_padded(ull n, int width, int base) {
    string s = "";
    int rem = 0;
    while (n > 0) {
        rem = n % base;
        n = n / base;
        s = "0123456789"[rem] + s;
    }
    int pad = width - s.size();
    s.insert(0, pad, '0');
    return s;
}

void part1() {
    vector<tuple<ull, vector<ull>>> v;

    string line;
    while (getline(cin, line)) {
        ull n = 0;
        vector<ull> a;

        string tok = "";
        for (int i = 0; i < (int)line.size(); i++) {
            if (line[i] == ':') {
                n = stoull(tok);
                tok = "";
                i++;
                continue;
            }
            if (line[i] == ' ') {
                a.push_back(stoull(tok));
                tok = "";
                continue;
            }
            tok += line[i];
        }
        a.push_back(stoull(tok));
        v.push_back(make_tuple(n, a));

    }

    ull total = 0;
    for (auto &i : v) {
        ull n = get<0>(i);
        vector<ull> a = get<1>(i);

        int num_operators = a.size() - 1;
        for (int i = 0; i < pow(2, num_operators); i++) {
            string curr_try = base_padded(i, num_operators, 2);
            ull curr_sum = a[0];
            for (int j = 0; j < num_operators; j++) {
                if (curr_try[j] == '0') {
                    curr_sum += a[j + 1];
                } else {
                    curr_sum *= a[j + 1];
                }
            }
            if (curr_sum == n) {
                total += n;
                // cout << "got: " << n << " with " << curr_try << "\n";
                break;
            }
        }
    }
    cout << "total: " << total << "\n";
}

void part2() {
    vector<tuple<ull, vector<ull>>> v;

    string line;
    while (getline(cin, line)) {
        ull n = 0;
        vector<ull> a;

        string tok = "";
        for (int i = 0; i < (int)line.size(); i++) {
            if (line[i] == ':') {
                n = stoull(tok);
                tok = "";
                i++;
                continue;
            }
            if (line[i] == ' ') {
                a.push_back(stoull(tok));
                tok = "";
                continue;
            }
            tok += line[i];
        }
        a.push_back(stoull(tok));
        v.push_back(make_tuple(n, a));

    }

    ull total = 0;
    for (auto &i : v) {
        ull n = get<0>(i);
        vector<ull> a = get<1>(i);

        int num_operators = a.size() - 1;
        for (int i = 0; i < pow(3, num_operators); i++) {
            string curr_try = base_padded(i, num_operators, 3);
            ull curr_sum = a[0];
            for (int j = 0; j < num_operators; j++) {
                if (curr_try[j] == '0') {
                    curr_sum += a[j + 1];
                } else if (curr_try[j] == '1') {
                    curr_sum *= a[j + 1];
                } else {
                    curr_sum = stoull(to_string(curr_sum) + to_string(a[j + 1]));
                }
            }
            if (curr_sum == n) {
                total += n;
                // cout << "got: " << n << " with " << curr_try << "\n";
                break;
            }
        }
    }
    cout << "total: " << total << "\n";
}
