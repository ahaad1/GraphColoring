/*#include "ConsoleInterface.hpp"*/
#include "../include/ConsoleInterface.hpp"
#include <iostream>

int main() {
    try {
        ConsoleInterface interface;
        interface.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "an error occurred: " << ex.what() << std::endl;
    }
}
