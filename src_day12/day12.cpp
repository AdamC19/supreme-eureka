
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


enum class Direction:int {
    DIR_NORTH = 0,
    DIR_EAST = 1,
    DIR_SOUTH = 2,
    DIR_WEST = 3,
    DIR_NONE = 4
};


class Cell {
private:
    char m_type;
    bool m_fences[5];
    int m_region_id;
public:
    Cell(char type) {
        m_type = type;
        m_region_id = -1;
        memset(m_fences, false, 4);
    }
    Cell(const Cell& obj) {
        m_type = obj.m_type;
        m_region_id = obj.m_region_id;
        memcpy(m_fences, obj.m_fences, 4);
    }

    char getType() {
        return m_type;
    }
    void setFence(Direction dir) {
        m_fences[(int)dir] = true;
    }
    bool isFenceSet(Direction dir) {
        return m_fences[(int)dir];
    }
    void setRegionId(int id) {
        m_region_id = id;
    }
    int regionId() {
        return m_region_id;
    }
    int edgeCount(){
        int retval = 0;
        for (int i = 0; i < 4; i++) {
            if (m_fences[i])
                retval++;
        }
        return retval;
    }
};


std::pair<int, int> formRegion(std::vector<std::vector<Cell>>& plots, int x, int y, int id) {
    plots[y][x].setRegionId(id);

    std::pair<int, int> retval = std::make_pair(1, 0);
    // check north
    if (!plots[y][x].isFenceSet(Direction::DIR_NORTH)) {
        if (plots[y - 1][x].regionId() == -1) {
            std::pair<int, int> val = formRegion(plots, x, y - 1, id);
            retval.first += val.first;
            retval.second += val.second;
        }
    } else {
        retval.second++;
    }
    // check EAST
    if (!plots[y][x].isFenceSet(Direction::DIR_EAST)) {
        if (plots[y][x + 1].regionId() == -1){
            std::pair<int, int> val = formRegion(plots, x + 1, y, id);
            retval.first += val.first;
            retval.second += val.second;
        }
    } else {
        retval.second++;
    }
    // check SOUTH
    if (!plots[y][x].isFenceSet(Direction::DIR_SOUTH)) {
        if (plots[y + 1][x].regionId() == -1) {
            std::pair<int, int> val = formRegion(plots, x, y + 1, id);
            retval.first += val.first;
            retval.second += val.second;
        }
    } else {
        retval.second++;
    }
    // check WEST
    if (!plots[y][x].isFenceSet(Direction::DIR_WEST)) {
        if (plots[y][x - 1].regionId() == -1) {
            std::pair<int, int> val = formRegion(plots, x - 1, y, id);
            retval.first += val.first;
            retval.second += val.second;
        }
    } else {
        retval.second++;
    }
    return retval;
}


