add_library(LispyLib STATIC
        ConsoleIO.cpp
        FileIO.cpp
        Parser.cpp
        Tokeniser.cpp
        VM.cpp)

target_include_directories(LispyLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../includes)
