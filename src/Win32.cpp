#include "Win32.hh"



namespace {

    constexpr WORD START_MENU_SUPPRESS_KEY = 0xE8;
    constexpr UINT NON_BLOCKING_POS_FLAGS = SWP_NOZORDER | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS;

    bool Window_isShell(HWND hwnd) {
        static constexpr const wchar_t* SHELL_CLASS[] = {
            L"Progman", L"WorkerW", L"Shell_TrayWnd", L"Shell_SecondaryTrayWnd"
        };

        wchar_t class_name[64] = {};
        GetClassNameW(hwnd, class_name, ARRAYSIZE(class_name));

        for (const wchar_t* shell_class : SHELL_CLASS) {
            if (wcscmp(class_name, shell_class) == 0) {
                return true;
            }
        }

        return false;
    }
}


HHOOK Win32Api::Init_MouseHook(HOOKPROC proc) {
    return SetWindowsHookEx(WH_MOUSE_LL, proc, GetModuleHandle(nullptr), 0);
}


HHOOK Win32Api::Init_KeyboardHook(HOOKPROC proc) {
    return SetWindowsHookEx(WH_KEYBOARD_LL, proc, GetModuleHandle(nullptr), 0);
}


void Win32Api::Kill_Hook(HHOOK& H) {
    if (H) {
        UnhookWindowsHookEx(H);
        H = nullptr;
    }
}


LRESULT Win32Api::Call_NextChain(int n_Code, WPARAM w_Param, LPARAM l_Param) {
    return CallNextHookEx(nullptr, n_Code, w_Param, l_Param);
}


bool Win32Api::Mouse_isInjected(const MSLLHOOKSTRUCT* mouse_struct) {
    return (mouse_struct -> flags & LLMHF_INJECTED) != 0;
}


bool Win32Api::Digital_isWinKeyDown() {
    return (GetAsyncKeyState(VK_LWIN) & 0x8000) != 0 || (GetAsyncKeyState(VK_RWIN) & 0x8000) != 0;
}


void Win32Api::Signal_VirtualKeyTap(WORD vk) {
     INPUT _input[2]      = {};
     
     _input[0].type       = INPUT_KEYBOARD;
     _input[1].type       = INPUT_KEYBOARD;
     _input[0].ki.wVk     = vk;  // Unassigned key
     _input[1].ki.wVk     = vk;
     _input[1].ki.dwFlags = KEYEVENTF_KEYUP;

     SendInput(2, _input, sizeof(INPUT));
}


void Win32Api::Signal_SuppressStartMenu() {
   Signal_VirtualKeyTap(START_MENU_SUPPRESS_KEY); 
}


HWND Win32Api::Window_findTarget(const POINT& pt) {
    HWND hwnd = WindowFromPoint(pt);

    if (!hwnd) {
        return nullptr;
    }

    hwnd = GetAncestor(hwnd, GA_ROOT);

    if (!hwnd || Window_isShell(hwnd)) {
        return nullptr;
    }

    if (!IsWindowVisible(hwnd) || IsZoomed(hwnd) || IsIconic(hwnd)) {
        return nullptr;
    }

    return hwnd;
}


bool Win32Api::Window_getRect(HWND hwnd, RECT& rect) {
    return GetWindowRect(hwnd, &rect) != 0l;
}


void Win32Api::Window_moveNoSize(HWND hwnd, int x, int y) {
    SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | NON_BLOCKING_POS_FLAGS);
}

void Win32Api::Window_resizeNoMove(HWND hwnd, int w, int h) {
    SetWindowPos(hwnd, nullptr, 0, 0, w, h, SWP_NOMOVE | NON_BLOCKING_POS_FLAGS);
}