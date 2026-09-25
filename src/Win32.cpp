#include "Win32.hh"



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


HWND Win32Api::Window_FromPoint(const POINT& Pt) {
    return WindowFromPoint(Pt);
}


HWND Win32Api::Window_getRoot(HWND hwnd) {
    return GetAncestor(hwnd, GA_ROOT);
}


bool Win32Api::Window_getRect(HWND hwnd, RECT& rect) {
    return GetWindowRect(hwnd, &rect) != 0l;
}


void Win32Api::Window_moveNoSize(HWND hwnd, int x, int y) {
    SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Win32Api::Window_resizeNoMove(HWND hwnd, int w, int h) {
    SetWindowPos(hwnd, nullptr, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
}