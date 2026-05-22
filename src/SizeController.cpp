#include "SizeController.hh"
#include "Win32.hh"



void SizeController::Resize_begin(const MSLLHOOKSTRUCT* mouse_struct) {
    
    m_target_window = Win32Api::Window_FromPoint(mouse_struct -> pt);
    m_sendWinKeyTap = Win32Api::Digital_isWinKeyDown();

    if (!m_target_window) {
        return;
    }

    m_target_window = Win32Api::Window_getRoot(m_target_window);

    RECT _rect;
    if (!Win32Api::Window_getRect(m_target_window, _rect)) {
        m_target_window = nullptr;
        return;
    }

    m_startRect = _rect;

    m_offsetX = _rect.right - mouse_struct -> pt.x;
    m_offsetY = _rect.bottom - mouse_struct -> pt.y;

    m_isActive = true;
}


void SizeController::Resize_update(const MSLLHOOKSTRUCT* mouse_struct) {
    
    if (!m_target_window) {
        return;
    }

    const int nRight = mouse_struct -> pt.x + m_offsetX;
    const int nBottom = mouse_struct -> pt.y + m_offsetY;

    int nWidth = nRight - m_startRect.left;
    int nHeight = nBottom - m_startRect.top;

    if (nWidth < m_minWidth) nWidth = m_minWidth;
    if (nHeight < m_minHeight) nHeight = m_minHeight;

    Win32Api::Window_resizeNoMove(m_target_window, nWidth, nHeight);
}


void SizeController::Resize_end() {

    if (m_isActive) {
        // Prevent the Windows menu from popping up on SUPER key release, same
        // as DragController.

        // Only send tap key if Win (SUPER) started it. Same as DragController.
        if (m_sendWinKeyTap) {
            Win32Api::Signal_VirtualKeyTap(0xE8);
        }

        m_isActive = false;
    }

    m_sendWinKeyTap = false;
    m_target_window = nullptr;
}