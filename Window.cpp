#include "Window.hpp"

xcb_atom_t Window::requestAtom(xcb_connection_t* connection, std::string name) {
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection, 0, static_cast<uint16_t>(name.length()), name.c_str());
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(connection, cookie, NULL);
    xcb_atom_t atom = reply->atom;
    free(reply);
    return atom;
}

void Window::create() {

        m_connection = xcb_connect(NULL, NULL);
    if (m_connection == NULL || xcb_connection_has_error(m_connection) < 0) {
        std::cout << "Fehler beim Herstellen der Verbindung zum X-Server." << std::endl;
        return;
    }

    m_setup = xcb_get_setup(m_connection);
    if (m_setup == nullptr) {
        std::cout << "Fehler beim Abrufen des X-Server-Setups." << std::endl;
        xcb_disconnect(m_connection); // Verbindung trennen, da Fehler aufgetreten ist
        return;
    }

    m_screenIterator = xcb_setup_roots_iterator(m_setup);
    m_screen = m_screenIterator.data;
    if (m_screen == nullptr) {
        std::cout << "Fehler beim Abrufen des X-Server-Bildschirms." << std::endl;
        xcb_disconnect(m_connection); // Verbindung trennen, da Fehler aufgetreten ist
        return;
    }


    uint32_t values[2];
    uint32_t mask;

    m_windowID = xcb_generate_id(m_connection);

    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_KEY_RELEASE;
    values[0] = m_screen->black_pixel;
    xcb_create_window(m_connection, m_screen->root_depth, m_windowID, m_screen->root, 10, 10, 1200, 720, 1, XCB_WINDOW_CLASS_INPUT_OUTPUT, m_screen->root_visual, mask, values);

    deleteAtom = requestAtom(m_connection, "WM_DELETE_WINDOW");
    xcb_atom_t protocolAtom = requestAtom(m_connection, "WM_PROTOCOLS");

    xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_windowID, protocolAtom, 4, 32, 1, &deleteAtom);
    xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_windowID, XCB_ATOM_WM_HINTS, XCB_ATOM_ATOM, 32, 1, &deleteAtom);

    std::string title("Vulkan Application");
    xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_windowID, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, static_cast<uint32_t>(title.length()), title.c_str());
    
    xcb_map_window(m_connection, m_windowID);

    xcb_flush(m_connection);
}

bool Window::advanceToNextFrame() {
    return !finished;
}

void Window::pollEvent() {

    xcb_generic_event_t* generic_event;

    while(generic_event = xcb_poll_for_event(m_connection)) {

            switch (generic_event->response_type & ~0x80) {

                case XCB_EXPOSE:
                {
                    xcb_expose_event_t* event = reinterpret_cast<xcb_expose_event_t*>(generic_event);
                    std::cout << "Width: " << event->width << " " << "Height: " << event->height << std::endl;
                    xcb_flush(m_connection);
                }

                case XCB_KEY_RELEASE: 
                {
                    xcb_key_press_event_t* event = reinterpret_cast<xcb_key_press_event_t*>(generic_event);
                    switch(event->detail) {

                        case 9:
                            finished = true;
                            break;   
                    }
                }

                case XCB_DESTROY_NOTIFY:
                {
                    xcb_destroy_notify_event_t* destroyEvent = reinterpret_cast<xcb_destroy_notify_event_t*>(generic_event);
                    if (destroyEvent->window == m_windowID) {
                        std::cout << "Window destroyed!" << std::endl;
                    }
                }
                break;

                case XCB_CLIENT_MESSAGE:
                {
                    xcb_client_message_event_t* clientMessageEvent = reinterpret_cast<xcb_client_message_event_t*>(generic_event);
                    
                    if (clientMessageEvent->window == m_windowID && clientMessageEvent->data.data32[0] == deleteAtom) {
                        finished = true;
                    }
                }
                break;
            }
            
            free(generic_event);
        }
}

void Window::destroy() {

    xcb_destroy_window(m_connection, m_windowID);
    xcb_flush(m_connection);

    xcb_disconnect(m_connection);
}