//
// Created by George Vassilev on 14.04.23.
//

#include "lispy.h"
#include <sstream>

std::string ConsoleIO::Read() {
    std::string ln;
    std::getline(std::cin, ln);

    return ln;
}

void ConsoleIO::Write(const std::string &output) {
    std::cout << output << '\n';
}

void ConsoleIO::Log(const std::string &log) {
    std::cout << "[LOG] " << log << '\n';
}

void ConsoleIO::Error(const std::string &error) {
    std::cerr << "[ERROR] " << error << '\n';
}