void countRegionSides(std::vector<std::vector<Cell>>& plots, Direction start_edge, int end_x, int end_y, int x, int y, Direction last_edge, int* total_sides) {
    
    if (last_edge == Direction::DIR_NONE) {
        
        if (plots[y][x].isFenceSet(Direction::DIR_WEST)) {
            //(*total_sides)++;
            if (!plots[y][x].isFenceSet(Direction::DIR_NORTH)){
                last_edge = Direction::DIR_WEST;
                start_edge = Direction::DIR_WEST;
            } 
            //countRegionSides(plots, end_x, end_y, x, y - 1, Direction::DIR_WEST, total_sides);
        } 
        if (plots[y][x].isFenceSet(Direction::DIR_SOUTH)) {
            //(*total_sides)++;
            if (!plots[y][x].isFenceSet(Direction::DIR_WEST)){
                last_edge = Direction::DIR_SOUTH;
                start_edge = Direction::DIR_SOUTH;
            } 
            //countRegionSides(plots, end_x, end_y, x - 1, y, Direction::DIR_SOUTH, total_sides);
        }
        if (plots[y][x].isFenceSet(Direction::DIR_EAST)) {
            //(*total_sides)++;
            if(!plots[y][x].isFenceSet(Direction::DIR_SOUTH)) {
                last_edge = Direction::DIR_EAST;
                start_edge = Direction::DIR_EAST;
            } 
            //countRegionSides(plots, end_x, end_y, x, y + 1, Direction::DIR_EAST, total_sides);
        }
        if (plots[y][x].isFenceSet(Direction::DIR_NORTH)) {
            //(*total_sides)++;
            if (!plots[y][x].isFenceSet(Direction::DIR_EAST)) {
                last_edge = Direction::DIR_NORTH;
                start_edge = Direction::DIR_NORTH;
            } 
        } 
        if (last_edge == Direction::DIR_NONE) {
            (*total_sides) = 4; // single cell region
            return;
        }
    } else if (last_edge == start_edge && x == end_x && y == end_y) {
        return;
    }
    //std::cout << "(" << x << ", " << y << "):  Last direction was ";

    if (last_edge == Direction::DIR_NORTH) {
        //std::cout << "NORTH.  ";
        if (!plots[y][x].isFenceSet(Direction::DIR_EAST)) {
            if (plots[y][x + 1].isFenceSet(Direction::DIR_NORTH)) {
                //std::cout << "Continuing to the EAST...\r\n";
                countRegionSides(plots, start_edge, end_x, end_y, x + 1, y, last_edge, total_sides);
            } else {
                (*total_sides)++;
                //std::cout << "Continuing to the NORTH...\r\n";
                countRegionSides(plots, start_edge, end_x, end_y, x + 1, y - 1, Direction::DIR_WEST, total_sides);
            }
        } 
        else {
            (*total_sides)++;
            countRegionSides(plots, start_edge, end_x, end_y, x, y, Direction::DIR_EAST, total_sides);
        }
    }
    else if (last_edge == Direction::DIR_EAST) {
        //std::cout << "EAST.  \r\n";
        if (!plots[y][x].isFenceSet(Direction::DIR_SOUTH)) {
            if (plots[y + 1][x].isFenceSet(Direction::DIR_EAST)) {
                countRegionSides(plots, start_edge, end_x, end_y, x, y + 1, last_edge, total_sides);
            } else {
                (*total_sides)++;
                countRegionSides(plots, start_edge, end_x, end_y, x + 1, y + 1, Direction::DIR_NORTH, total_sides);
            }
        } 
        else {
            (*total_sides)++;
            countRegionSides(plots, start_edge, end_x, end_y, x, y, Direction::DIR_SOUTH, total_sides);
        }
    }
    else if (last_edge == Direction::DIR_SOUTH) {
        //std::cout << "SOUTH.  \r\n";
        if (!plots[y][x].isFenceSet(Direction::DIR_WEST)) {
            if (plots[y][x - 1].isFenceSet(Direction::DIR_SOUTH)) {
                countRegionSides(plots, start_edge, end_x, end_y, x - 1, y, last_edge, total_sides);
            } else {
                (*total_sides)++;
                countRegionSides(plots, start_edge, end_x, end_y, x - 1, y + 1, Direction::DIR_EAST, total_sides);
            }
        } 
        else {
            (*total_sides)++;
            countRegionSides(plots, start_edge, end_x, end_y, x, y, Direction::DIR_WEST, total_sides);
        }
    }
    else if (last_edge == Direction::DIR_WEST) {
        //std::cout << "WEST.  \r\n";
        if (!plots[y][x].isFenceSet(Direction::DIR_NORTH)) {
            if (plots[y - 1][x].isFenceSet(Direction::DIR_WEST)){
                countRegionSides(plots, start_edge, end_x, end_y, x, y - 1, last_edge, total_sides);
            } else {
                (*total_sides)++;
                countRegionSides(plots, start_edge, end_x, end_y, x - 1, y - 1, Direction::DIR_SOUTH, total_sides);
            }
        } 
        else {
            (*total_sides)++;
            countRegionSides(plots, start_edge, end_x, end_y, x, y, Direction::DIR_NORTH, total_sides);
        }
    }
}




