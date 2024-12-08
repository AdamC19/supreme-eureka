
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

class Line {
    public:
    std::pair<int, int> a;
    std::pair<int, int> b;

    Line(std::pair<int, int> ptA, std::pair<int, int> ptB) {
        a = ptA;
        b = ptB;
    }
    Line(const Line& obj) {
        a = obj.a;
        b = obj.b;
    }

    friend bool operator==(const Line& line1, const Line& line2) {
        if (line1.a == line2.a && line1.b == line2.b) {
            return true;
        }
        if (line1.a == line2.b && line1.b == line2.a) {
            return true;
        }
        return false;
    }
    friend bool operator!=(const Line& line1, const Line& line2) { return !(line1 == line2); }
};


class Frequency {
    public:

    // coordinates of the antennas
    std::vector<std::pair<int, int>> coords; 

    // coordinates of the antinodes for this antenna. Populated by findAntinodes
    std::vector<std::pair<int, int>> antinodes; 

    char id;

    Frequency() { id = '\0'; }
    Frequency(char c) { id = c; }
    Frequency(const Frequency& obj) {
        id = obj.id;
        coords = obj.coords;
    }

    /**
     * Add the coordinate (x, y) to the list of coordinates if it does not already exist
     */
    void addCoord(int x, int y) {
        auto new_coord = std::make_pair(x, y);
        std::vector<std::pair<int, int>>::iterator found = std::find(coords.begin(), coords.end(), new_coord);
        if (found == coords.end()) {
            coords.push_back(new_coord);
        }
    }

    /**
     * Find antinodes
     */
    std::vector<std::pair<int, int>> findAntinodes() {

        std::cout << "Freq " << id << ": Finding antinodes...\r\n";
        // first, create the lines between all antennas
        std::vector<Line> lines;
        
        for (auto pt_it = coords.begin(); pt_it != coords.end(); pt_it++) {
            std::pair<int, int> anchor = *pt_it; // copy our first point
            // iterate over all other points, drawing lines between pt and each other point
            for (auto pt = coords.begin(); pt != coords.end(); pt++) {
                if (*pt != anchor) {
                    // the points are not the same, so see if we need to add a new line
                    Line line(anchor, *pt);
                    auto found = std::find(lines.begin(), lines.end(), line);
                    if (found == lines.end()) {
                        lines.push_back(line);
                    }
                }
            }
        }

        std::cout << "  Drew " << lines.size() << " lines from " << coords.size() << " points.\r\n";

        for (auto line = lines.begin(); line != lines.end(); ++line) {
            int delta_x = line->b.first - line->a.first;
            int delta_y = line->b.second - line->a.second;
            // one antinode "before" point A and one "after" point B
            std::pair<int, int> before;
            before.first = line->a.first - delta_x;
            before.second = line->a.second - delta_y;
            auto found = std::find(antinodes.begin(), antinodes.end(), before);
            if (found == antinodes.end())
                antinodes.push_back(before);

            std::pair<int, int> after;
            after.first = line->b.first + delta_x;
            after.second = line->b.second + delta_y;
            found = std::find(antinodes.begin(), antinodes.end(), after);
            if (found == antinodes.end())
                antinodes.push_back(after);
        }
        std::cout << "  Found " << antinodes.size() << " antinodes\r\n";
        return antinodes;
    }

};


void plotAntinodes(std::vector<std::vector<char>>& map, std::vector<std::pair<int, int>> antinodes) {
    int min_x = 0;
    int min_y = 0;
    int max_x = map[0].size() - 1;
    int max_y = map.size() - 1;

    for (auto pt = antinodes.begin(); pt != antinodes.end(); ++pt) {
        if (pt->first >= min_x && pt->first <= max_x && pt->second >= min_y && pt->second <= max_y) {
            map[pt->second][pt->first] = '#';
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "************************\r\n";
    std::cout << "******** DAY 8 *********\r\n";
    std::cout << "************************\r\n";


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
    
    std::vector<std::vector<char>> og_map;
    std::vector<std::vector<char>> anode_map;
    std::vector<char> freq_list;

    std::vector<char> row_tmp;
    std::vector<char> anode_row_tmp;
    char c;
    while (fin.get(c)) {
        if (c != '\n') {
            row_tmp.push_back(c);
            anode_row_tmp.push_back('.');
            if (c != '.') {
                std::vector<char>::iterator found = std::find(freq_list.begin(), freq_list.end(), c);
                if (found == freq_list.end()) {
                    freq_list.push_back(c);
                }
            }
        } else {
            og_map.push_back(row_tmp);
            anode_map.push_back(anode_row_tmp);
            row_tmp.clear();
            anode_row_tmp.clear();
        }
    }
    og_map.push_back(row_tmp);
    anode_map.push_back(anode_row_tmp);

    fin.close();

    std::cout << "Found " << freq_list.size() << " different frequencies.\r\n";

    // PART 1
    std::map<char, Frequency> freqs;
    for (int i_row = 0; i_row < og_map.size(); i_row++) {
        for (int i_col = 0; i_col < og_map[i_row].size(); i_col++) {
            char c = og_map[i_row][i_col];
            if (c != '.') {
                auto found = freqs.find(c);
                if (found == freqs.end()) {
                    // create new Frequency object, then add to map
                    Frequency f(c);
                    f.addCoord(i_col, i_row);
                    freqs[c] = f;
                } else {
                    // add coord to existing freq object
                    found->second.addCoord(i_col, i_row);
                }
            }
            
        }
    }

    for (auto freq_it = freqs.begin(); freq_it != freqs.end(); freq_it++) {
        freq_it->second.findAntinodes();
        plotAntinodes(anode_map, freq_it->second.antinodes);
    }

    
    int part1 = 0;

    std::cout << "**************** PART 1 ****************\r\n";
    for (int i_row = 0; i_row < anode_map.size(); i_row++) {
        for (int i_col = 0; i_col < anode_map[i_row].size(); i_col++) {
            char c = anode_map[i_row][i_col];
            if (c == '#') {
                part1++;
            }
            std::cout << anode_map[i_row][i_col];
        }
        std::cout << "\r\n";
    }
    
    std::cout << "  COUNT OF ANTINODES: " << part1 << "\r\n";
    std::cout << "****************************************\r\n";


    return 0;
}