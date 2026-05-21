#include "WindowManager.hh"
#include "Win32.hh"



// Create instance in memory
WindowManager &WindowManager::getInstance() {
    static WindowManager _INSTANCE;
    return _INSTANCE;
}


// Intercept Windows mouse events
void WindowManager::Mouse_Start() { 
    m_mouse_hook     = Win32Api::Init_MouseHook(MouseProcess);
    m_keyboard_hook  = Win32Api::Init_KeyboardHook(KeyboardProcess);
}


void WindowManager::Mouse_Stop() {
    Win32Api::Kill_Hook(m_mouse_hook);
    Win32Api::Kill_Hook(m_keyboard_hook);
}


// Clean-up incase of crash or close
WindowManager::~WindowManager() { Mouse_Stop(); }


// Main keyboard event callback
LRESULT CALLBACK WindowManager::KeyboardProcess(int n_code, WPARAM w_param, LPARAM l_Param) {
    if (n_code >= 0) {
        auto* keyboard_struct = reinterpret_cast<KBDLLHOOKSTRUCT*> (l_Param);
        auto& manager = getInstance();

        if (keyboard_struct -> vkCode == VK_LWIN || keyboard_struct -> vkCode == VK_RWIN) {

            if (w_param == WM_KEYUP || w_param == WM_SYSKEYUP) {
                
                if (manager.m_drag.isActive()) {
                    manager.m_drag.Drag_end();
                }

                if (manager.m_size.isActive()) {
                    manager.m_size.Resize_end();
                }
            }
        }
    }

    return Win32Api::Call_NextChain(n_code, w_param, l_Param);
}


// Main mouse event callback
LRESULT CALLBACK WindowManager::MouseProcess(int n_code, WPARAM w_param, LPARAM l_Param) {
    if (n_code >= 0) {
        
        auto* mouse_struct = reinterpret_cast<MSLLHOOKSTRUCT*> (l_Param);
        auto& manager = getInstance();

        if (Win32Api::Mouse_isInjected(mouse_struct)) {
            return Win32Api::Call_NextChain(n_code, w_param, l_Param);
        }


        bool super_press = Win32Api::Digital_isWinKeyDown();

        if (w_param == WM_RBUTTONDOWN && super_press) {
            manager.m_size.Resize_begin(mouse_struct);
            return 1;

        } 
        
        else if (w_param == WM_MOUSEMOVE && manager.m_size.isActive()) {
            manager.m_size.Resize_update(mouse_struct);
        }

        else if (w_param == WM_RBUTTONUP && manager.m_size.isActive()) {
            manager.m_size.Resize_end();
            return 1;
        }
        

        if (w_param == WM_LBUTTONDOWN && super_press) {
            manager.m_drag.Drag_begin(mouse_struct);
            return 1;
        }

        else if (w_param == WM_MOUSEMOVE && manager.m_drag.isActive()) {
            manager.m_drag.Drag_update(mouse_struct);
            //return 1;
        }

        else if (w_param == WM_LBUTTONUP) {
            manager.m_drag.Drag_end();
        }
    }

    return Win32Api::Call_NextChain(n_code, w_param, l_Param);
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