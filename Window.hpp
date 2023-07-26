#pragma once

#include <xcb/xcb.h>
#include <iostream>

class Window {

    private:
        xcb_connection_t* m_connection;
        const xcb_setup_t* m_setup;
        const xcb_screen_t* m_screen;
        xcb_window_t m_windowID;
        xcb_screen_iterator_t m_screenIterator;

        bool finished = false;

        xcb_atom_t requestAtom(xcb_connection_t* connection, std::string name);

        xcb_atom_t deleteAtom;

    public:
        void create();
        bool advanceToNextFrame();
        void pollEvent();
        void destroy();
};