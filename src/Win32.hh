#pragma once

#include <Windows.h>

class Win32Api {

    public:
        
        // ----- Hooks ----- 

        static HHOOK Init_MouseHook(HOOKPROC proc);
        static HHOOK Init_KeyboardHook(HOOKPROC proc);

        static void Kill_Hook(HHOOK& H);

        static LRESULT Call_NextChain(int n_Code, WPARAM w_Param, LPARAM l_Param);

        
        // -----  Input ----- 

        static bool Mouse_isInjected(const MSLLHOOKSTRUCT* mouse_struct);
        static bool Digital_isWinKeyDown();

        static bool Key_isInjected(const KBDLLHOOKSTRUCT* keyboard_struct);
        static void Signal_MaskedWinKeyUp(WORD win_vk);

        // ----- Window -----

        static HWND Window_findTarget(const POINT& pt);
                
        static bool Window_getRect(HWND hwnd, RECT& _rect);

        static void Window_moveNoSize(HWND hwnd, int x, int y);
        static void Window_resizeNoMove(HWND hwnd, int w, int h);
};