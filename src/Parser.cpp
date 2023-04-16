//
// Created by George Vassilev on 14.04.23.
//

#include <iterator>
#include "lispy.h"

AstNode *Parser::Parse(std::vector<std::string> &tokens) {
    auto it = tokens.begin();
    return ParseList(it, tokens.end());
}

AstNode* Parser::ParseAtom(std::string value) {
    return new AtomNode{std::move(value)};
}

AstNode* Parser::ParseList(std::vector<std::string>::iterator& it,
                           const std::vector<std::string>::const_iterator& end) {
    auto node = new ListNode();
    while (it < end && *it != ")") {
        if (*it == "(") {
            ++it;
            node->children.push_back(ParseList(it, end));
        } else {
            node->children.push_back(ParseAtom(*it));
        }
        ++it;
    }
    return node;
}
