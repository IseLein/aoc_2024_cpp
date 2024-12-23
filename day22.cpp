
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#define ll long long
#define NUM_SECRET_NUMBERS 2000

using namespace std;

ll nextSecretNumber(ll n);

int main() {
    ifstream fin("input/day22");

    ll total = 0;
    ll n;
    vector<vector<int>> buyerNumbers;
    while (fin >> n) {
        ll ans = n;
        vector<int> numbers;
        string ansStr = to_string(ans);
        numbers.push_back((ansStr[ansStr.length() - 1]) - '0');
        for (int i = 0; i < NUM_SECRET_NUMBERS; i++) {
            ans = nextSecretNumber(ans);
            ansStr = to_string(ans);
            numbers.push_back((ansStr[ansStr.length() - 1]) - '0');
        }
        // cout << n << ": " << ans << "\n";
        total += ans;
        buyerNumbers.push_back(numbers);
    }
    cout << "Total: " << total << "\n";

    map<tuple<int, int, int, int>, ll> tracker;
    int bestScore = 0;
    tuple<int, int, int, int> bestSequence = make_tuple(0, 0, 0, 0);
    for (int i = 0; i < (int)buyerNumbers.size(); i++) {
        vector<int> numbers = buyerNumbers[i];
        vector<int> diffs;
        for (int j = 1; j < (int)numbers.size(); j++) {
            diffs.push_back(numbers[j] - numbers[j - 1]);
        }

        set<tuple<int, int, int, int>> seen;
        for (int i = 0; i < (int)diffs.size() - 3; i++) {
            tuple<int, int, int, int> sequence = make_tuple(diffs[i], diffs[i + 1], diffs[i + 2], diffs[i + 3]);
            int score = numbers[i + 4];

            if (seen.count(sequence) > 0) {
                continue;
            }
            seen.insert(sequence);

            if (tracker.count(sequence) == 0) {
                tracker[sequence] = score;
            } else {
                tracker[sequence] = tracker[sequence] + score;
                score = tracker[sequence];
            }

            if (score > bestScore) {
                bestScore = score;
                bestSequence = sequence;
            }
        }
    }
    cout << "Best score: " << bestScore << "\n";
    cout << "Best sequence: " << get<0>(bestSequence) << ", " << get<1>(bestSequence) << ", " << get<2>(bestSequence) << ", " << get<3>(bestSequence) << "\n";

    return 0;
}

ll nextSecretNumber(ll n) {
    ll ans = n;

    ll i = ans * 64;
    ans = i ^ ans;
    ans = ans % 16777216;

    i = ans / 32;
    ans = i ^ ans;
    ans = ans % 16777216;

    i = ans * 2048;
    ans = i ^ ans;
    ans = ans % 16777216;

    return ans;
}
