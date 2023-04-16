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

bool VM::eval() {
    if (auto listNode = dynamic_cast<ListNode*>(root_)) {
        for (auto node : listNode->children) {
            root_ = node;  // Set the root to the current child node
            eval();  // Recursively evaluate the child node
        }
    }

    else if (auto atomNode = dynamic_cast<AtomNode*>(root_)) {
        if (atomNode->value == "+") {
            long double result = 0;
            int count = 0;
            auto listNode = dynamic_cast<ListNode*>(root_->parent);
            for (auto child : listNode->children) {
                auto atom = dynamic_cast<AtomNode*>(child);

                // Check if the atom exists and is not empty, and not the same as atomNode

                // NOTICE: the last comparison can be avoided if using indexes,
                // instead of a range-based for loop
                if (atom && !atom->value.empty() && atom != atomNode) {
                    if (auto x = std::stold(atom->value)) {
                        result += x;
                    }
                    else {
                        std::cerr << "Conversion problem: "
                                     "Operator (+) takes arguments of types Numeric... -> Numeric"
                                     << '\n';
                        exit(-1);
                    }
                    count++;
                }
            }

            if (count != 0) atomNode->value = std::to_string(result);

            else {
                std::cerr << "Argument count problem: "
                             "Operator (+) does not have any operands."
                          << '\n';

                exit(-1);
            }
        }

        else if (atomNode->value == "print") {
            auto listNode = dynamic_cast<ListNode*>(root_->parent);
            for (auto child : listNode->children) {
                auto atom = dynamic_cast<AtomNode*>(child);

                if (atom && !atom->value.empty()) {
                    printer_ << atom->value;
                }

                else {
                    std::cerr << "Argument count problem: "
                                 "print method requires at least one argument."
                              << '\n';
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


void VM::print(IO &io) {
    io.Write(printer_.str());
}