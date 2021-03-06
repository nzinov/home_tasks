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
ofstream LOG;

const time_t TICKS = CLOCKS_PER_SEC * 14 / 5;
const time_t MIN_TICKS = CLOCKS_PER_SEC * 5 / 4;
const time_t MAX_TICKS = CLOCKS_PER_SEC / 2;
const time_t MAX_TICKS_PASSES = CLOCKS_PER_SEC;
int PASSES = 30;

const int MATRIX[8][8] = {
   { 99, -8, 8, 6, 6, 8, -8,  99,},
   {-8, -24,  -4, -3, -3, -4, -24, -8,},
   { 8,  -4,  7, 4, 4,  7,   -4,  8,},
   {  6,   -3,  4, 0, 0,  4,   -3,   6,},
   {  6,   -3,  4, 0, 0,  4,   -3,   6,},
   { 8,   -4,  7, 4, 4,  7,   -4,  8,},
   {-8, -24,  -4, -3, -3,  -4, -24, -8,},
   { 99, -8, 8, 6, 6, 8, -8,  99,}
};

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
        memset(this, 0, sizeof(Field));
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
        LOG << endl;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                short cur = field[j][i];
                LOG << (cur == NONE ? '.' : (cur == BLACK ? 'X' : 'O'));
            }
            LOG << endl;
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
        int moves = 0;
        int delta = 0;
        int sum_pos = 0;
        int delta_pos = 0;
        int delta_mob = 0;
        int mob = 0;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (field[i][j] == NONE && can_move(i, j, BLACK)) {
                    mob++;
                    delta_mob++;
                }
                if (field[i][j] == NONE && can_move(i, j, WHITE)) {
                    mob++;
                    delta_mob--;
                }
                if (field[i][j] != NONE) {
                    delta_pos += coef(field[i][j])*MATRIX[i][j];
                    sum_pos += MATRIX[i][j];
                    moves++;
                    delta += coef(field[i][j]);
                }
            }
        }
        if (sum_pos > 0) {
            ans += delta_pos * 200000 / sum_pos;
        }
        if (mob > 0) {
            ans += delta_mob * 50000 / mob;
        }
        if (moves < 30) {
            ans -= delta*30000/moves;
        }
        if (moves > 40) {
            ans += delta*30000/moves;
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

    int best_score(Field cur, int required_depth, int my = -1000000000, int opponent = 1000000000, bool make_move = false) {
        if (start != 0 && clock() > start) {
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
            //LOG << '{' << endl;
            for (int i = 0; i < n; ++i) {
                if (i % 3 == 2 && new_depth > 1) {
                    //new_depth--;
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
            //LOG << '}' << endl;
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
        //LOG << required_depth << ':' << score << endl;
        //cur.print();
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
        //LOG << "------------------";
        best_score(position, 2, -100000, 100000, true);
        //LOG << "------------------";
        best_score(position, save_depth, -100000, 100000, true);
        do_move();
        position.print();
        time_t left = start > clock() ? start - clock() : 0;
        if ((collapse || left <= MAX_TICKS) && save_depth > 3) {
            save_depth--;
        } else if (left > MIN_TICKS) {
            save_depth++;
        }
        //LOG << collapse << ' ' << save_depth << ' ' << PASSES << endl;
        collapse = false;
        cerr << save_depth << endl;
    }

    void serialize() {
        ofstream o;
        o.open("debut.txt");
        for (auto el : cache) {
            char* a = (char*)&(el.first);
            char* b = (char*)&(el.second);
            o.write(a, sizeof(Field));
            o.write(b, sizeof(Data));
        }
        o.close();
    }

    void load() {
        ifstream o;
        o.open("debut.txt");
        while (!o.eof()) {
            Field f;
            char* a = (char*)(&f);
            o.read(a, sizeof(Field));
            char* b = (char*)(&cache[f]);
            o.read(b, sizeof(Data));
        }
        o.close();
    }
};

int main()
{
    LOG.open("log.txt");
    srand(time(NULL));
    std::string input;
    Gamer gamer;
    gamer.start = 0;
    gamer.my_color = BLACK;
    //gamer.best_score(gamer.position, 50);
    //gamer.serialize();
    //exit(0);
    //gamer.load();
    getline(cin, input);
    short color = input[5] == 'w';
    gamer.my_color = color;
    gamer.collapse = false;
    gamer.save_depth = 4;
    while (true)
    {
        gamer.process_move();
    }
    return 0;
}
