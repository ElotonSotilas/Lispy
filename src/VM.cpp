//
// Created by George Vassilev on 14.04.23.
//

#include "lispy.h"

VM::VM() : state_(State::INIT_STATE), root_(nullptr) {}

void VM::Run(IO &io) {
    while (state_ != State::EXIT_STATE) {
        switch (state_) {
            case State::INIT_STATE: {
                state_ = State::READ_STATE;
                break;
            }
            case State::READ_STATE: {
                read(io);
                state_ = State::EVAL_STATE;
                break;
            }
            case State::EVAL_STATE: {
                // If eval() returns true, we are going to the print state
                // Else, we go back to init state
                state_ = eval() ? State::PRINT_STATE : State::INIT_STATE;
                break;
            }
            case State::PRINT_STATE: {
                print(io);
                state_ = State::INIT_STATE;
                break;
            }
            default: {
                io.Error("Unknown state.");
                state_ = State::EXIT_STATE;
            }
        }
    }
}

void VM::read(IO &io) {
    auto tokens = Tokeniser().Tokenise(io);

    if (tokens.empty()) {
        io.Log("Finished execution.");
        state_ = State::EXIT_STATE;
    } else {
        root_ = Parser().Parse(tokens);
    }
}

// Helper function for the eval state
bool VM::evalNode(AstNode *node) {
    if (auto listNode = dynamic_cast<ListNode*>(node)) {
        if (listNode->children.empty()) {
            std::cerr << "Argument count problem: "
                         "Operator requires at least one operand."
                      << '\n';
            exit(-1);
        }
        for (auto child : listNode->children) {
            if (evalNode(child)) {
                return true;
            }
        }
    } else if (auto atomNode = dynamic_cast<AtomNode*>(node)) {
        if (atomNode->value == "+") {
            long double result = 0;
            int count = 0;
            if (auto listNode = dynamic_cast<ListNode*>(node->parent)) {
                for (auto child: listNode->children) {
                    auto atom = dynamic_cast<AtomNode *>(child);
                    if (atom && !atom->value.empty() && atom != atomNode) {
                        if (auto x = std::stold(atom->value)) {
                            result += x;
                        } else {
                            std::cerr << "Conversion problem: "
                                         "Operator (+) takes arguments of types Numeric... -> Numeric"
                                      << '\n';
                            exit(-1);
                        }
                        count++;
                    }
                }
            }

            if (count == 0) {
                std::cerr << "Argument count problem: "
                             "Operator (+) does not have any operands."
                          << '\n';

                exit(-1);
            }
            else if (count == 1) {
                std::cerr << "Argument count problem: "
                             "Operator (+) requires at least two operands."
                          << '\n';

                exit(-1);
            }
            else {
                atomNode->value = std::to_string(result);
            }
        }

        else if (atomNode->value == "print") {
            if (auto listNode = dynamic_cast<ListNode*>(node->parent)) {
                if (listNode->children.size() == 1) {
                    std::cerr << "Argument count problem: "
                                 "print method requires at least one argument."
                              << '\n';
                    exit(-1);
                }
                for (auto child: listNode->children) {
                    auto atom = dynamic_cast<AtomNode *>(child);

                    if (atom && !atom->value.empty()) {
                        printer_ << atom->value;
                    }

                }
            }

            return true;
        }
    }

    else {
        throw std::runtime_error("Unknown node type.");
    }

    return false;
}

bool VM::eval() {
    if (root_) {
        return evalNode(root_);
    }

    return false;
}

void VM::print(IO &io) {
    io.Write(printer_.str());
}