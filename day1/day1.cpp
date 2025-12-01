#include <iostream>
#include <fstream>
#include <string>

int main() {
    
    std::ifstream input;

    int onZeroCount = 0;
    int passedZeroCount = 0;

    int dial = 50;
    int lastDial;

    int clicks;
    int change;

    std::string line;

    input.open("day1_input.txt");
    if (input.is_open()) {
        
        while (getline(input, line)) {

            clicks = stoi(line.substr(1));
            
            if (dial == 0 && clicks >= 100 && clicks % 100 == 0) {
                // One or more exact full rotations starting from zero (and therefore ending on zero)
                // This actually will never happen with the official test data, it's just an edge case I thought of
                // Count one zero-landing, any additional rotations count as passes
                onZeroCount += 1;
                passedZeroCount += (clicks / 100) - 1;
            } else {
                // Otherwise, treat every full rotation as a zero pass
                passedZeroCount += clicks / 100;
            }
            
            // The number of clicks minus full rotations
            change = clicks % 100;

            lastDial = dial;

            if (line[0] == 'R') {

                dial += change;
                if (dial > 99) {
                    // We know zero has either been landed on or passed
                    dial -= 100;
                    if (dial == 0) {
                        onZeroCount++;
                    } else {
                        passedZeroCount++;
                    }
                }

            } else {
                
                dial -= change;
                if (dial < 0) {
                    // We know zero has been passed, but it could have started at zero
                    dial += 100;
                    if (lastDial != 0) {
                        // Dial did not start at zero, count as pass
                        passedZeroCount += 1;
                    }
                } else if (dial == 0 && lastDial != 0) {
                    // Dial is at zero and did not start at zero (see comment further up about that edge case)
                    onZeroCount += 1;
                }

            }
            
        }

    }
    
    printf("On-zero count: %d\n", onZeroCount);
    printf("Total zero count: %d\n", onZeroCount + passedZeroCount);

    return 0;
}