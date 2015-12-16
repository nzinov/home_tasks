#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstring>
#include <string>
#include <random>
#include <cstdlib>

using namespace std;

const time_t TICKS = CLOCKS_PER_SEC * 11 / 4;
const time_t MIN_TICKS = CLOCKS_PER_SEC;

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

    Data(int score = 0, short depth = -1) : score(score), depth(depth) {}
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

    void skip() {
         color = op();
         passed = true;
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
        passed = false;
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
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                short cur = field[j][i];
            }
        }
    }

    int coef() const {
        return coef(color);
    }

    int coef(short color) const {
        return color == BLACK ? 1 : -1;
    }

    int score() const
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
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                if (field[i*7][j*7] != NONE) {
                    ans += 10*coef(field[i*7][j*7]);
                }
            }
        }
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                if (i == 0) {
                for (int a = 0; a < 8; ++a) {
                    if (field[i*7+a][j*7] != NONE) {
                        ans += 6*coef(field[i*7+a][j*7]);
                    }
                }
                }
                if (j == 0) {
                for (int b = 0; b < 8; ++b) {
                    if (field[i*7][j*7+b] != NONE) {
                        ans += 6*coef(field[i*7][j*7+b]);
                    }
                }
                }
            }
        }
        return ans;
    }

    bool operator==(const Field& other) const {
        return std::memcmp(this, &other, sizeof other) == 0;
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

struct Move {
    Field field;
    Coord move;
    int score;
};

struct Gamer {
    std::unordered_map<Field, Data, HashField> cache;
    Field position;
    short my_color;
    Coord move;
    short save_depth;
    time_t start;
    bool collapse;

    inline int get_score(const Field& pos) {
        Data& data = cache[pos];
        if (data.depth == -1) {
            data.score = pos.score();
            data.depth = 0;
        }
    }

    inline void do_move()
    {
        if (move.x == -1) {
            skip();
            return;
        }
        position.make_move(move.x, move.y);
        cout << "move " << (char)('a'+move.x) << ' ' << move.y+1 << endl;
    }

    void process_move() {
        string input;
        short x, y;
        std::getline(cin, input);
        if (input[0] != 't' && input[0] != 'm') {
            exit(0);
        }
        if (input[0] == 't') {
            if (position.color != my_color) {
                position.skip();
            }
            find_move();
        } else {
            if (position.color != 1 - my_color) {
                position.skip();
            }
            x = input[5] - 'a'; y = input[7] - '1';
            position.make_move(x, y);
        }
    }

    inline void skip()
    {
        position.skip();
    }

    int best_score(Field cur, int required_depth, int my = -100000, int opponent = 100000, bool make_move = false) {
        if (clock() > start) {
            return 0;
        }
        int score = -1000000;
        if (required_depth > 4) {
        }
        if (!make_move && cache.count(cur) && cache[cur].depth >= required_depth) {
           return cache[cur].score;
        }
        Coord best_move;
        bool has_move = false;
        if (required_depth == 0) {
            score = cur.score();
        } else {
            Move moves[64];
            short n = 0;
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (cur.field[i][j] == NONE && cur.can_move(i, j)) {
                        has_move = true;
                        moves[n].field = cur;
                        moves[n].field.make_move(i, j);
                        moves[n].move.x = i;
                        moves[n].move.y = j;
                        moves[n].score = get_score(moves[n].field);
                        ++n;
                    }
                }
            }
            for(int i = 1; i < n; ++i) {
                for(int j = i; j > 0 && moves[j-1].score < moves[j].score; --j) {
                    swap(moves[j-1], moves[j]);
                }
            }
            for (int i = 0; i < n; ++i) {
                int cur_score = best_score(moves[i].field, required_depth-1, -opponent, -my);
                if (collapse) {
                    return 0;
                }
                if (cur.color == WHITE) {
                    cur_score = -cur_score;
                }
                if (cur_score > score) {
                    best_move = moves[i].move;
                    score = cur_score;
                }
                if (score > my) {
                    my = score;
                    if (opponent <= my) {
                        break;
                    }
                }
            }
            if (cur.color == WHITE) {
                score = -score;
            }
            if (!has_move) {
                if (cur.passed) {
                    score = cur.score()*100;
                } else {
                    Field next = cur;
                    next.skip();
                    score = best_score(next, required_depth-1, -opponent, -my);
                }
            }
        }
        cache[cur] = Data(score, required_depth);
        if (make_move) {
            if (has_move) {
                move = best_move;
            } else {
                move = Coord(-1, -1);
            }
        }
        return score;
    }

    void find_move() {
        start = clock() + TICKS;
        best_score(position, 2, -100000, 100000, true);
        best_score(position, save_depth, -100000, 100000, true);
        do_move();
        if (collapse) {
            save_depth--;
        } else if (start > clock() && start - clock() > MIN_TICKS) {
            save_depth++;
        }
        collapse = false;
    }
};

int main()
{
    srand(time(NULL));
    std::string input;
    getline(cin, input);
    short color = input[5] == 'w';
    Gamer gamer;
    gamer.my_color = color;
    gamer.collapse = false;
    gamer.save_depth = 4;
    while (true)
    {
        gamer.process_move();
    }
    return 0;
}
