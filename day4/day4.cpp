#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::ifstream input;

std::vector<std::vector<char>> grid;

int totalRolls = 0;

bool checkPos(int row, int col) {
    if (row < 0 || row >= grid.size() || col < 0 || col >= grid[row].size()) {
        // Check if out of grid bounds
        return false;
    }
    return (grid[row][col] == '@');
}

bool checkSurrounding(int row, int col) {

    // Check 3x3 grid with row,col as the center
    int count = 0;
    for (int r = row - 1; r <= row + 1; r++) {
        for (int c = col - 1; c <= col + 1; c++) {
            count += checkPos(r,c);
        }
    }

    // count will include the @ in row,col
    if (count <= 4) {
        grid[row][col] = '.';
        return true;
    } else {
        return false;
    }

}

void runPass() {

    for (int row = 0; row < grid.size(); row++) {
        for (int col = 0; col < grid[row].size(); col++) {

            if (checkPos(row, col)){
                totalRolls += checkSurrounding(row, col);
            }

        }
    }
    
}

int main() {
    
    std::string line;

    input.open("day4_input.txt");
    if (input.is_open()) {
        
        while (getline(input, line)) {
            std::vector<char> row;
            for (char &c : line) {
                if (c != '\n') {
                    row.push_back(c);
                }
            }
            grid.push_back(row);
        }

        // Run a single pass for part 1
        runPass();
        std::cout << "Part 1 / Total accessible rolls: " << totalRolls << '\n';

        int lastRolls = totalRolls;

        // For part 2, keep running passes until the total stops increasing
        while (true) {
            lastRolls = totalRolls;
            runPass();
            if (totalRolls == lastRolls) {
                break;
            }
        }

        std::cout << "Part 2 / Total accessible rolls: " << totalRolls << '\n';

    }

    return 0;
}