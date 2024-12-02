#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <windows.h>
#include <conio.h>

typedef unsigned char UCHR;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
class map {
public:
    int height, width;
    int cursorX = 0, cursorY = 0;
    int not_revealed, mines;
    int marked_count = 0; // Đếm số ô đã được đánh dấu
    int** minemap;
    bool** revealed;
    bool** marked; // Mảng lưu trạng thái đánh dấu

public:
    void inc(int x, int y, int h, int w);
    void spawnmines(int h, int w, int minec);
    void reveal(int x, int y);
    void toggle_mark(int x, int y); // Hàm đánh dấu hoặc bỏ đánh dấu
    void ingame_print();
    int controller();
    void failed();
    void succeed();
    map(int h, int w, int minecount) : height(h), width(w), mines(minecount) {
        srand(time(0));
        while (h <= 0 || w <= 0 || minecount <= 0) {
            std::cout << "K DC DAU, HAY NHAP LAI\n";
            std::cout << "NHAP SO HANG: ";
            std::cin >> h;
            std::cout << "NHAP SO COT: ";
            std::cin >> w;
            std::cout << "NHAP SO LUONG BOM: ";
            std::cin >> minecount;
            system("cls");
        }
        this->height = h, this->width = w; mines = minecount;
        if (h * w <= minecount * 4 || minecount <= 0) {
            minecount = int(h * w / 4);
            printf("KO CHOI DC DAU, SO LUONG BOM DAT THANH %d.\n", minecount);
        }
        not_revealed = h * w;
        minemap = new int* [h];
        revealed = new bool* [h];
        marked = new bool* [h]; // Khởi tạo mảng marked
        for (int i = 0; i < h; ++i) {
            minemap[i] = new int[w];
            revealed[i] = new bool[w];
            marked[i] = new bool[w]; // Khởi tạo
            std::fill(minemap[i], minemap[i] + w, 0);
            std::fill(revealed[i], revealed[i] + w, false);
            std::fill(marked[i], marked[i] + w, false); // Đặt tất cả ô chưa được đánh dấu
        }
    }
    ~map() {
        for (int i = 0; i < this->height; ++i) {
            delete[] minemap[i];
            delete[] revealed[i];
            delete[] marked[i]; // Giải phóng mảng marked
        }
        delete[] minemap;
        delete[] revealed;
        delete[] marked;
    }
};

void map::toggle_mark(int x, int y) {
    int remaining_flags = mines - marked_count;

    if (!revealed[y][x]) {
        if (marked[y][x]) {
            marked[y][x] = false;
            --marked_count; // Bỏ đánh dấu
        }
        else if (remaining_flags > 0) { // Chỉ cho phép đánh dấu nếu còn cờ
            marked[y][x] = true;
            ++marked_count; // Đánh dấu
        }
        else {
            
        }
    }
}


void map::spawnmines(int h, int w, int minec) {
    while (minec--) {
        int x = rand() % w, y = rand() % h;
        while (minemap[y][x] == -1 || (x == cursorX && y == cursorY)) x = rand() % w, y = rand() % h;
        minemap[y][x] = -1;
        inc(x, y, h, w);
    }
    return;
}
void map::inc(int x, int y, int h, int w) {
    for (int i = x - 1; i <= x + 1; ++i) {
        for (int j = y - 1; j <= y + 1; ++j) {
            if (0 <= i && i < w && 0 <= j && j < h && minemap[j][i] != -1) ++minemap[j][i];
        }
    }
    return;
}
void map::reveal(int x, int y) {
    revealed[y][x] = true; --not_revealed;
    if (x >= 0 && x < this->width && y >= 0 && y < this->height && minemap[y][x] == 0) {
        if (x > 0) {
            if (revealed[y][x - 1] == false) reveal(x - 1, y);
        }
        if (x < this->width - 1) {
            if (revealed[y][x + 1] == false) reveal(x + 1, y);
        }
        if (y > 0) {
            if (revealed[y - 1][x] == false) reveal(x, y - 1);
        }
        if (y < this->height - 1) {
            if (revealed[y + 1][x] == false) reveal(x, y + 1);
        }
        if (x > 0 && y > 0) {
            if (revealed[y - 1][x - 1] == false) reveal(x - 1, y - 1);
        }
        if (y < this->height - 1 && x < this->width - 1) {
            if (revealed[y + 1][x + 1] == false) reveal(x + 1, y + 1);
        }
        if (y < this->height - 1 && x>0) {
            if (revealed[y + 1][x - 1] == false) reveal(x - 1, y + 1);
        }
        if (x < this->width - 1 && y>0) {
            if (revealed[y - 1][x + 1] == false) reveal(x + 1, y - 1);
        }
    }
    else return;
}

