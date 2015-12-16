#!/bin/python
import sys
from  subprocess import Popen, PIPE
BLACK = 0
WHITE = 1
NONE = 2
field = [[NONE for i in range(8)] for j in range(8)]
field[3][3] = WHITE;
field[4][4] = WHITE;
field[3][4] = BLACK;
field[4][3] = BLACK;
color = BLACK
passed = False
with Popen([sys.argv[1]], stdin=PIPE, stdout=PIPE, universal_newlines=True) as a:
    with Popen([sys.argv[2]], stdin=PIPE, stdout=PIPE, universal_newlines=True) as b:
        clients = [a, b]
        a.stdin.write("init black\n")
        a.stdin.flush()
        b.stdin.write("init white\n")
        b.stdin.flush()
        while True:
            has_move = False
            for i in range(8):
                for j in range(8):
                    data = ["X", "O", "."]
                    print(data[field[j][i]], end=" ")
                print()
            print()
            for i in range(8):
                for j in range(8):
                    if field[i][j] != NONE:
                        continue
                    for dx in range(-1, 2):
                        for dy in range(-1, 2):
                            if dx == 0 and dy == 0:
                                continue
                            x, y = i, j
                            x += dx
                            y += dy
                            rev = False
                            while 0 <= x < 8 and 0 <= y < 8 and field[x][y] == 1 - color:
                                rev = True
                                x += dx
                                y += dy
                            if rev and 0 <= x < 8 and 0 <= y < 8 and field[x][y] == color:
                                has_move = True
            if not has_move:
                if passed:
                    score = 0
                    for i in range(8):
                        for j in range(8):
                            if field[i][j] == BLACK:
                                score += 1
                            if field[i][j] == WHITE:
                                score -= 1
                    print(score)
                    break
                color = 1 - color
                passed = True
                continue
            clients[color].stdin.write("turn\n")
            clients[color].stdin.flush()
            turn = clients[color].stdout.readline()
            print(color, turn)
            clients[1 - color].stdin.write(turn)
            clients[1 - color].stdin.flush()
            move_x = ord(turn[5]) - ord('a');
            move_y = ord(turn[7]) - ord('1');
            for dx in range(-1, 2):
                for dy in range(-1, 2):
                    if dx == 0 and dy == 0:
                        continue
                    x, y = move_x, move_y
                    x += dx
                    y += dy
                    rev = False
                    while 0 <= x < 8 and 0 <= y < 8 and field[x][y] == 1 - color:
                        rev = True
                        x += dx
                        y += dy
                    if rev and 0 <= x < 8 and 0 <= y < 8 and field[x][y] == color:
                        x -= dx
                        y -= dy
                        while x != move_x or y != move_y:
                            field[x][y] = color
                            x -= dx
                            y -= dy
                        field[x][y] = color
            color = 1 - color
            passed = False





