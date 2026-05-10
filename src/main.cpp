#include <Windows.h>
#include "WindowManager.hpp"

int main(int argc, char **argv) {
    
    // Start Hook window hook
    WindowManager::getInstance().MOUSE_start();

    MSG _msg;
    while (GetMessage(&_msg, nullptr, 0, 0)) {
        TranslateMessage(&_msg);
        DispatchMessage(&_msg);
    }

    WindowManager::getInstance().MOUSE_stop();
    return 0;
}
