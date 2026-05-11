#include "WindowManager.hpp"

// Create instance in memory
WindowManager &WindowManager::getInstance() {
    static WindowManager _INSTANCE;
    return _INSTANCE;
}

// Intercept Windows mouse events
void WindowManager::MOUSE_start() { 
    m_mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, MouseProcess, GetModuleHandle(NULL), 0);
    m_keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProcess, GetModuleHandle(NULL), 0);
}

void WindowManager::MOUSE_stop() {
    if (m_mouse_hook) {

        UnhookWindowsHookEx(m_mouse_hook);
        m_mouse_hook = nullptr;

    } else if (m_keyboard_hook) {
        UnhookWindowsHookEx(m_keyboard_hook);
        m_keyboard_hook = nullptr;        
    }
    
}

// Clean-up incase of crash or close
WindowManager::~WindowManager() { MOUSE_stop(); }

// Main keyboard event callback
LRESULT CALLBACK WindowManager::KeyboardProcess(int n_code, WPARAM w_param, LPARAM l_Param) {
    if (n_code >= 0) {
        auto* keyboard_struct = reinterpret_cast<KBDLLHOOKSTRUCT*> (l_Param);
        auto& _manager = getInstance();

        if (keyboard_struct -> vkCode == VK_LWIN || keyboard_struct -> vkCode == VK_RWIN) {

            if (w_param == WM_KEYUP || w_param == WM_SYSKEYUP) {
                if (_manager.m_drag._is_Dragging()) {
                    _manager.m_drag.Drag_end();
                }
            }
        }
    }

    return CallNextHookEx(nullptr, n_code, w_param, l_Param);
}

// Main mouse event callback
LRESULT CALLBACK WindowManager::MouseProcess(int n_code, WPARAM w_param, LPARAM l_Param) {
    if (n_code >= 0) {
        
        auto* mouse_struct = reinterpret_cast<MSLLHOOKSTRUCT*> (l_Param);
        auto& _manager = getInstance();

        if (mouse_struct -> flags & LLKHF_INJECTED) {
            return CallNextHookEx(nullptr, n_code, w_param, l_Param);
        }

        // 0x8000 => Bitmask check
        bool super_press = (GetAsyncKeyState(VK_LWIN) & 0x8000) || (GetAsyncKeyState(VK_RWIN) & 0x8000);

        if (w_param == WM_LBUTTONDOWN && super_press) {
            _manager.m_drag.Drag_begin(mouse_struct);
            return 1;
        }

        else if (w_param == WM_MOUSEMOVE && _manager.m_drag._is_Dragging()) {
            _manager.m_drag.Drag_update(mouse_struct);
            //return 1;
        }

        else if (w_param == WM_LBUTTONUP) {
            _manager.m_drag.Drag_end();
        }
    }

    return CallNextHookEx(nullptr, n_code, w_param, l_Param);
}


void WindowManager::Hanlde_Mouse_DOWN(MSLLHOOKSTRUCT* mouse_struct) {
    m_drag.Drag_begin(mouse_struct);
}

void WindowManager::Handle_Mouse_MOVE(MSLLHOOKSTRUCT* mouse_struct) {
    m_drag.Drag_update(mouse_struct);
}


void WindowManager::Handle_Mouse_UP() {
    m_drag.Drag_end();
}