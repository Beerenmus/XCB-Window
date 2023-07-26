#include "Window.hpp"

int main() {

    Window window;
    window.create();

    while(window.advanceToNextFrame()) {

        window.pollEvent();
    } 

    window.destroy();

    return 0;
}