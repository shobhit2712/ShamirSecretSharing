#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <cctype>
#include <iomanip>

using namespace std;

typedef long long ll;
typedef long double ld;

struct Point {
    ll x;
    ll y;
};


ll baseToDecimal(const string& s, int base) {
    ll value = 0;
    for (char c : s) {
        int digit = isdigit(c) ? c - '0' : toupper(c) - 'A' + 10;
        value = value * base + digit;
    }
    return value;
}


ll lagrangeInterpolation(const vector<Point>& points) {
    ld result = 0.0;
    int k = points.size();
    for (int i = 0; i < k; i++) {
        ld term = points[i].y;
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (0.0 - points[j].x) / (points[i].x - points[j].x);
            }
        }
        result += term;
    }
    return ll(round(result));
}

map<ll, pair<int, string>> parseJson(const string& filename, int& n, int& k) {
    ifstream file(filename);
    string line;
    map<ll, pair<int, string>> data;

    while (getline(file, line)) {
        if (line.find("\"n\"") != string::npos)
            n = stoi(line.substr(line.find(":") + 1));
        else if (line.find("\"k\"") != string::npos)
            k = stoi(line.substr(line.find(":") + 1));
        else if (line.find(": {") != string::npos) {
            ll x = stoll(line.substr(line.find("\"") + 1));
            string baseLine, valueLine;
            getline(file, baseLine);
            getline(file, valueLine);

            int base = stoi(baseLine.substr(baseLine.find(":") + 2));
            string val = valueLine.substr(valueLine.find(":") + 2);
            val = val.substr(0, val.find("\"", 1));

            data[x] = make_pair(base, val);
        }
    }
    return data;
}

ll computeSecret(const string& filename) {
    int n, k;
    map<ll, pair<int, string>> input = parseJson(filename, n, k);

    vector<Point> selected;
    int count = 0;
    for (map<ll, pair<int, string>>::iterator it = input.begin(); it != input.end(); ++it) {
        if (count >= k) break;
        ll x = it->first;
        int base = it->second.first;
        string value = it->second.second;
        ll y = baseToDecimal(value, base);
        selected.push_back({x, y});
        count++;
    }

    return lagrangeInterpolation(selected);
}

int main() {
    string files[2] = {"testcase1.json", "testcase2.json"};

    for (int i = 0; i < 2; i++) {
        ll secret = computeSecret(files[i]);
        cout << "Secret (c) from " << files[i] << ": " << secret << endl;
    }

    return 0;
}
