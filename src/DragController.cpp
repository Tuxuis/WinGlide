#include "DragController.hh"
#include "Win32.hh"



bool DragController::Drag_begin(const MSLLHOOKSTRUCT* mouse_struct) {

    HWND target = Win32Api::Window_findTarget(mouse_struct -> pt);
    
    RECT rect;

    if (!target || !Win32Api::Window_getRect(target, rect)) {
        return false;
    }

    m_target_window = target;
    m_offsetX       = mouse_struct -> pt.x - rect.left;
    m_offsetY       = mouse_struct -> pt.y - rect.top;
    m_isActive      = true;

    return true;
}


void DragController::Drag_update(const MSLLHOOKSTRUCT* mouse_struct) {

    if (!m_isActive) {
        return;
    }
    
    const int newX = mouse_struct -> pt.x - m_offsetX;
    const int newY = mouse_struct -> pt.y - m_offsetY;

    Win32Api::Window_moveNoSize(m_target_window, newX, newY);
}


void DragController::Drag_end() {
    m_isActive      = false;
    m_target_window = nullptr;
}