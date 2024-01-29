// See: https://arxiv.org/abs/2309.12985
// An example of asymptotic worst-case depth for a 3-letter word in a Fractal Word Search on an alphabet of size N.
// Given A < B < C <= N-3, generates a configuration with a word that doesn't appear until depth LCM(A,B,C)+1.
// This gives a lower bound of (N-4)(N-5)(N-6)+1 (i.e. Ω(N^3)) for the worst-case depth for any N.
// Runs a simple search to print out the depth of first occurrence for all 2x2 patterns and 3-letter words.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
using namespace std;

const int N = 10, A = 5, B = 6, C = 7;

typedef vector<vector<int>> VVI;
ostream& operator<<(ostream& out, const VVI& v) {
  out << v[0][0] << ',' << v[0][1] << '/' << v[1][0] << ',' << v[1][1];
  return out;
}

VVI Start;
vector<VVI> Rules(N);

void GenerateRules() {
  assert(3 <= A && A < B && B < C && C <= N-3);
  Start = VVI{{0,1},{1,2}};
  Rules = vector<VVI>(N, {{0,0},{0,0}});
  for (int i = 1; i < A; i++) Rules[i][0][0] = i+1;
  for (int i = 1; i < B; i++) Rules[i][0][1] = i+1;
  for (int i = 1; i < C; i++) Rules[i][1][0] = i+1;
  Rules[A][0][0] = Rules[B][0][1] = Rules[C][1][0] = 1;
  Rules[B][1][1] = N-2;
  Rules[C][1][1] = N-1;

  cout << "Start: " << Start << endl;
  for (int i = 0; i < N; i++) cout << "Rule: " << i << " -> " << Rules[i] << endl;
  cout << endl;
}

map<vector<vector<int>>, int> PatternLevel;
map<vector<int>, int> WordLevel;
priority_queue<pair<int, vector<vector<int>>>> Q;

void Add(const VVI& v, int d) {
  if (PatternLevel.count(v)) return;
  PatternLevel[v] = d;
  Q.push({-d, v});
  cout << "Depth " << d << ": First occurrence of pattern " << v << endl;
}

void Expand(const VVI& v, int d) {
  VVI v2(4, vector<int>(4));
  for (int y = 0; y < 4; y++)
  for (int x = 0; x < 4; x++) {
    v2[y][x] = Rules[v[y/2][x/2]][y%2][x%2];
  }
  for (int y = 0; y < 3; y++)
  for (int x = 0; x < 3; x++) {
    Add({{v2[y][x], v2[y][x+1]}, {v2[y+1][x], v2[y+1][x+1]}}, d+1);
  }
  for (int dy = -1; dy <= 1; dy++)
  for (int dx = -1; dx <= 1; dx++) if (dy || dx) {
    for (int y = 0; y < 4; y++)
    for (int x = 0; x < 4; x++) {
      vector<int> word;
      for (int i = 0, x2 = x, y2 = y; i < 3; i++, x2 += dx, y2 += dy) {
        if (x2 >= 0 && x2 < 4 && y2 >= 0 && y2 < 4) word.push_back(v2[y2][x2]);
      }
      if (word.size() == 3 && !WordLevel.count(word)) {
        WordLevel[word] = d+1;
        cout << "Depth " << d+1 << ": First occurrence of word";
        for (auto x : word) cout << ' ' << x;
        cout << endl;
      }
    }
  }
}

int main() {
  GenerateRules();
  Add(Start, 1);
  while (!Q.empty()) {
    auto [d, v] = Q.top();
    Q.pop();
    Expand(v, -d);
  }
}
