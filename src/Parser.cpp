//
// Created by George Vassilev on 14.04.23.
//

#include <iterator>
#include "lispy.h"

std::unique_ptr<AstNode> Parser::Parse(std::vector<std::string> &tokens) {
    auto it = tokens.begin();
    auto end = tokens.end();
    return ParseList(it, end);
}

std::unique_ptr<AstNode> Parser::ParseAtom(const std::string& value) {
    return std::make_unique<AtomNode>(value);
}

std::unique_ptr<AstNode> Parser::ParseList(std::vector<std::string>::iterator& it,
                           std::vector<std::string>::iterator& end) {
    auto node = std::make_unique<ListNode>();
    while (it <= end && *it != ")") {
        if (*it == "(") {
            ++it;
            node->children.push_back(ParseList(it, end));
            for (auto& c : node->children) {
                if (c) c->parent = dynamic_cast<AstNode*>(node.get());
            }
        }

        else {
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