void exit_game() {
    system("cls");
    system("color 0E"); // Màu vàng để làm nổi bật thông báo
    std::cout << "___________________________________________\n";
    std::cout << "|--------------- TAM BIET! ---------------|\n";
    std::cout << "|                                         |\n";
    std::cout << "|  CAM ON BAN DA CHOI MINESWEEPER!        |\n";
    std::cout << "|-----------------------------------------|\n";
    std::cout << "|     An phim bat ky de ket thuc...       |\n";
    std::cout << "___________________________________________\n";
    _getch(); // Chờ người chơi nhấn phím trước khi thoát
    exit(0);  // Thoát chương trình
}


bool end(int value = 0) {
    if (value = 0) return false;
    return true;
}

void map::ingame_print() {
    UCHR m = 254, nr = 178, mark_symbol = '@';
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->width; ++j) {
            if (i == this->cursorY && j == this->cursorX) {
                SetConsoleTextAttribute(hConsole, 6);
                std::cout << "+ ";
                SetConsoleTextAttribute(hConsole, 7);
            }
            else if (marked[i][j]) {
                std::cout << mark_symbol << " "; // Hiển thị ô đánh dấu là '@'
            }
            else if (revealed[i][j]) {
                if (minemap[i][j] == -1) std::cout << m << " ";
                else std::cout << minemap[i][j] << " ";
            }
            else {
                std::cout << nr << " ";
            }
        }
        std::cout << "\n";
    }
    int remaining_flags = mines - marked_count;
    std::cout << "\nSo co con lai: " << remaining_flags << "\n";
}

int map::controller() {
    char cmd;
    bool need_to_spawn = true;

    while (true) {
        ingame_print();
        cmd = _getch();
        system("cls");
        switch (cmd) {
        case 'a':
            cursorX = (cursorX == 0) ? 0 : (cursorX - 1);
            break;
        case 'd':
            cursorX = (cursorX == this->width - 1) ? cursorX : (cursorX + 1);
            break;
        case 'w':
            cursorY = (cursorY == 0) ? 0 : (cursorY - 1);
            break;
        case 's':
            cursorY = (cursorY == this->height - 1) ? cursorY : (cursorY + 1);
            break;
        case ' ':
            if (need_to_spawn) {
                spawnmines(this->height, this->width, this->mines);
                need_to_spawn = false;
            }
            if (!marked[cursorY][cursorX]) { // Không cho phép mở ô đã được đánh dấu
                reveal(cursorX, cursorY);
                if (minemap[cursorY][cursorX] == -1) return 0; // Thua nếu chạm bom
            }
            break;
        case 'm':
            toggle_mark(cursorX, cursorY);
            break;
        }

        // Kiểm tra điều kiện thắng
        if (marked_count == mines) {
            bool all_correct = true;
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    if (marked[i][j] && minemap[i][j] != -1) {
                        all_correct = false;
                        break;
                    }
                }
                if (!all_correct) break;
            }
            if (all_correct) return 1; // Thắng nếu tất cả các cờ đúng vị trí bom
        }
    }
}

int start();

