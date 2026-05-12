#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <Windows.h>
#include "WindowManager.hpp"

int main(int argc, char **argv) {
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    
    // Start Hook window hook
    WindowManager::getInstance().Mouse_Stop();

    MSG _msg;
    while (GetMessage(&_msg, nullptr, 0, 0)) {
        TranslateMessage(&_msg);
        DispatchMessage(&_msg);
    }

    WindowManager::getInstance().Mouse_Stop();
    return 0;
}
