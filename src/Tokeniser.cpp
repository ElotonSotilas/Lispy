//
// Created by George Vassilev on 14.04.23.
//

#include "lispy.h"
#include <unordered_set>

// Tokenising the input
std::vector<std::string> Tokeniser::Tokenise(IO &input) {
    std::string data = input.Read();
    std::vector<std::string> tokens;

    // Token
    std::string tok;

    // Ignored characters
    std::unordered_set<char> delimiters {' ', '\t', '\r', '\n'};

    for (char c : data) {
        if (delimiters.contains(c)) {
            if (!tok.empty()) {
                tokens.push_back(tok);
                tok.clear();
            }
        }

        else if (c == '(' || c == ')') {
            if (!tok.empty()) {
                tokens.push_back(tok);
                tok.clear();
            }
            tokens.push_back(std::string{c});
        }

        else {
            tok += c;
        }
    }
    if (!tok.empty()) {
        tokens.push_back(tok);
    }

    // DEBUG

//    std::copy(tokens.begin(), tokens.end(),
//              std::ostream_iterator<std::string>(std::cout, "\n"));

    return tokens;
}

