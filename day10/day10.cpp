#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <deque>

struct Button {
    std::vector<int> lightIndices;
};

enum Light {
    Off,
    On
};

struct Machine {
    std::vector<Button> buttons;
    std::vector<Light> lightTarget;
    std::vector<int> joltageTarget;
};

struct TEB_Params {
    int depth;
    std::vector<Light> state;
    int parentIndex;
};

struct TEB_Result {
    bool success = false;
    int successDepth;
    std::vector<TEB_Params> params;
};

TEB_Result tryEveryButton(int currentDepth, int parentButtonIndex, std::vector<Button> &buttons, std::vector<Light> &currentState, std::vector<Light> &targetState) {
    // Checks the outcome of every button press (Except for the last pressed button)

    int depth = currentDepth + 1;

    std::vector<Light> state;

    TEB_Result res;

    for (int b = 0; b < buttons.size(); b++) {
            
        if (b != parentButtonIndex) {

            state = currentState;

            // Toggle each light specified by the button
            for (int &l : buttons[b].lightIndices) {
                state[l] = state[l] ? Off : On;
            }

            // Report a match and the depth (number of presses) it was at
            if (state == targetState) {
                res.success = true;
                res.successDepth = depth;
                break;

            } else {
                // Push all the buttons again 
                res.params.push_back({depth, state, b});
            }

        }
    }

    return res;
}



enum ParseStage {
    Lights,
    Buttons,
    Joltage
};

int main() {

    std::ifstream input;

    std::vector<Machine> machines;


    input.open("day10_input.txt");
    if (input.is_open()) {

        std::string line;

        while (getline(input, line)) {
            
            Machine m;
            Button B;

            ParseStage stage = Lights;

            std::string numBuffer = "";

            // Parse input line
            for (char &c : line) {
                switch (stage) {
                    case Lights:

                        if (c == ']') stage = Buttons;

                        else if (c != '[') m.lightTarget.push_back((c == '#') ? Light::On : Light::Off);

                        break;

                    case Buttons:

                        if (c == ',' || c == ')') {
                            B.lightIndices.push_back(stoi(numBuffer));
                            numBuffer = "";
                        }
                        
                        else if (c == '{') stage = Joltage;

                        else if (c != '(' && c != ' ') numBuffer += c;

                        if (c == ')') {
                            m.buttons.push_back(B);
                            B = Button{};
                        }

                        break;

                    case Joltage:

                        if (c == ',' || c == '}') {
                            m.joltageTarget.push_back(stoi(numBuffer));
                            numBuffer = "";

                        } else numBuffer += c;

                        break;

                    default:
                        break;
                }
            }

            machines.push_back(m);
        }

    } else {
        std::cout << "Unable to open input\n";
        return -1;
    }


    // Breadth-first search to find the fewest required presses for each machine

    int totalPresses = 0;

    for (Machine &m : machines) {

        // Starting state (all lights off)
        std::vector<Light> state (m.lightTarget.size(), Off);
        
        std::queue<TEB_Params> params;

        // Initial push of all buttons
        params.push({0, state, -1});

        while (true) {

            // Check all button presses starting from the first state in the queue, add all the possible resulting states to the end of the queue

            TEB_Result r = tryEveryButton(params.front().depth, params.front().parentIndex, m.buttons, params.front().state, m.lightTarget);

            if (r.success) {
                totalPresses += r.successDepth;
                break;
            }

            for (TEB_Params p : r.params) {
                params.push(p);
            }

            params.pop();
            
        }

    }

    std::cout << "Part 1 / Total presses: " << totalPresses << '\n';

    return 0;
}
