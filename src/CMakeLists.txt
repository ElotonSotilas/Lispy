add_library(LispyLib STATIC
        ConsoleIO.cpp
        FileIO.cpp
        Parser.cpp
        Tokeniser.cpp
        VM.cpp Lispy.cpp)

target_include_directories(LispyLib PUBLIC ${PROJECT_SOURCE_DIR}/includes)