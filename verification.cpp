#include <bits/stdc++.h>
#include <windows.h>
#include <iostream>
#include <unistd.h>
using namespace std;

vector<int> dx = {1, -1, 0, 0, 1, 1, -1, -1};
vector<int> dy = {0, 0, 1, -1, 1, -1, 1, -1};

int main() {
    HANDLE  hConsole;	
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int N, M;
    cin >> N >> M;
    vector<vector<pair<int,int>>> arr(N, vector<pair<int,int>>(M));
    vector<vector<int>> config(N, vector<int>(M, -1));

    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            cin >> arr[i][j].second;
    
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            cin >> arr[i][j].first;
        
    int last_region = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            last_region = max(last_region, arr[i][j].first);

    vector<vector<int>> B(last_region+1, vector<int>(N*M + 1, 0));

    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (arr[i][j].second != 0)
                B[arr[i][j].first][arr[i][j].second]++;

    vector<int> region_size(last_region+1, 0);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            region_size[arr[i][j].first]++;

    auto inside = [&](int r, int c) {
        return r >= 0 && r < N && c >= 0 && c < M;
    };

    auto is_valid_cell = [&](int r, int c) {
        bool all_valid = true;
        for (int i = 0; i < 8; i++)
            if (inside(r + dx[i], c + dy[i]))
                all_valid &= (config[r][c] != config[r+dx[i]][c+dy[i]]);
        all_valid &= B[arr[r][c].first][config[r][c]] == 1;
        all_valid &= 1 <= config[r][c] && config[r][c] <= region_size[arr[r][c].first];
        if (arr[r][c].second != 0)
            all_valid &= config[r][c] == arr[r][c].second;
        return all_valid;
    };

    auto verifier = [&]() {
        bool valid = true;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                valid &= is_valid_cell(i, j);
        return valid;
    };

    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) {
            cin >> config[i][j];
            if (arr[i][j].second == 0)
                B[arr[i][j].first][config[i][j]]++;
        }

    if (verifier()) {
        cout << "The configuration is indeed a solution!\n";
    } else {
        cout << "The configuration isn't a solution...\n";
    }
}