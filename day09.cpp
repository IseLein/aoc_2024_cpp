
#include <bits/stdc++.h>

using namespace std;

void part1();
void part2();

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day09_2t", "r", stdin);

    part2();
}

string print_vec(vector<int> v) {
    string s = "";
    for (size_t i = 0; i < v.size(); i++) {
        s += to_string(v[i]);
        s += " ";
    }
    return s;
}

void vec_insert(vector<int> &v, int val) {
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] >= val) {
            v.insert(v.begin() + i, val);
            return;
        }
    }
    v.push_back(val);
}

void part1() {
    string input;
    cin >> input;

    vector<string> blocks;
    int ids = 0;
    for (int i = 0; i < (int)input.size(); i++) {
        int num = stoi(input.substr(i, 1));
        if (i % 2 == 0) {
            for (int j = 0; j < num; j++) {
                blocks.push_back(to_string(ids));
            }
            ids++;
        } else {
            for (int j = 0; j < num; j++) {
                blocks.push_back(".");
            }
        }
    }
    // cout << blocks << "\n";

    vector<int> gaps;
    for (int i = 0; i < (int)blocks.size(); i++) {
        if (blocks[i] == ".") {
            gaps.push_back(i);
        }
    }

    cout << blocks.size() << "\n";

    int num_gaps = gaps.size();
    int last_block = blocks.size() - 1;
    while (gaps[0] < (int)blocks.size() - num_gaps) {
        while ((last_block >= 0) && (blocks[last_block] == ".")) {
            last_block--;
        }
        if ((last_block < 0) || (blocks[last_block] == ".")) {
            // should not happen
            cout << "should not happen\n";
            break;
        }
        swap(blocks[gaps[0]], blocks[last_block]);

        gaps.erase(gaps.begin());
        vec_insert(gaps, last_block);

        // cout << "Blocks: " << blocks << "\n";
        // cout << "Gaps: " << print_vec(gaps) << "\n";
    }

    cout << "last_block: " << last_block << "\n";
    cout << "num_gaps: " << num_gaps << "\n";
    cout << "first_gap: " << gaps[0] << "\n";
    cout << "vector size: " << blocks.size() << "\n";

    for (int i = 0; i < (int)blocks.size(); i++) {
        cout << blocks[i] << " ";
    }
    cout << "\n";

    unsigned long long checksum = 0;
    for (int i = 0; i < (int)blocks.size(); i++) {
        if (blocks[i] == ".") {
            checksum += 0;
        } else {
            checksum += stoull(blocks[i]) * i;
        }
    }
    cout<< "Checksum: " << checksum << "\n";
}

void part2() {
    string input;
    cin >> input;

    vector<string> blocks;
    int ids = 0;
    unordered_map<string, int> block_counts;
    for (int i = 0; i < (int)input.size(); i++) {
        int num = stoi(input.substr(i, 1));
        if (i % 2 == 0) {
            for (int j = 0; j < num; j++) {
                blocks.push_back(to_string(ids));
            }
            block_counts[to_string(ids)] = num;
            ids++;
        } else {
            for (int j = 0; j < num; j++) {
                blocks.push_back(".");
            }
        }
    }

    vector<int> gaps;
    unordered_map<int, int> gap_runs;
    int current_gap = 0;
    int current_run = 0;
    for (int i = 0; i < (int)blocks.size(); i++) {
        if (blocks[i] == ".") {
            gaps.push_back(i);
            if (current_run == 0) {
                current_gap = i;
            }
            current_run++;
        } else {
            if (current_run > 0) {
                gap_runs[current_gap] = current_run;
                current_run = 0;
            }
        }
    }

    cout << blocks.size() << "\n";

    int curr = blocks.size() - 1;
    while (true) {
        while ((curr - 1 >= 0) && (blocks[curr - 1] == blocks[curr])) {
            curr--;
        }
        if (curr < 0) {
            break;
        }
        bool found = false;
        int need = block_counts[blocks[curr]];
        for (int i = 0; i < curr; i++) {
            if ((gap_runs.count(i) > 0) && (gap_runs[i] >= need)) {
                for (int j = 0; j < need; j++) {
                    swap(blocks[i + j], blocks[curr + j]);
                }
                int new_space = gap_runs[i] - need;
                if (new_space > 0) {
                    gap_runs[i + need] = new_space;
                }
                gap_runs.erase(i);
                found = true;
                break;
            }
        }
        if (!found) {
            curr--;
        }
        while ((curr >= 0) && (blocks[curr] == ".")) {
            curr--;
        }
    }


    unsigned long long checksum = 0;
    for (int i = 0; i < (int)blocks.size(); i++) {
        if (blocks[i] == ".") {
            checksum += 0;
        } else {
            checksum += stoull(blocks[i]) * i;
        }
    }
    cout<< "Checksum: " << checksum << "\n";
}
