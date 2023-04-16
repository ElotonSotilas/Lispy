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
    FileIO(std::string fileName);
    std::string Read() override;
    void Write(const std::string& output) override;
    void Log(const std::string& log) override;
    void Error(const std::string& error) override;
    ~FileIO() override {
        delete &fileStream;
    }

private:
    std::fstream fileStream;
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
    AstNode(AstNode* parent = nullptr) : parent(parent) {}
    virtual ~AstNode() = default;
    AstNode* parent;
};

// Terminal AST node
class AtomNode : public AstNode {
private:
    enum class AtomType {
        NUMERIC,
        STRING
    };

public:
    explicit AtomNode(std::string value, AstNode* parent = nullptr)
            : AstNode(parent), value(std::move(value)) {}
    std::string value;

    AtomType type;
};

// Non-terminal AST node
class ListNode : public AstNode {
public:
    explicit ListNode(AstNode* parent = nullptr) : AstNode(parent) {}
    std::vector<AstNode*> children;
    ~ListNode() override {
        for (auto& child : children) {
            delete child;
        }
    }
};


// Parser class that splits tokens using an AST
class Parser {
protected:
    std::vector<ListNode> tree;

private:
    AstNode* ParseAtom(std::string value);
    AstNode* ParseList(std::vector<std::string>::iterator& it,
                       const std::vector<std::string>::const_iterator& end);

public:
    AstNode* Parse(std::vector<std::string>& tokens);

};

// Tokeniser class that translates tokens into the parser
class Tokeniser {
public:
    std::vector<std::string> Tokenise(IO& input);
};

// VM class is a finite state machine
class VM {
public:
    // Enum for FSM states
    enum class State {
        INIT_STATE,
        READ_STATE,
        EVAL_STATE,
        PRINT_STATE,
        EXIT_STATE
    };

    VM();
    void Run(IO& io);

private:
    State state_;
    AstNode* root_;
    std::stringstream printer_;
    void read(IO& io);
    bool eval();
    bool evalNode(AstNode* node);
    void print(IO& io);
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
