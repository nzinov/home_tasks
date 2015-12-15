#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstring>
#include <string>
#include <random>

using namespace std;

const int ITER = 10;
const int DEPTH = 2;

enum color {BLACK, WHITE, NONE};

struct Coord {
    short x;
    short y;

    Coord(short x = 0, short y = 0) : x(x), y(y) {}
};

struct Data {
    int score;
    short depth;
    Coord best_move;

    Data(int score = 0, short depth = 0) : score(score), depth(depth) {}
};

struct Field {
    short color;
    bool passed;
    char field[8][8];

    Field() {
        for (short i = 0; i < 8; ++i) {
            for (short j = 0; j < 8; ++j) {
                field[i][j] = NONE;
            }
        }
        field[3][3] = WHITE;
        field[4][4] = WHITE;
        field[3][4] = BLACK;
        field[4][3] = BLACK;
        color = BLACK;
        passed = false;
    }

    inline short op() {
        return 1 - color;
    }

    void make_move(short move_x, short move_y) {
        for (short dx = -1; dx <= 1; dx++) {
            for (short dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) {
                    continue;
                }
                short x = move_x, y = move_y;
                for (x+=dx,y+=dy;x >= 0 && y >= 0 && x < 8 && y < 8; x+=dx,y+=dy) {
                    if (field[x][y] != op()) {
                        break;
                    }
                }
                if (x >= 0 && y >= 0 && x < 8 && y < 8 && field[x][y] == color) {
                    for (x-=dx,y-=dy; x != move_x || y != move_y; x-=dx,y-=dy) {
                        field[x][y] = color;
                    }
                }
            }
        }
        field[move_x][move_y] = color;
        color = op();
    }

    bool can_move(short move_x, short move_y) {
        for (short dx = -1; dx <= 1; dx++) {
            for (short dy = -1; dy <= 1; dy++) {
                short x = move_x, y = move_y;
                if (dx == 0 && dy == 0) {
                    continue;
                }
                bool reverse = false;
                for (x+=dx,y+=dy;x >= 0 && y >= 0 && x < 8 && y < 8; x+=dx,y+=dy) {
                    if (field[x][y] != op()) {
                        break;
                    }
                    reverse = true;
                }
                if (x >= 0 && y >= 0 && x < 8 && y < 8 && field[x][y] == color && reverse) {
                    return true;
                }
            }
        }
        return false;
    }

    void print()
    {
        cerr << endl;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                short cur = field[j][i];
                cerr << ((cur == 2) ? '.' : ((cur == 0) ? '*' : '&'));
            }
            cerr << endl;
        }
    }

    int score()
    {
        int ans = 0;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                short cur = field[i][j];
                switch (cur) {
                    case 0:
                        ++ans;
                        break;
                    case 1:
                        --ans;
                        break;
                }
            }
        }
        return ans;
    }

    bool operator==(const Field& other) const {
        if (color != other.color) {
            return false;
        }
        if (passed != other.passed) {
            return false;
        }
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (field[i][j] != other.field[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
};

struct HashField {
    size_t operator()(const Field& field) const {
        size_t ans = 0;
        for (size_t* p = (size_t*)(&field); p < (size_t*)(&field + 1); ++p) {
            ans ^= *p;
        }
        return ans;
    }
};

struct State {
    Field field;
    Data data;
};

struct Gamer {
    std::unordered_map<Field, Data, HashField> cache;
    Field position;

    inline void do_move(Coord coord)
    {
        string s;
        std::getline(cin, s);
        position.make_move(coord.x, coord.y);
        cout << "move " << (char)('a'+coord.x) << ' ' << coord.y+1 << endl;
    }

    void read_move() {
        string input;
        short x, y;
        std::getline(cin, input);
        x = input[5] - 'a';
        y = input[7] - '1';
        position.make_move(x, y);
    }

    inline void skip()
    {
        cerr << "skip";
    }

    int best_score(Field cur, int required_depth, bool make_move = false) {
        int score = -1000000;
        if (!make_move && cache.count(cur) && cache[cur].depth <= required_depth) {
            return cache[cur].score;
        }
        Coord best_move;
        bool has_move = false;
        if (required_depth == 0) {
            score = cur.score();
        } else {
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (cur.field[i][j] == NONE && cur.can_move(i, j)) {
                        has_move = true;
                        Field next = cur;
                        next.make_move(i, j);
                        int cur_score = best_score(next, required_depth-1);
                        if (cur.color == WHITE) {
                            cur_score = -cur_score;
                        }
                        if (cur_score > score) {
                            best_move = Coord(i, j);
                            score = cur_score;
                        }
                    }
                }
            }
            if (!has_move) {
                if (cur.passed) {
                    if (cur.score() > 0) {
                        score = 1000000;
                    } else {
                        score = -1000000;
                    }
                } else {
                    cur.color = cur.op();
                    cur.passed = true;
                    score = best_score(cur, required_depth-1);
                    cur.color = cur.op();
                    cur.passed = false;
                }
            }
             if (cur.color == WHITE) {
            score = -score;
        }
        }
        cache[cur] = Data(score, required_depth);
        if (make_move) {
            if (has_move) {
                do_move(best_move);
            } else {
                skip();
            }
        }
        return score;
    }

    void move() {
        best_score(position, 5, true);
    }
};

int main()
{
    srand(time(NULL));
    Gamer gamer;
    std::string input;
    getline(cin, input);
    bool black = input[5] == 'b';
    cerr << black;
    if (!black) {
        gamer.read_move();
    }
    while (true)
    {
        cerr << "step";
        gamer.position.print();
        gamer.move();
        gamer.read_move();
    }
    return 0;
}
