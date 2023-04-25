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
                eval();
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
    auto tokens = Tokeniser::Tokenise(io);

    if (tokens.empty()) {
        io.Log("Finished execution.");
    } else {
        root_ = Parser().Parse(tokens);
    }
}

// Helper function for the eval state
std::string VM::evalNode(const AstNode& node) {
    if (auto atomNode = dynamic_cast<const AtomNode*>(&node)) {
        return atomNode->value;
    }

    auto& listNode = dynamic_cast<const ListNode&>(node);
    

    if (listNode.head == "+") {
        long double sum = 0;
        
        for (auto& arg : listNode.tail) {
            auto evaledArg = evalNode(*arg);
            if (evaledArg.empty()) {
                throw std::runtime_error("Using empty result as argument to addition");
            }
            try {
                auto x = std::stold(evaledArg);
                sum += x;
            }
            catch (...) {
                throw std::runtime_error("Conversion problem:"
                                         "\nOperator (+) takes arguments of types Numeric... -> Numeric");
            }
        }

        return std::to_string(sum);
    }

    if (listNode.head == "print") {
        if (listNode.tail.empty()) {
            throw std::runtime_error("Argument count problem:"
                                     "\nPrinting requires at least one operand.");
        }

        for (auto& arg : listNode.tail) {
            std::cout << evalNode(*arg) << ' ';
        }

        std::cout << '\n';

        return "";
    }

    throw std::runtime_error("Unknown function: " + listNode.head);
}

void VM::eval() {
    if (root_) {
        evalNode(*root_);
    }

    // reset root
    root_ = nullptr;
}