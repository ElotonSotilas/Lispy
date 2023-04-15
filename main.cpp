#include <iostream>
#include "lispy.h"

int main(int argc, char** argv) {
    ConsoleIO io = ConsoleIO();
    VM app = VM();

    app.Run(io);
}
