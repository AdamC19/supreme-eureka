
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "util.h"



void printMap(std::vector<std::vector<char>>& map) {
    for (auto row = map.begin(); row != map.end(); row++) {
        for (auto c = row->begin(); c != row->end(); c++) {
            std::cout << *c;
        }
        std::cout << std::endl;
    }
}



std::pair<int, int> move(std::vector<std::vector<char>>& map, char dir, std::pair<int, int> coord) {
    int x = coord.first;
    int y = coord.second;
    int init_x = x;
    int init_y = y;

    if (map[y][x] != '@') {
        return coord;
    }

    int step_x = 0;
    int step_y = 0;

    if (dir == '^') {
        step_y = -1;
    } 
    else if (dir == '>') {
        step_x = 1;
    } 
    else if (dir == 'v') {
        step_y = 1;
    } 
    else if (dir == '<') {
        step_x = -1;
    }

    while (map[y][x] != '#' && map[y][x] != '.') {
        x += step_x;
        y += step_y;
    }

    std::pair<int, int> retval(coord);
    if (map[y][x] == '.') {
        while (map[init_y][init_x] == '@') {
            int new_x = x - step_x;
            int new_y = y - step_y;
            map[y][x] = map[new_y][new_x];
            if (map[y][x] == '@') {
                retval.first = x;
                retval.second = y;
            }
            map[new_y][new_x] = '.';
            x = new_x;
            y = new_y;
        }
    }
    return retval;
}


int assessMove(std::vector<std::vector<char>>& map, std::pair<int, int> from, int y_dir) {
    int from_x = from.first;
    int from_y = from.second;
    int to_y = from_y + y_dir;
    int x_coords[2];
    int retval = 0;

    if (map[from.second][from.first] == '@') {
        return assessMove(map, std::make_pair(from_x, to_y), y_dir);
    }
    else if (map[from.second][from.first] == '[') {
        x_coords[0] = from.first;
        x_coords[1] = x_coords[0] + 1;
    }
    else if (map[from.second][from.first] == ']') {
        x_coords[1] = from.first;
        x_coords[0] = x_coords[1] - 1;
    }
    else if (map[from.second][from.first] == '#') {
        return 1;
    }
    else {
        return 0;
    }

    for (int i = 0; i < 2; i++) {
        if (map[to_y][x_coords[i]] == '[' || map[to_y][x_coords[i]] == ']') {
            retval += assessMove(map, std::make_pair(x_coords[i], to_y), y_dir);
        } else if (map[to_y][x_coords[i]] == '#') {
            retval++;
        }
        // else, do nothing, leave at 0 contribution
    }

    return retval;
}


void pushUpDown(std::vector<std::vector<char>>& map, char dir, std::pair<int, int> coord) {
    int x = coord.first;
    int y = coord.second;
    int to_y = y + dir;
    int x_coords[2];

    if (map[y][x] == '@') {
        pushUpDown(map, dir, std::make_pair(x, to_y));
        map[to_y][x] = '@';
        map[y][x] = '.';
        return;
    }

    if (map[y][x] == '[') {
        x_coords[0] = x;
        x_coords[1] = x + 1;
    }
    else if (map[y][x] == ']')  {
        x_coords[1] = x;
        x_coords[0] = x - 1;
    }
    else {
        return;
    }
    
    for (int i = 0; i < 2; i++) {
        if (map[to_y][x_coords[i]] == '.') {
            map[to_y][x_coords[i]] = map[y][x_coords[i]];
            map[y][x_coords[i]] = '.';
        } 
        else {
            pushUpDown(map, dir, std::make_pair(x_coords[i], to_y));
        }
    }

}


std::pair<int, int> move2(std::vector<std::vector<char>>& map, char dir, std::pair<int, int> coord) {
    int x = coord.first;
    int y = coord.second;

    if (map[y][x] != '@') {
        return coord;
    }

    int step_y = 0;

    if (dir == '^') {
        step_y = -1;
    } 
    else if (dir == 'v') {
        step_y = 1;
    }
    else {
        return coord;
    }
    if(assessMove(map, std::make_pair(x, y), step_y) == 0) {
        //std::cout << "  Assess move in " << step_y << " y-direction SUCCEEDED!\r\n";
        pushUpDown(map, step_y, std::make_pair(x, y));
        return std::make_pair(x, y + step_y);
    } else {
        //std::cout << "  Assess move in " << step_y << " y-direction failed\r\n";
    }
    return coord;
}

