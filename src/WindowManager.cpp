#include "WindowManager.hpp"

// Create instance in memory
WindowManager &WindowManager::getInstance() {
    static WindowManager _INSTANCE;
    return _INSTANCE;
}

// Intercept Windows mouse events
void WindowManager::MOUSE_start() { m_mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, MOUSE_process, GetModuleHandle(NULL), 0); }
void WindowManager::MOUSE_stop()
{
    if (m_mouse_hook) {

        UnhookWindowsHookEx(m_mouse_hook);
        m_mouse_hook = nullptr;
    }
}

// Clean-up incase of crash or close
WindowManager::~WindowManager() {
    MOUSE_stop();
}


// Main event callback
LRESULT CALLBACK WindowManager::MOUSE_process(int n_code, WPARAM w_param, LPARAM l_Param) {
    if (n_code >= 0) {
        
        auto* mouse_struct = reinterpret_cast<MSLLHOOKSTRUCT*> (l_Param);
        auto& _manager     = getInstance();

        // 0x8000 => Bitmask check
        bool super_press = (GetAsyncKeyState(VK_LWIN) & 0x8000) || (GetAsyncKeyState(VK_RWIN) & 0x8000);

    }
}


void WindowManager::Hanlde_Mouse_DOWN(MSLLHOOKSTRUCT* mouse_struct) {
    m_target_window = WindowFromPoint(mouse_struct -> pt);

    if (m_target_window) {

        // Force it to grab parent window instead
        m_target_window = GetAncestor(m_target_window, GA_ROOT);
        m_lastMouse_pos = mouse_struct -> pt;
        m_isDragging    = true;
    }
}

void WindowManager::Handle_Mouse_MOVE(MSLLHOOKSTRUCT* mouse_struct) {
    RECT _rect;

    // Get position and size of window
    if (GetWindowRect(m_target_window, &_rect)) {

        int _dX = mouse_struct -> pt.x - m_lastMouse_pos.x; 
        int _dY = mouse_struct -> pt.y - m_lastMouse_pos.y;
        
        SetWindowPos(
            m_target_window, nullptr,
            _rect.left + _dX,
            _rect.top + _dY,
            0, 0,
            SWP_NOSIZE | SWP_NOZORDER
        );
    }
}
