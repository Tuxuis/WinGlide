#pragma once

#include <Windows.h>

class WindowsMover;

class DragController {

    public:
        explicit DragController(WindowsMover& mover) : m_mover(mover) {}

        bool Drag_begin(const MSLLHOOKSTRUCT* mouse_struct);
        void Drag_update(const MSLLHOOKSTRUCT* mouse_struct);
        void Drag_end();

        bool isActive() const { return m_isActive; }
    
    private:
        WindowsMover& m_mover;

        HWND m_target_window = nullptr;

        int m_offsetX = 0;
        int m_offsetY = 0;

        bool m_isActive = false;
};