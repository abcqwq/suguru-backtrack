#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

int m, n;
vector<vector<pair<int,int>>> G, C;
vector<vector<int>> CV;
vector<int> s;

vector<int> dx = {1, -1, 0, 0, 1, 1, -1, -1};
vector<int> dy = {0, 0, 1, -1, 1, -1, 1, -1};
chrono::steady_clock::time_point start, finish;

void printSolution() {
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            cout << C[i][j].first << " \n"[j == n];
    long double timeTaken = chrono::duration_cast<chrono::microseconds>(finish - start).count();
    cout << "Time taken: "<< timeTaken/1000.0 << " ms" << '\n';
}

pair<int,int> nextCell(int i, int j) {
    return pair<int,int>((j == n ? i + 1 : i), (j == n ? 1 : j+1));
}

bool isInsideGrid(int i, int j) {
    return i >= 1 && i <= m && j >= 1 && j <= n;
}

bool mustBacktrack(int i, int j) {
    auto [h, r] = G[i][j];
    bool conditionA = false;

    for (int d = 0; d < 8; d++) {
        int i_ = i + dx[d], j_ = j + dy[d];
        if (isInsideGrid(i_, j_)) {
            auto [h_, r_] = G[i_][j_];
            conditionA |= (h == h_);
        }
    }    
    
    bool conditionB = (CV[r][h] > 1);
    
    return conditionA || conditionB;
}

void search(int i, int j) {
    if (i <= m) {
        auto [h, r] = G[i][j];

        if (h != 0) {
            auto [i_, j_] = nextCell(i, j);
            search(i_, j_);
        } else {
            for (int beta = 1; beta <= s[r]; beta++) {
                G[i][j] = pair<int,int>(beta, r);
                CV[r][beta]++;

                if (!mustBacktrack(i, j)) {
                    auto [i_, j_] = nextCell(i, j);
                    search(i_, j_);
                }

                G[i][j] = pair<int,int>(0, r);
                CV[r][beta]--;
            }
        }
    } else {
        finish = chrono::steady_clock::now();
        C = G;
        printSolution();
        exit(0);
    }
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
    readInput();
    search(1, 1);
    finish = chrono::steady_clock::now();

    cout << "The instance has no solution.";
    
    long double timeTaken = chrono::duration_cast<chrono::microseconds>(finish - start).count();
    cout << "Time taken: "<< timeTaken/1000.0 << " ms" << '\n';
}