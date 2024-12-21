
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


static const int NORTH = '^';
static const int EAST = '>';
static const int SOUTH = 'v';
static const int WEST = '<';
static const int TURN_CW = 1;
static const int TURN_ANTI_CW = -1;

class Step {
private:
    int m_type;
    Step* m_north;
    Step* m_east;
    Step* m_south;
    Step* m_west;
    int m_x;
    int m_y;
public:
    Step(int type, int x, int y) {
        m_type = type;
        m_x = x;
        m_y = y;
    }
    int type() {
        return m_type;
    }
    int setStep(Step* step, char dir) {
        switch (dir) {
            case '^': 
                m_north = step;
                break;
            case '>': 
                m_east = step;
                break;
            case 'v':
                m_south = step;
                break;
            case '<':
                m_west = step;
                break;
        };
    }

};

void printMap(std::vector<std::vector<char>>& map) {
    for (auto row = map.begin(); row != map.end(); row++) {
        for (auto c = row->begin(); c != row->end(); c++) {
            std::cout << *c;
        }
        std::cout << std::endl;
    }
}

int buildPath(std::vector<std::vector<char>>& map, Step* steps, int x, int y) {
    if (map[y - 1][x] == '.') {
        Step* step = new Step('^', x, y - 1);
        steps->setStep(step, )
    }
}


///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "==========================\r\n";
    std::cout << "========= DAY 16 =========\r\n";
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
    
    
    int map_y = 0;
    std::pair<int, int> start;
    std::pair<int, int> end;

    for (auto line = lines.begin(); line != lines.end(); ++line) {
        std::vector<char> row;
        int map_x = 0;
        //for (auto c = (*line).begin(); c != (*line).end(); ++c) {
        for (int i = 0; i < line->length(); i++) {
            char c = line->at(i);
            row.push_back(c);
            if (c == 'S') {
                std::cout << "Found start at " << map_x << ", " << map_y << "\r\n";
                start.first = map_x;
                start.second = map_y;
            } else if (c == 'E') {
                std::cout << "Found END at " << map_x << ", " << map_y << "\r\n";
                end.first = map_x;
                end.second = map_y;
            }
            map_x++;
        }
        map_og.push_back(row);
        map_y++;
    }
    std::vector<std::vector<char>> map(map_og); // store original map

    std::vector<std::vector<char>> paths;


    std::cout << "\r\n";

    int part1 = 0;

    std::cout << "PART 1 ANSWER: " << part1 << std::endl;

    return 0;

}