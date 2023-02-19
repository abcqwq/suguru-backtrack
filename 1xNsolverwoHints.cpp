#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

int main() {
    HANDLE  hConsole;	
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int N; cin >> N;
    int reg[N];
    for (int i = 0; i < N; i++)
        cin >> reg[i];
    int arr[N]; arr[0] = 1;
    
    int now = 2;
    for (int i = 1; i < N; i++) {
        while (i < N && reg[i] == reg[i-1]) {
            arr[i] = now;
            now++;
            i++;
        }
        cout << i << '\n';
        if (i >= N)
            break;
        now = 2;
        arr[i] = 1;
    }

    // for (int i = 0; i < N; i++) {
    //     cout << arr[i] << " \n"[i == N-1];
    // }

    for (int i = 0; i < N-2; i++)
        if (arr[i] == arr[i+1])
            swap(arr[i+1], arr[i+2]);
        
    bool solved = true;
    for (int i = 0; i < N-1; i++)
        if (arr[i] == arr[i+1])
            solved = false;
    
    cout << (solved ? "Solved!" : "Impossible!") << '\n';
    for (int i = 0; i < N; i++) {
        SetConsoleTextAttribute(hConsole, (reg[i] % 16) + 3);
        cout << arr[i] << " \n"[i == N-1];
    }
}