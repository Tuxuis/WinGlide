#pragma once

#include <Windows.h>

class Win32Api {

    public:
        
        // ----- Hooks ----- 

        static HHOOK _Init_MouseHook(HOOKPROC proc);
        static HHOOK _Init_KeyboardHook(HOOKPROC proc);

        static void _Kill_Hook(HHOOK& H);

        static LRESULT _Call_NextChain(int n_code, WPARAM w_param, LPARAM l_param);

        
        // -----  Input ----- 

        static bool Mouse_isInjected(const MSLLHOOKSTRUCT* mouse_struct);
        static bool Digital_isWinKeyDown();

        static void Signal_VirtualKeyUp(WORD vk);


        // ----- Window -----

        static HWND Window_FromPoint(const POINT& _pt);
        static HWND Window_getRoot(HWND _hwnd);
        
        static bool Window_getRect(HWND _hwnd, RECT& _rect);

        static void Window_moveNoSize(HWND _hwnd, int x, int y);
};