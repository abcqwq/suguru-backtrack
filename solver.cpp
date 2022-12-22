#include <bits/stdc++.h>
#include <windows.h>
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
        return all_valid;
    };

    auto verifier = [&]() {
        bool valid = true;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                valid &= is_valid_cell(i, j);
        return valid;
    };

    bool found = false;
    function<void(int)> search = [&](int num) {

        if (num >= N*M) {
            found = verifier();
            return;
        }

        int er = num / M;
        int ce = num % M;

        if (arr[er][ce].second != 0) {
            config[er][ce] = arr[er][ce].second;
            search(num+1);
        } else {
            for (int s = 1; s <= region_size[arr[er][ce].first]; s++) {
                config[er][ce] = s;
                B[arr[er][ce].first][s]++;

                if (is_valid_cell(er, ce)) {
                    search(num+1);
                }

                if (found)
                    return;

                B[arr[er][ce].first][s]--;
                config[er][ce] = -1;
            }
        }
    };

    search(0);

    if (found) {
        cout << "Found!\n";
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++) {
                SetConsoleTextAttribute(hConsole, arr[i][j].first + 1);
                cout << config[i][j] << " \n"[j == M-1];
            }
    } else {
        cout << "No solution found for this Suguru instance.\n";
    }


}