#pragma once

#include <Windows.h>

class DragController {

    public:
        void Drag_begin(const MSLLHOOKSTRUCT* mouse_struct);
        void Drag_update(const MSLLHOOKSTRUCT* mouse_struct);
        void Drag_end();

        // Read only for drag state
        bool isActive() const { return m_isDragging; }
    
    private:
        HWND m_target_window = nullptr;

        int m_offsetX = 0;
        int m_offsetY = 0;

        bool m_isDragging = false;
};