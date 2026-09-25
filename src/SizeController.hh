#pragma once

#include <Windows.h>

class WindowsMover;

class SizeController {

    public:
        explicit SizeController(WindowsMover& mover) : m_mover(mover) {}
    
        bool Resize_begin(const MSLLHOOKSTRUCT* mouse_struct);
        void Resize_update(const MSLLHOOKSTRUCT* mouse_struct);
        void Resize_end();
        
        bool isActive() const { return m_isActive; }

    private:
        WindowsMover& m_mover;
    
        HWND m_target_window = nullptr;

        RECT m_startRect = {};
        
        int m_offsetX   = 0;
        int m_offsetY   = 0;
        int m_minWidth  = 120;
        int m_minHeight = 80;

        bool m_isActive = false;
};