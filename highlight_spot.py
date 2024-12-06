

import sys

with open(sys.argv[1], 'r') as fin:
    x = int(sys.argv[2])
    y = int(sys.argv[3])

    print(f"Highlighting position {x},{y}...")

    lines = []
    for line in fin:
        lines.append(line.strip())


    for row_ind, row in enumerate(lines):
        for col_ind, c in enumerate(list(row)):
            if col_ind == x and row_ind == y:
                print(f"\033[31;1;4mO\033[0m", end='')
            else:
                print(c, end='')
        print()