///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "==========================\r\n";
    std::cout << "========= DAY 15 =========\r\n";
    std::cout << "==========================\r\n";


    std::ifstream fin;

    if (argc < 2) {
        std::cout << "Need to supply a file path. Exiting...\r\n";
        return -1;
    }

    if (!file_exists(argv[1])) {
        std::cout << "File " << argv[1] << " does not exist. Exiting...\r\n";
        return -2;
    }

    std::cout << "Reading-in data...\r\n";
    fin.open(argv[1], std::ifstream::in);

    std::vector<std::vector<char>> map_og;
    std::vector<char> seq;

    std::vector<std::string> lines;
    parseInput(lines, fin);

    fin.close();
    
    auto line = lines.begin();
    int map_y = 0;
    std::pair<int, int> coord;
    for (; line != lines.end(); ++line) {
        if ((*line).length() < 1) {
            break;
        }
        std::vector<char> row;
        int map_x = 0;
        //for (auto c = (*line).begin(); c != (*line).end(); ++c) {
        for (int i = 0; i < line->length(); i++) {
            char c = line->at(i);
            row.push_back(c);
            if (c == '@') {
                std::cout << "Found robot at " << map_x << ", " << map_y << "\r\n";
                coord = std::make_pair(map_x, map_y);
            }
            map_x++;
        }
        map_og.push_back(row);
        map_y++;
    }
    std::vector<std::vector<char>> map(map_og); // store original map

    for (; line != lines.end(); ++line) {
        for (int i = 0; i < line->length(); i++) {
            char c = line->at(i);
            seq.push_back(c);
            std::cout << c;
        }
    }
    std::cout << "\r\n";

    /* process sequence */
    std::cout << "Processing sequence..." << std::endl;
    for (auto c = seq.begin(); c != seq.end(); c++) {
        coord = move(map, *c, coord);
    }
    printMap(map);

    int part1 = 0;
    for (int y = 1; y < map.size(); y++) {
        for (int x = 1; x < map[y].size(); x++) {
            if (map[y][x] == 'O') {
                part1 += 100*y + x;
            }
        }
    }
    std::cout << "PART 1 ANSWER: " << part1 << std::endl;


    /* ===== PART 2 ===== */
    std::cout << "\r\n\r\n ======= PART 2 ======== \r\n\r\n";
    std::vector<std::vector<char>> map2;
    std::pair<int, int> coord2;
    int y = 0;
    int x = 0;
    for (auto row = map_og.begin(); row != map_og.end(); row++) {
        std::vector<char> row2;
        x = 0;
        for (auto c = row->begin(); c != row->end(); c++) {
            if (*c == '#') {
                row2.push_back('#');
                row2.push_back('#');
            } 
            else if (*c == '.') {
                row2.push_back('.');
                row2.push_back('.');
            }
            else if (*c == 'O') {
                row2.push_back('[');
                row2.push_back(']');
            }
            else if (*c == '@') {
                row2.push_back('@');
                row2.push_back('.');
                coord2.first = x;
                coord2.second = y;
            }
            x += 2;
        }
        map2.push_back(row2);
        y++;
    }
    printMap(map2);

    for (auto c = seq.begin(); c != seq.end(); c++) {
        char dir = *c;
        std::cout << "Moving " << dir << std::endl;
        if (dir == '<' || dir == '>') {
            coord2 = move(map2, dir, coord2);
        } else {
            coord2 = move2(map2, dir, coord2);
        }
        printMap(map2);
    }

    printMap(map2);

    
    int part2 = 0;
    for (int y = 1; y < map2.size(); y++) {
        for (int x = 1; x < map2[y].size(); x++) {
            if (map2[y][x] == '[') {
                part2 += 100*y + x;
            }
        }
    }
    std::cout << "PART 2 ANSWER: " << part2 << std::endl;

    return 0;

}