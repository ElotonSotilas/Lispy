//
// Created by George Vassilev on 12.03.23.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <array>

// custom function for me so I don't have
// to count the amount of elements when writing an std::array
template <typename... T>
constexpr auto make_array(T&&... values) ->
std::array<typename std::decay<typename std::common_type<T...>::type>::type,
           sizeof...(T)>
{
    return std::array<
            typename std::decay<
                    typename std::common_type<T...>::type>::type,
            sizeof...(T)>{std::forward<T>(values)...};
}

const auto Methods = make_array(
            "print",
            "println",
            "+",
            "-",
            "/",
            "*",
            "%",
            "defunc",
            "defstruct",
            "member",
            "if",
            "else",
            "setq",
            "while",
            "return",
            "break",
            "continue"
        );

const auto Types = make_array(
            "number",
            "string",
            "function",
            "boolean",
            "struct"
        );

// Enum for FSM states
enum class State {
    INIT_STATE,
    READ_STATE,
    EVAL_STATE,
    PRINT_STATE,
    EXIT_STATE
};

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

private:
    std::ifstream fileStream;
};

// ConsoleIO class extends IO
class ConsoleIO : public IO {
public:
    std::string Read() override;
    void Write(const std::string& output) override;
    void Log(const std::string& log) override;
    void Error(const std::string& error) override;
};

// Abstract Syntax Tree node
class AstNode {
public:
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

public:
    AstNode* Parse(std::vector<std::string>& tokens);
};

// Tokeniser class that translates tokens into the parser
class Tokeniser : public Parser {
public:
    std::vector<std::string> Tokenise(IO& input);
};

// VM class is a finite state machine
class VM {
public:
    void Run(IO& io);

private:
    State state_;
    AstNode* root_;
    void read(IO& io);
    void eval(IO& io);
    void print(IO& io);
};

// Multiple inheritance class that will be used to initialise a runtime with a specific input
class Lispy : private VM, public IO {
public:
    Lispy(std::string inputType, std::string inputValue); // If it's a file, inputValue is a filepath
    std::string Read() override;
    void Write(const std::string& output) override;
    void Log(const std::string& log) override;
    void Error(const std::string& error) override;
    void Run();
private:
    std::unique_ptr<IO> io_;
};
