#include "SizeController.hh"
#include "Win32.hh"



bool SizeController::Resize_begin(const MSLLHOOKSTRUCT* mouse_struct) {
    
    HWND target = Win32Api::Window_findTarget(mouse_struct -> pt);

    RECT rect;

    if (!target || !Win32Api::Window_getRect(target, rect)) {
        return false;
    }

    m_target_window = target;
    m_startRect     = rect;
    m_offsetX       = rect.right  - mouse_struct -> pt.x;
    m_offsetY       = rect.bottom - mouse_struct -> pt.y;
    m_isActive      = true;

    return true;
}


void SizeController::Resize_update(const MSLLHOOKSTRUCT* mouse_struct) {
    if (!m_isActive) {
        return;
    }

    const int newRight  = mouse_struct -> pt.x + m_offsetX;
    const int newBottom = mouse_struct -> pt.y + m_offsetY;

    int newWidth  = newRight  - m_startRect.left;
    int newHeight = newBottom - m_startRect.top;

    if (newWidth  < m_minWidth)  newWidth  = m_minWidth;
    if (newHeight < m_minHeight) newHeight = m_minHeight;

    Win32Api::Window_resizeNoMove(m_target_window, newWidth, newHeight);
}


void SizeController::Resize_end() {
    m_isActive      = false;
    m_target_window = nullptr;
}