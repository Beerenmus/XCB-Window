#include "Window.hpp"

int main() {

    Window window;
    window.create();

    window.show();

    while(window.advanceToNextFrame()) {

        window.pollEvent();
    } 

    window.hide();

    window.destroy();

    return 0;
}