//
// Created by George Vassilev on 12.03.23.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <array>
#include <sstream>
#include <memory>

// IO class is an abstract class that feeds a data stream into the tokeniser
class IO {
public:
    virtual std::string Read() = 0;
    virtual void Write(const std::string& output) = 0;
    virtual void Log(const std::string& log) = 0;
    virtual void Error(const std::string& error) = 0;
    virtual ~IO() = default;
};

// FileIO class extends IO
class FileIO : public IO {
public:
    explicit FileIO(const std::string& fileName);
    std::string Read() override;
    void Write(const std::string& output) override;
    void Log(const std::string& log) override;
    void Error(const std::string& error) override;
    ~FileIO() override = default;

private:
    std::unique_ptr<std::fstream> fileStream;
};

// ConsoleIO class extends IO
class ConsoleIO : public IO {
public:
    ConsoleIO() = default;
    ~ConsoleIO() override = default;
    std::string Read() override;
    void Write(const std::string& output) override;
    void Log(const std::string& log) override;
    void Error(const std::string& error) override;
};

// Abstract Syntax Tree node
class AstNode {
public:
    explicit AstNode() = default;
    virtual ~AstNode() = default;
};

// Terminal AST node
class AtomNode : public AstNode {
public:
    explicit AtomNode(std::string value) : value(std::move(value)) {}
    std::string value;
};

// Non-terminal AST node
class ListNode : public AstNode {
public:
    explicit ListNode() = default;
    std::string head;
    std::vector<std::unique_ptr<AstNode>> tail;
};

// Parser class that splits tokens using an AST
class Parser {
private:
    std::unique_ptr<AstNode> ParseList(std::vector<std::string>::const_iterator& it,
                                       std::vector<std::string>::const_iterator end);

public:
    std::unique_ptr<AstNode> Parse(const std::vector<std::string>& tokens);
};

// Tokeniser class that translates tokens into the parser
class Tokeniser {
public:
    static std::vector<std::string> Tokenise(IO& input);
};

// VM class is a finite state machine
class VM {
public:
    // Enum for FSM states
    enum class State {
        INIT_STATE,
        READ_STATE,
        EVAL_STATE,
        EXIT_STATE
    };

    VM();
    void Run(IO& io);

private:
    State state_;
    std::unique_ptr<AstNode> root_;
    void read(IO& io);
    void eval();
    std::string evalNode(const AstNode& node);
};

// // Multiple inheritance class that will be used to initialise
// // a runtime with a specific input
//class Lispy : public VM, public IO {
//public:
//    enum class InputType {
//        FILE,
//        CONSOLE
//    };
//    Lispy(InputType inputType,
//          std::string* inputValue = nullptr); // If it's a file, inputValue is a filepath
//                                              // Otherwise, the value is ignored (default nullptr)
//    std::string Read() override;
//    void Write(const std::string& output) override;
//    void Log(const std::string& log) override;
//    void Error(const std::string& error) override;
//    void Run();
//private:
//    std::unique_ptr<IO> io_;
//};
