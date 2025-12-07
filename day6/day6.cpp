#include <iostream>
#include <fstream>
#include <string>
#include <vector>

enum Operation {
    Addition,
    Multiplication
};

enum Part {
    Part1,
    Part2
};

struct expression {

    // Container for each individual problem

    // Holds numbers interpreted both ways
    std::vector<std::string> pt1_numbers;
    std::vector<std::string> pt2_numbers;

    Operation operation;

    // Calculate total for either interpretation of numbers
    long long total(Part part) {

        std::vector<std::string> * numbers;

        part ? numbers = &pt2_numbers : numbers = &pt1_numbers;

        long long result = 0;
        bool first = true;

        for (std::string &n : *numbers) {

            if (first && operation == Multiplication) {
                result = stoi(n);
                first = false;
            } else {
                operation ? result *= stoi(n) : result += stoi(n);
            }
            
        }
        
        return result;
    }

};



int main() {
    
    std::ifstream input;

    input.open("day6_input.txt");
    if (input.is_open()) {

        std::vector<std::string> inputGrid;

        std::string line;

        while (getline(input, line)) {
            inputGrid.push_back(line);
        }

        std::vector<expression> problems;

        
        int id = 0;
        bool first = true;

        char cbuf;
        std::string pt2_buffer;

        // Step backwards through each column
        for (int col = inputGrid[0].length() - 1; col >= 0; col--) {

            if (first) {

                problems.push_back({}); // Create an expression instance

                // Prepopulate number entries for each line for pt.1
                for (int r = 0; r < inputGrid.size() - 1; r++) {
                    problems[id].pt1_numbers.push_back("");
                }

                first = false;

            }

            // Step through the rows
            for (int row = 0; row < inputGrid.size(); row++) {

                cbuf = inputGrid[row][col];

                if (cbuf != ' ') {

                    if (cbuf == '+') {
                        problems[id].operation = Addition;
                    } else if (cbuf == '*') {
                        problems[id].operation = Multiplication;
                    } else {

                        problems[id].pt1_numbers[row] = cbuf + problems[id].pt1_numbers[row]; // (Pt.1) Add digit to number for this row
                        pt2_buffer += cbuf; // (Pt.2) Add digit to number for this column

                    }

                }

            }

            // Checks if column is blank
            if (pt2_buffer.length() > 0) {

                // Push total column number for pt.2
                problems[id].pt2_numbers.push_back(pt2_buffer);
                pt2_buffer = "";

            } else {

                // Advance to next problem
                id++;
                first = true;

            }

        }

        long long pt1_total = 0;
        long long pt2_total = 0;

        for (expression &exp : problems) {
            pt1_total += exp.total(Part1);
            pt2_total += exp.total(Part2);
        }

        std::cout << "Part 1 / Total: " << pt1_total << '\n';
        std::cout << "Part 2 / Total: " << pt2_total << '\n';

    }
    return 0;
}
