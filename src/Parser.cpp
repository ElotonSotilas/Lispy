//
// Created by George Vassilev on 14.04.23.
//

#include <iterator>
#include "lispy.h"

std::unique_ptr<AstNode> Parser::Parse(const std::vector<std::string> &tokens) {
    if (tokens.size() == 1 && tokens.front() != "(") {
        return std::make_unique<AtomNode>(tokens.front());
    }
    auto parseHead = tokens.begin();
    ++parseHead; // skip first opening parenthesis
    return ParseList(parseHead, tokens.end());
}

std::unique_ptr<AstNode> Parser::ParseList(std::vector<std::string>::const_iterator& it,
    const std::vector<std::string>::const_iterator end) {
    auto node = std::make_unique<ListNode>();
    while (true) {
        if (it == end) {
            throw std::runtime_error("Missing closing parenthesis");
        }
        else if (*it == ")") {
            break;
        }
        else if (*it == "(") {
            if (node->head.empty()) {
                // this means we encountered two opening parentheses in a row
                throw std::runtime_error("Parse Error: List nodes must have literal head");
            }
            ++it; // advance past opening parenthesis
            node->tail.push_back(ParseList(it, end));
        }
        else if (node->head.empty()) {
            node->head = *it;
        }
        else {
            node->tail.push_back(std::make_unique<AtomNode>(*it)); // add as child if parent is a ListNode
        }
        ++it;
    }
    return node;
}

