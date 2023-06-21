#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

int m, n;
vector<vector<pair<int,int>>> G;
vector<vector<int>> CV;
vector<int> s;

vector<int> dx = {1, -1, 0, 0, 1, 1, -1, -1};
vector<int> dy = {0, 0, 1, -1, 1, -1, 1, -1};
chrono::steady_clock::time_point start, finish;

bool isInsideGrid(int i, int j) {
    return i >= 1 && i <= m && j >= 1 && j <= n;
}

bool isComplyingValuesWithinRegionRule(int i, int j) {
    return 1 <= G[i][j].first && G[i][j].first <= s[G[i][j].second]; 
}

bool isUniqueCellWithinRegion(int i, int j) {
    int alpha = G[i][j].second, beta = G[i][j].first;
    return CV[alpha][beta] == 1;
}

bool isComplyingAdjacentCellValuesRule(int i, int j) {
    auto [h, r] = G[i][j];
    bool isComplying = true;

    for (int d = 0; d < 8; d++) {
        int i_ = i + dx[d], j_ = j + dy[d];
        if (isInsideGrid(i_, j_)) {
            auto [h_, r_] = G[i_][j_];
            isComplying &= (h != h_);
        }
    }  
    
    return isComplying;
}

void readInput() {
    cin >> m >> n;
    G.resize(m+1, vector<pair<int,int>>(n+1));

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            cin >> G[i][j].first;
        
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            cin >> G[i][j].second;
    
    start = chrono::steady_clock::now();

    int R = 0;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            R = max(R, G[i][j].second);

    s.resize(R+1, 0);
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++) {
            int k = G[i][j].second;
            s[k]++;
        }

    CV.resize(R+1);
    for (int r = 1; r <= R; r++)
        CV[r].resize(s[r]+1, 0);
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++) 
            if (G[i][j].first != 0) {
                int alpha = G[i][j].second, beta = G[i][j].first;
                CV[alpha][beta]++;
            }
}

int main() {
    bool isCorrect = true;

    readInput();

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            isCorrect &= (isComplyingValuesWithinRegionRule(i, j) && isUniqueCellWithinRegion(i, j) && isComplyingAdjacentCellValuesRule(i, j));

    finish = chrono::steady_clock::now();

    cout << "The solution is " << (isCorrect ? "" : "not ") << "correct" << '\n';
    
    long double timeTaken = chrono::duration_cast<chrono::microseconds>(finish - start).count();
    cout << "Time taken: "<< timeTaken/1000.0 << " ms" << '\n';
}