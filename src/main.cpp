#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <Windows.h>
#include "WindowManager.hh"

int main(int argc, char **argv) {
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    
    // Start Hook window hook
    WindowManager::getInstance().Init();

    MSG _msg;
    while (GetMessage(&_msg, nullptr, 0, 0)) {
        TranslateMessage(&_msg);
        DispatchMessage(&_msg);
    }

    WindowManager::getInstance().Kill();
    return 0;
}
