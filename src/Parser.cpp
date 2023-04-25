//
// Created by George Vassilev on 14.04.23.
//

#include <iterator>
#include "lispy.h"

std::unique_ptr<AstNode> Parser::Parse(std::vector<std::string> &tokens) {
    auto it = tokens.begin();
    return ParseList(it, tokens.end());
}

std::unique_ptr<AstNode> Parser::ParseAtom(const std::string& value) {
    std::cout << value << "\n";
    return std::make_unique<AtomNode>(value);
}

std::unique_ptr<AstNode> Parser::ParseList(std::vector<std::string>::iterator& it,
                           const std::vector<std::string>::const_iterator& end) {
    auto node = std::make_unique<ListNode>();
    while (it <= end && *it != ")") {
        if (*it == "(") {
            ++it;
            node->children.push_back(ParseList(it, end));
            for (auto& c : node->children) {
                c->parent = dynamic_cast<AstNode*>(node.get());
            }
        } else {
            auto child = ParseAtom(*it);
            if (auto listNode = dynamic_cast<ListNode*>(node.get())) {
                child->parent = listNode;
                listNode->children.push_back(std::move(child)); // add as child if parent is a ListNode
            }
        }
        ++it;
    }
    tree.push_back(std::move(node));
    return node;
}