///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "==========================\r\n";
    std::cout << "========= DAY 12 =========\r\n";
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

    std::vector<std::vector<Cell>> plots;

    std::vector<Cell> row;

    char c;
    while (fin.get(c)) {
        if (c != '\n') {
            row.push_back(Cell(c));
        } else {
            plots.push_back(row);
            row.clear();
        }
    }
    plots.push_back(row);

    fin.close();


    std::cout << "---------- BEGIN PART 1 -----------\r\n";

    /* ==== Build Fences ==== */
    std::cout << "Building fences...\r\n";
    // plots[0][0].setRegionId(0); // set initial state
    for (int y = 0; y < plots.size(); y++) {
        for (int x = 0; x < plots[y].size(); x++) {
            char type = plots[y][x].getType();
            int region_id = plots[y][x].regionId();
            
            /* check for east and west fence conditions */
            if (x == 0) {
                plots[y][x].setFence(Direction::DIR_WEST);
            }
            else {
                /* look to the WEST to see if we need a fence */
                if (plots[y][x - 1].getType() != type) {
                    plots[y][x].setFence(Direction::DIR_WEST);
                }
            } 
            if (x == plots[y].size() - 1) {
                plots[y][x].setFence(Direction::DIR_EAST);
            }
            else {
                /* look to the EAST to see if we need a fence there */
                if (plots[y][x + 1].getType() != type) {
                    plots[y][x].setFence(Direction::DIR_EAST);

                    // if (plots[y][x + 1].regionId() == -1) {
                    //     /* new region */
                    //     plots[y][x + 1].setRegionId(n_regions);
                    //     n_regions++;
                    // }
                    
                } 
                // else {
                //     // cell to the EAST is same type, so mark region ID accordingly
                //     plots[y][x + 1].setRegionId(region_id);
                // }
            } 

            /* check for north and south fence conditions */
            if (y == 0) {
                plots[y][x].setFence(Direction::DIR_NORTH);
            }
            else {
                /* look NORTH to see if fence is required */
                if (plots[y - 1][x].getType() != type) {
                    plots[y][x].setFence(Direction::DIR_NORTH);
                }
            }
            
            if (y == plots.size() - 1) {
                plots[y][x].setFence(Direction::DIR_SOUTH);
            }
            else {
                /* look SOUTH to see if fence is required */
                if (plots[y + 1][x].getType() != type) {
                    plots[y][x].setFence(Direction::DIR_SOUTH);
                    
                    // if (plots[y + 1][x].regionId() == -1) {
                    //     plots[y + 1][x].setRegionId(n_regions);
                    //     n_regions++;
                    // } 

                } 
                // else {
                //     plots[y + 1][x].setRegionId(region_id);
                // }
            }
        }
    }


    /* ==== ASSIGN REGION IDs ==== */
    std::cout << "Assigning region IDs...\r\n";
    std::map<int, std::pair<int,int>> region_areas;
    std::map<int, int> region_sides;
    int n_regions = 0;
    for (int y = 0; y < plots.size(); y++) {
        for (int x = 0; x < plots[y].size(); x++) {
            if (plots[y][x].regionId() == -1) {
                std::pair<int,int> data = formRegion(plots, x, y, n_regions);
                region_areas[n_regions] = data;
                int n_sides = 0;
                std::cout << "Counting sides for region " << n_regions << "... ";
                countRegionSides(plots, Direction::DIR_NONE, x, y, x, y, Direction::DIR_NONE, &n_sides);
                // if (plots[y][x].isFenceSet(Direction::DIR_NORTH) && !plots[y][x].isFenceSet(Direction::DIR_EAST)) {
                //     
                // } else if (!plots[y][x].isFenceSet(Direction::DIR_SOUTH)) {
                //     countRegionSides(plots, x, y, x, y + 1, Direction::DIR_EAST, &n_sides);
                // } else if (!plots[y][x].isFenceSet(Direction::DIR_WEST)) {
                //     countRegionSides(plots, x, y, x - 1, y, Direction::DIR_SOUTH, &n_sides);
                // } else {
                //     countRegionSides(plots, x, y, x, y - 1, Direction::DIR_WEST, &n_sides);
                // }
                std::cout << n_sides << "\r\n";
                region_sides[n_regions] = n_sides;
                n_regions++;
            }
        }
    }

    int part1_price = 0;
    int part2_price = 0;
    std::cout << "Found " << n_regions << " regions\r\n";
    for (int id = 0; id < n_regions; id++){ 
        std::cout << "Region " << id << ": Area = " << region_areas[id].first;
        //std::cout << ", perimeter = " << region_areas[id].second;
        int price = region_areas[id].first * region_areas[id].second;
        int price_pt2 = region_areas[id].first * region_sides[id];
        std::cout << ", Sides = " << region_sides[id];
        std::cout << ", Price = " << price_pt2 << "\r\n";
        part1_price += price;
        part2_price += price_pt2;
    }
    
    // for (int y = 0; y < plots.size(); y++) {
    //     for (int x = 0; x < plots[y].size(); x++) {
    //         std::cout << plots[y][x].regionId();
    //         // if (plots[y][x].regionId() == id) {
    //         //     std::cout << plots[y][x].getType();
    //         // } else {
    //         //     std::cout << '.';
    //         // }
    //     }
    //     std::cout << "\r\n";
    // }
    std::cout << "PART 1 PRICE = " << part1_price << "\r\n";
    std::cout << "PART 2 PRICE = " << part2_price << "\r\n";
    


    return 0;

}