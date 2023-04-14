//
// Created by George Vassilev on 14.04.23.
//

#include "lispy.h"

VM::VM() : state_(State::INIT_STATE), root_(nullptr) {}

void VM::Run(IO &io) {
    while (state_ != State::EXIT_STATE) {
        switch (state_) {
            case State::INIT_STATE: {
                read(io);
                state_ = State::READ_STATE;
                break;
            }
            case State::EVAL_STATE: {
                eval(io);
                state_ = State::PRINT_STATE;
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
        io.Log("No input provided. Finished execution.");
        state_ = State::EXIT_STATE;
    } else {
        root_ = Parser().Parse(tokens);
    }
}

void VM::eval(IO &io) {

}

void VM::print(IO &io) {

}