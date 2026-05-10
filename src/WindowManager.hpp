#pragma once

#include <Windows.h>

class WindowManager {
    
    public:
        static WindowManager& getInstance();

        void MOUSE_start();
        void MOUSE_stop();
        void Handle_Mouse_UP();

    private:
        WindowManager() = default;
        ~WindowManager();

        static LRESULT CALLBACK MouseProcess(int n_Code, WPARAM w_Param, LPARAM l_Param);
        static LRESULT CALLBACK KeyboardProcess(int n_code, WPARAM w_param, LPARAM l_Param);

        void Handle_Mouse_MOVE(MSLLHOOKSTRUCT* mouse_struct);
        void Hanlde_Mouse_DOWN(MSLLHOOKSTRUCT* mouse_struct);

        HHOOK m_mouse_hook     = nullptr;
        HHOOK m_keyboard_hook   = nullptr;
        HWND  m_target_window  = nullptr;
        
        int m_offsetX = 0;
        int m_offsetY = 0;

        bool m_isDragging      = false;
};
