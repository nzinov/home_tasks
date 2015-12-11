#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>

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
    char field[8][8];

    Field() {
        for (short i = 0; i < 8; ++i) {
            for (short j = 0; j < 8; ++j) {
                field[i][j] = NONE;
            }
        }
        field[3][3] = BLACK;
        field[4][4] = BLACK;
        field[3][4] = WHITE;
        field[4][3] = WHITE;
        color = BLACK;
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
                short cur = field[i][j];
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
                        --ans;
                        break;
                    case 1:
                        ++ans;
                        break;
                }
            }
        }
        return ans;
    }

    bool operator==(const Field& other) {
        return std::equal(this, this + sizeof this / sizeof *this, &other);
    }
};

struct State {
    Field field;
    Data data;
};

class Gamer {
    std::unordered_map<Field, Data> cache;
    short my_color;
    Field position;

    inline void do_move(short x, short y)
    {
        make_move(x, y);
        cout << (char)('a'+x) << y+1 << endl;
    }

    inline void skip()
    {
        cout << "Skip" << endl;
    }

    int best_score(Field cur, int required_depth) {
        int score = -1000000;
        if (cache.count(cur) && cache[cur].depth <= required_depth) {
            return cache[cur].score;
        }
        if (required_depth == 0) {
            score = cur.score();
        } else {
            Coord best_move;
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (cur.field[i][j] == NONE && cur.can_move(i, j)) {
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
        }
        if (cur.color == WHITE) { 
            score = -score;
        }
        cache[cur] = Data(score, required_depth);
        return score;
    }
};


int cost(f field, bool black)
{
    int count = 0;
    for (int i = 0; i < ITER; i++)
    {
        count += simulate(field, black);
    }
    return count;
}
int move_white(f& field, int a = -1000000, int b = 1000000, int depth = 0, bool passed=false);
int move_black(f& field, int a = -1000000, int b = 1000000, int depth = 0, bool passed=false)
{
    cerr << "move_black(a=" << a << ", b=" << b << ", depth=" << depth << endl;
    if (depth == DEPTH)
        return cost(field, true);
    int max_cost = -1000000;
    cord best_move(-1, -1);
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
        {
            if (at(field, x, y) == 2 && can_move(field, true, x, y))
            {
                f temp = field;
                make_move(temp, true, x, y);
                int cur = move_white(temp, a, b, depth+1);
                if (cur > max_cost)
                {
                    if (cur > b)
                        return 1000000;
                    max_cost = cur;
                    best_move = cord(x, y);
                    a = max(max_cost, a);
                }
            }
        }
    if (best_move.first == -1)
    {
        if (passed)
            max_cost = winner(field) * ITER;
        else
        {
            f temp = field;
            max_cost = move_white(temp, a, b, depth+1, true);
        }
    }
    if (depth == 0)
    {
        if (best_move.first == -1)
            skip();
        else
            do_move(best_move);
    }
    return max_cost;
}
int move_white(f& field, int a, int b, int depth, bool passed)
{
    cerr << "move_white(a=" << a << ", b=" << b << ", depth=" << depth << endl;
    if (depth == DEPTH)
        return cost(field, false);
    int max_cost = 1000000;
    cord best_move(-1, -1);
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++)
        {
            if (at(field, x, y) == 2 && can_move(field, false, x, y))
            {
                f temp = field;
                make_move(temp, false, x, y);
                int cur = move_black(temp, a, b, depth+1);
                if (cur < max_cost)
                {
                    if (cur < a)
                        return -1000000;
                    max_cost = cur;
                    best_move = cord(x, y);
                    b = min(max_cost, b);
                }
            }
        }
    if (best_move.first == -1)
    {
        if (passed)
            max_cost = winner(field) * ITER;
        else
        {
            f temp = field;
            max_cost = move_black(temp, a, b, depth+1, true);
        }
    }
    if (depth == 0)
    {
        if (best_move.first == -1)
            skip();
        else
            do_move(best_move);
    }
    return max_cost;
}
inline void init()
{
    srand(time(NULL));
}
int main()
{
    init();
    short first;
    cin >> first;
    black = first == 1;
    if (!black)
        read_move();
    while (true)
    {
        if (black)
            move_black(field);
        else
            move_white(field);
        read_move();
    }
    return 0;
}
