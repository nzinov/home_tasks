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

const time_t TICKS = CLOCKS_PER_SEC * 14 / 5;
const time_t MIN_TICKS = CLOCKS_PER_SEC * 2;
const time_t MAX_TICKS = CLOCKS_PER_SEC / 2;
const time_t MAX_TICKS_PASSES = CLOCKS_PER_SEC;
int PASSES = 30;

bool is_corner(short x, short y) {
    return (((x == 0) || (x == 7)) && ((y == 0) || (y == 7)));
}

bool is_middle(short x, short y) {
    return (x >= 2) && (x <= 5) && (y >= 2) && (y <= 5);
}

bool is_pre_corner(short x, short y) {
    return (((x == 1) || (x == 6)) && ((y == 0) || (y == 1) || (y == 6) || (y == 7))) || (((x == 0) || (x == 7)) && ((y == 1) || (y == 6)));
}

bool is_side(short x, short y) {
    return ((x == 0) || (x == 7) || (y == 0) || (y == 7)) && !is_pre_corner(x, y) && !is_corner(x, y);
}

bool is_pre_side(short x, short y) {
    return ((x == 1) || (x == 6) || (y == 1) || (y == 6)) && !is_pre_corner(x, y);
}

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

    bool can_move(short move_x, short move_y, short use_color = -1) const {
        if (use_color == -1) {
            use_color = color;
        }
        for (short dx = -1; dx <= 1; dx++) {
            for (short dy = -1; dy <= 1; dy++) {
                short x = move_x, y = move_y;
                if (dx == 0 && dy == 0) {
                    continue;
                }
                bool reverse = false;
                for (x+=dx,y+=dy;x >= 0 && y >= 0 && x < 8 && y < 8; x+=dx,y+=dy) {
                    if (field[x][y] != 1 - use_color) {
                        break;
                    }
                    reverse = true;
                }
                if (x >= 0 && y >= 0 && x < 8 && y < 8 && field[x][y] == use_color && reverse) {
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

    int real_score() const {
        int ans = 0;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                short cur = field[i][j];
                switch (cur) {
                    case BLACK:
                        ++ans;
                        break;
                    case WHITE:
                        --ans;
                        break;
                }
            }
        }
        return ans;
    }
    int score() const
    {
        int ans = 0;
        for (int i = 0; i < PASSES; i++)
        {
           ans += simulate();
        }
        ans = ans*1000/PASSES;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (field[i][j] == NONE && can_move(i, j, 1 - color)) {
                    ans -= 500*coef();
                }
                if (field[i][j] == NONE && can_move(i, j)) {
                    ans += 400*coef();
                }
                int rank = 0;
                if (is_corner(i, j)) {
                    rank = 10000;
                } else if (is_pre_corner(i, j)) {
                    rank = -5000;
                } else if (is_side(i, j)) {
                    rank = 1000;
                } else if (is_pre_side(i, j)) {
                    rank = -800;
                }
                ans += coef(field[i][j])*rank;
            }
        }
        return ans;
    }

    int simulate() const {
        Field cur = *this;
        while (true) {
            bool has_move = false;
            vector<Coord> moves;
            moves.reserve(5);
            short n = 0;
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (cur.field[i][j] == NONE && cur.can_move(i, j)) {
                        has_move = true;
                        moves.emplace_back(i, j);
                        ++n;
                    }
                }
            }
            if (has_move) {
                int ind = rand() % n;
                cur.make_move(moves[ind].x, moves[ind].y);
            } else {
                if (cur.passed) {
                    return cur.real_score();
                } else {
                    cur.skip();
                }
            }
        }
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
        return data.score;
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

    int best_score(Field cur, int required_depth, int my = -100000000, int opponent = 100000000, bool make_move = false) {
        if (clock() > start) {
            collapse = true;
            return 0;
        }
        int score = -1000000000;
        if (!make_move && cache.count(cur) && cache[cur].depth >= required_depth) {
           return cache[cur].score;
        }
        Coord best_move;
        bool has_move = false;
        if (required_depth == 0) {
            score = get_score(cur);
        } else {
            vector<Move> moves;
            moves.reserve(5);
            short n = 0;
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (cur.field[i][j] == NONE && cur.can_move(i, j)) {
                        has_move = true;
                        moves.push_back(Move());
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
            short new_depth = required_depth-1;
            for (int i = 0; i < n; ++i) {
                if (i % 3 == 0 && new_depth > 1) {
                    new_depth--;
                }
                int cur_score = best_score(moves[i].field, new_depth, -opponent, -my);
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
                    score = cur.real_score()*1000000;
                } else {
                    Field next = cur;
                    next.skip();
                    score = best_score(next, required_depth, -opponent, -my);
                }
            }
        }
        cache[cur] = Data(score, required_depth);
        if (!collapse && make_move) {
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
        time_t left = start > clock() ? start - clock() : 0;
        if (collapse || left <= MAX_TICKS) {
            save_depth--;
            PASSES = 30;
        } else if (left > MIN_TICKS) {
            save_depth++;
            PASSES = 30;
        } else if (left > MAX_TICKS_PASSES) {
            PASSES += 5;
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
