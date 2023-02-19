#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

// Algorithm's idea from https://cs.stackexchange.com/questions/157283/possibly-tractable-variation-of-suguru-puzzles?noredirect=1#comment330860_157283

bool algorithm(int M, int N, int cell[], int reg[]) {
    // Check whether there are no 2 adjacent cells that share a same number initially
    bool ok = true;
    for (int i = 2; i <= N; i++)
        ok &= cell[i] == 0 || cell[i-1] == 0 || (cell[i] != cell[i-1]);
    
    if (!ok) {
        return false;
        cout << "This Suguru instance have no solution.";
    }

    int reg_cnt = 0; // The number of regions in the Suguru instance
    for (int i = 1; i <= N; i++)
        reg_cnt = max(reg_cnt, reg[i]);

    vector<int> reg_size(reg_cnt + 1, 0);
    vector<vector<int>> reg_index(reg_cnt + 1);
    // reg_size[i] -> number of cells in i-th region
    // reg_index[i][j] -> the j-th cell index from i-th region in the cell and reg array
    for (int i = 1; i <= N; i++)
        reg_size[reg[i]]++, reg_index[reg[i]].push_back(i);

    vector<set<int>> init_choices(reg_cnt + 1);
    //init_choices[i] is a set of number that can be used to fill a cell in i-th region initially

    for (int i = 1; i <= reg_cnt; i++)
        for (int j = 1; j <= reg_size[i]; j++)
            init_choices[i].insert(j);
    
    //Removing all of the number that are initially used from the set of choices for each region
    for (int i = 1; i <= N; i++)
        if (cell[i] != 0)
            init_choices[reg[i]].erase(cell[i]);

    set<int> currentA, currentB;
    
    auto getLastCellIndex = [&](vector<int> vec) {
        return vec[vec.size() - 1];
    };

    auto getFirstCellIndex = [&](vector<int> vec) {
        return vec[0];
    };

    if (cell[getLastCellIndex(reg_index[1])] == 0) {
        //The right most cell of first region is empty
        currentA = init_choices[1];
    } else {
        currentA.insert(cell[getLastCellIndex(reg_index[1])]);
    }

    for (int i = 2; i <= reg_cnt; i++) {

        if ((int)init_choices[i].size() == 1) {
            for (int j = getFirstCellIndex(reg_index[i]); j <= getFirstCellIndex(reg_index[i]); j++) {
                if (cell[j] == 0) {
                    cell[j] = *init_choices[i].begin();
                    break;
                }
            }
        }

        if ((int)currentA.size() == 1) {
            if (*currentA.begin() == cell[getFirstCellIndex(reg_index[i])]) {
                return false;
            }

            if ((int)init_choices[i].size() == 2) {
                int fi = *init_choices[i].begin();
                int se = *init_choices[i].end();

                if (cell[getFirstCellIndex(reg_index[i])] == 0) {
                    if (*currentA.begin() == fi && cell[getLastCellIndex(reg_index[i])] == 0)
                        currentB.insert(fi);
                    if (*currentA.begin() == se && cell[getLastCellIndex(reg_index[i])] == 0)
                        currentB.insert(se);
                } else {
                    if (cell[getLastCellIndex(reg_index[i])] == 0) {
                        currentB.insert(fi);
                        currentB.insert(se);
                    }
                }

                if (cell[getLastCellIndex(reg_index[i])] != 0) {
                    currentB.insert(cell[getLastCellIndex(reg_index[i])]);
                }
            }
        } else {
            if (cell[getLastCellIndex(reg_index[i])] == 0) {
                currentB = init_choices[i];
            } else {
                currentB.insert(cell[getLastCellIndex(reg_index[i])]);
            }
        }
        currentA = currentB;
        currentB.clear();
    }

    return true;
}

int main() {
    HANDLE  hConsole;	
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int M, N; cin >> M >> N;
    assert(M == 1);

    int cell[N + 1], reg[N + 1];
    // reg[i] -> Region number of i-th cell
    // cell[i] = 0 -> i-th cell is empty cell initially
    // cell[i] = x, x != 0 -> i-th cell is a hint cell, filled with x
    for (int i = 1; i <= N; i++)
        cin >> reg[i];

    for (int i = 1; i <= N; i++)
        cin >> cell[i];
    
    cout << (algorithm(M, N, cell, reg) ? "Yes" : "No") << '\n';
}