void map::failed() {
    system("cls");
    system("color 04");
    UCHR nr = 178;
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->width; ++j) {
            if (minemap[i][j] == -1) { std::cout << "X" << " "; }
            else if (revealed[i][j] == true) { std::cout << minemap[i][j] << " "; }
            else std::cout << nr << " ";
        }
        printf("\n");
    }
    std::cout << "___________________________________________\n";
    std::cout << "|-------------BAN DA THAT BAI--------------|\n";
    std::cout << "|  An phim 1 de choi lai.                  |\n";
    std::cout << "|  An phim bat ky de thoat.                |\n";
    std::cout << "___________________________________________\n";
    char tmp;
    tmp = _getch();
    switch (tmp)
    {
    case '1':
        system("cls");
        start();
        break;
    default:
        exit_game();
        break;
    }
    return;
}
void map::succeed() {
    system("cls");
    system("color 09");
    UCHR m = 254;
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->width; ++j) {
            if (minemap[i][j] == -1) std::cout << m << " ";
            else std::cout << minemap[i][j] << " ";
        }
        printf("\n");
    }
    std::cout << "___________________________________________\n";
    std::cout << "|-------------BAN DA CHIEN THANG-----------|\n";
    std::cout << "|  An phim 1 de choi lai.                  |\n";
    std::cout << "|  An phim bat ky de thoat.                |\n";
    std::cout << "___________________________________________\n";
    char tmp;
    tmp = _getch();
    switch (tmp)
    {
    case '1':
        system("cls");
        start();
        break;
    default:
        exit_game();
        break;
    }
}

int start() {
    SetConsoleTextAttribute(hConsole, 7);
    std::cout << "_______________________________________________________\n";
    std::cout << "|------------ MINESWEEPER CONSOLE EDITION ------------|\n";
    std::cout << "|HUONG DAN: ------------------------------------------|\n";
    std::cout << "|--- [ADSW] DI CHUYEN --------------------------------|\n";
    std::cout << "|--- [SPACE] DAO -------------------------------------|\n";
    std::cout << "|--- SO TRONG 1 O CHI RA SO BOM TRONG 8 O ------------|\n";
    std::cout << "|--- XUNG QUANH O DO ---------------------------------|\n";
    std::cout << "|-------------NHAN PHIM BAT KI DE DI TIEP-------------|\n";
    std::cout << "_______________________________________________________\n";
    _getch();
    system("cls");
    std::cout << "_______________________________________________________\n";
    std::cout << "|----------------CHON DO KHO:-------------------------|\n";
    std::cout << "|--- [1] DE VL: MAP 9X9, 10 BOM ----------------------|\n";
    std::cout << "|--- [2] BTHUONG: MAP 16X16, 40 BOM ------------------|\n";
    std::cout << "|--- [3] KHO: MAP 20X30, 99 BOM ----------------------|\n";
    std::cout << "|--- [Q] THOAT CHUONG TRINH --------------------------|\n";
    std::cout << "|--- [PHIM BAT KI] CUSTOM ----------------------------|\n";
    std::cout << "_______________________________________________________\n";
    int cmd = _getch() - '0', heigheight, width, minecount;
    switch (cmd) {
    case 1: { heigheight = width = 9, minecount = 10; break; }
    case 2: { heigheight = width = 16, minecount = 40; break; }
    case 3: { heigheight = 20, width = 30, minecount = 99; break; }
    case 'q': { exit_game(); break; }
    default: {
        std::cout << "NHAP SO HANG: ";
        std::cin >> heigheight;
        std::cout << "NHAP SO COT: ";
        std::cin >> width;
        std::cout << "NHAP SO LUONG BOM: ";
        std::cin >> minecount;
        break;
    }
    }
    system("cls");
    map mines(heigheight, width, minecount);
    //mines.ingame_print();
    int RESULT = mines.controller();
    if (RESULT == 0) mines.failed();
    else mines.succeed();
    mines.~map();
    system("cls");
    return 0;
}

int main() {
    start();
    return 0;
}