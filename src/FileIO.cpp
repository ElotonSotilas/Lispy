//
// Created by George Vassilev on 14.04.23.
//

#include "lispy.h"
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

// Constructor
inline FileIO::FileIO(std::string fileName) {
    fileStream.open(fileName);

    if (fileStream.fail()) {
        fs::path p{fileName};

        std::cerr << "Error occurred: "
                  << "Cannot open file -> "
                  << '"' << fs::absolute(p) << '"' << "\n";
        exit(-1);
    }
}

// Read the file into a string.
inline std::string FileIO::Read() {
    std::stringstream buf{""}; // initialised as an empty string
    std::string line;

    if (fileStream.eof() || !fileStream)
        return buf.str();

    while (std::getline(fileStream, line)) {
        buf << line << '\n';
    }

    return buf.str();
}

inline void FileIO::Write(const std::string &output) {
    fileStream << output << '\n';
}

inline void FileIO::Log(const std::string &log) {
    fileStream << "[LOG] " << log << '\n';
}

inline void FileIO::Error(const std::string &error) {
    fileStream << "[ERROR] " << error << '\n';
}