#pragma once

#include <Windows.h>

class WindowManager {
    
    public:
        static WindowManager& getInstance();

        void MOUSE_start();
        void MOUSE_stop();

    private:
        WindowManager() = default;
        ~WindowManager();

        static LRESULT CALLBACK MOUSE_process(int n_Code, WPARAM w_Param, LPARAM l_Param);

        void Handle_Mouse_MOVE(MSLLHOOKSTRUCT* mouse_struct);
        void Hanlde_Mouse_DOWN(MSLLHOOKSTRUCT* mouse_struct);
        void Handle_Mouse_UP();

        HHOOK m_mouse_hook     = nullptr;
        HHOOK m_target_window  = nullptr;
        POINT m_lastMouse_pos  = {0, 0};

        bool m_isDragging      = false;
};
