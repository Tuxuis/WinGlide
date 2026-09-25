#include "WindowManager.hh"
#include "Win32.hh"



// Create instance in memory
WindowManager &WindowManager::getInstance() {
    static WindowManager Instance;
    return Instance;
}


// Intercept Windows mouse events
bool WindowManager::Init() { 
    m_mouse_hook     = Win32Api::Init_MouseHook(MouseProcess);
    m_keyboard_hook  = Win32Api::Init_KeyboardHook(KeyboardProcess);

    if (!m_mouse_hook || !m_keyboard_hook) {
        Kill();
        return false;
    }

    return true;
}


void WindowManager::Kill() {
    Win32Api::Kill_Hook(m_mouse_hook);
    Win32Api::Kill_Hook(m_keyboard_hook);
}


WindowManager::~WindowManager() { Kill(); }


// Main keyboard event callback
LRESULT CALLBACK WindowManager::KeyboardProcess(int n_Code, WPARAM w_Param, LPARAM l_Param) {

    if (n_Code >= 0) {

        const auto* keyboard_struct = reinterpret_cast<KBDLLHOOKSTRUCT*>(l_Param);

        const bool is_win_key = keyboard_struct -> vkCode == VK_LWIN || keyboard_struct -> vkCode == VK_RWIN;
        const bool is_key_up  = w_Param == WM_KEYUP || w_Param == WM_SYSKEYUP;

        if (is_win_key && is_key_up) {

            auto& manager = getInstance();

            manager.m_drag.Drag_end();
            manager.m_size.Resize_end();
            
        }
    }

    return Win32Api::Call_NextChain(n_Code, w_Param, l_Param);
}


// Main mouse event callback
LRESULT CALLBACK WindowManager::MouseProcess(int n_Code, WPARAM w_Param, LPARAM l_Param) {
    
    if (n_Code < 0) {
        return Win32Api::Call_NextChain(n_Code, w_Param, l_Param);
    }

    const auto* mouse_struct = reinterpret_cast<MSLLHOOKSTRUCT*>(l_Param);

    if (Win32Api::Mouse_isInjected(mouse_struct)) {
        return Win32Api::Call_NextChain(n_Code, w_Param, l_Param);
    }

    auto& manager = getInstance();
    bool consumed = false;

    switch (w_Param) {

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            consumed = manager.Handle_Mouse_DOWN(w_Param, mouse_struct);
            break;

        case WM_MOUSEMOVE:
            manager.Handle_Mouse_MOVE(mouse_struct);
            break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
            consumed = manager.Handle_Mouse_UP(w_Param);
            break;
    }


    return consumed ? 1 : Win32Api::Call_NextChain(n_Code, w_Param, l_Param);
}


bool WindowManager::Handle_Mouse_DOWN(WPARAM button, const MSLLHOOKSTRUCT* mouse_struct) {
    
    if (!Win32Api::Digital_isWinKeyDown() || isBusy()) {
        return false;
    }

    const bool started = (button == WM_LBUTTONDOWN) ? m_drag.Drag_begin(mouse_struct) : m_size.Resize_begin(mouse_struct);

    // Must be sent while Win is still held
    if (started) {
        Win32Api::Signal_SuppressStartMenu();
    }

    return started;
}


void WindowManager::Handle_Mouse_MOVE(const MSLLHOOKSTRUCT* mouse_struct) {
    if (m_drag.isActive()) m_drag.Drag_update(mouse_struct);
    if (m_size.isActive()) m_size.Resize_update(mouse_struct);
}


bool WindowManager::Handle_Mouse_UP(WPARAM button) {
    
    if (button == WM_LBUTTONUP && m_drag.isActive()) {
        m_drag.Drag_end();
        return true;
    }

    if (button == WM_RBUTTONUP && m_size.isActive()) {
        m_size.Resize_end();
        return true;
    }

    return false;
}