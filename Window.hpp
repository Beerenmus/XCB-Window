#pragma once

#include <xcb/xcb.h>
#include <iostream>

#include "Keys.hpp"

class Window final {

    private:
        xcb_connection_t* m_connection;
        const xcb_setup_t* m_setup;
        const xcb_screen_t* m_screen;
        xcb_window_t m_windowID;
        xcb_screen_iterator_t m_screenIterator;

        KeycodeMap map;

    private:
        bool finished = false;
        
    private:
        xcb_atom_t deleteAtom;

    private:
        xcb_atom_t requestAtom(xcb_connection_t* connection, std::string name);

    public:
        void create();
        void show();
        void hide();
        bool advanceToNextFrame();
        void pollEvent();
        void destroy();
};