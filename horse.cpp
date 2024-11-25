#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
using namespace std;

const int N = 8;

int board[N][N];

int finalX = -1, finalY = -1;

//Ходы коня
int dx[] = {2, 1, -1, -2, -2, -1, 1, 2};
int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};

//Проверка, находится ли клетка в пределах доски и свободна ли она
bool isValid(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N && board[x][y] == -1;
}

//Подсчёт количества возможных ходов из клетки (эвристика Варнсдорфа)
int countMoves(int x, int y) {
    int count = 0;

    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValid(nx, ny)) {
            ++count; 
        }
    }
    return count;
}

// Функция для поиска пути (рекурсивная)
bool solve(int x, int y, int move) {
    //Присваиваю текущей клетке номер хода
    board[x][y] = move;

    if (move == N * N - 1) {
        finalX = x+1;
        finalY = y+1;
        return true;
    }

    //Список всех возможных ходов из текущей клетки
    vector<pair<int, pair<int, int>>> moves;

    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValid(nx, ny)) {
            moves.push_back({countMoves(nx, ny), {nx, ny}});
        }
    }

    sort(moves.begin(), moves.end());

    //Пробуем каждый возможный ход
    for (auto &m : moves) {
        int nx = m.second.first;
        int ny = m.second.second;
        if (solve(nx, ny, move + 1)) {
            return true;
        }
    }

    //Если ни один ход не сработал, откатываемся
    board[x][y] = -1;
    return false;
}


void printBoard() {
    cout << endl;
    cout << "    ";
    for (int i = 1; i <= N; ++i) {
        cout << setw(3) << i << " "; 
    }
    cout << endl;
    cout << "    --------------------------------" << endl;

    for (int i = 1; i <= N; ++i) {
        cout << setw(2) << i << " |";  
        for (int j = 1; j <= N; ++j) {
            cout << setw(3) << board[i-1][j-1] << " "; 
        }
        cout << endl;
    }
}


int main() {
    setlocale(0, "RUS");

    int x0 = 5, y0 = 5;
    cout << "Введите начальные координаты: (пример: 1 2)" << endl;
    cin >> x0 >> y0;
    cout << endl;

    fill(&board[0][0], &board[0][0] + sizeof(board) / sizeof(int), -1);

    auto start = chrono::high_resolution_clock::now();

    if (solve(x0-1, y0-1, 0)) {
        cout << "Начальные координаты: " << "(" << x0 << "," << y0 << ")" << endl;
        printBoard();
        cout << endl;
        cout << "Конечная точка (координаты последнего хода): (" << finalX << ", " << finalY << ")" << endl;
    } else {
        cout << "Решение не найдено." << endl;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> dur = end - start;
    cout << "Решение найдено за " << dur.count() << " секунд." << endl;
    return 0;
}
