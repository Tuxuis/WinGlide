#pragma once

#include <Windows.h>
#include "DragController.hh"
#include "SizeController.hh"
#include "WindowsMover.hh"


class WindowManager {
    
    public:
        static WindowManager& getInstance();

        bool Handle_WinKey_UP(WORD win_vk);

        bool Init();
        void Kill();

    private:
        WindowManager() = default;
        ~WindowManager();

        static LRESULT CALLBACK MouseProcess(int n_Code, WPARAM w_Param, LPARAM l_Param);
        static LRESULT CALLBACK KeyboardProcess(int n_Code, WPARAM w_Param, LPARAM l_Param);

        bool Handle_Mouse_DOWN(WPARAM button, const MSLLHOOKSTRUCT* mouse_struct);
        void Handle_Mouse_MOVE(const MSLLHOOKSTRUCT* mouse_struct);
        bool Handle_Mouse_UP(WPARAM button);

        bool isBusy() const { return m_drag.isActive() || m_size.isActive(); }

        HHOOK m_mouse_hook     = nullptr;
        HHOOK m_keyboard_hook  = nullptr;

        WindowsMover m_mover;
        DragController m_drag { m_mover };
        SizeController m_size { m_mover };

        bool m_maskWinKeyUp = false;
};
