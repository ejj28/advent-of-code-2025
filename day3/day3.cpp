#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

std::ifstream input;

struct bankCell {
    int number;
    int position;
};

// Finds the first instance of the highest number, beginning at startIndex and stopping before endIndex
bankCell findHighest(std::string bank, int startIndex, int endIndex) {

    char highest = '0';
    int position = 0;

    for (int i = startIndex; i < endIndex; i++) {
        if (bank[i] > highest) {
            highest = bank[i];
            position = i;
        }
    }

    return {highest - '0', position};
}

// Gets the highest joltage of a bank possible with the given the number of cells
long long bankJoltage(std::string bank, int numCells) {

    int pos = 0;
    long long result = 0;

    for (int i = numCells - 1; i >= 0; i--) {

        bankCell num = findHighest(bank, pos, bank.length() - i);

        pos = num.position + 1;
        result += (num.number * std::pow(10, i));

    }

    return result;
}

int main() {

    long long totalJoltage_pt1 = 0;
    long long totalJoltage_pt2 = 0;
    
    std::string bankLine;

    input.open("day3_input.txt");
    if (input.is_open()) {

        while (getline(input, bankLine)) {
            totalJoltage_pt1 += bankJoltage(bankLine, 2);
            totalJoltage_pt2 += bankJoltage(bankLine, 12);
        }

    }

    std::cout << "Total Joltage (Pt.1): " << totalJoltage_pt1 << '\n';
    std::cout << "Total Joltage (Pt.2): " << totalJoltage_pt2 << '\n';

    return 0;
}
