#include "DragController.hh"
#include "Win32.hh"



void DragController::Drag_begin(const MSLLHOOKSTRUCT* mouse_struct) {
    
    m_target_window = Win32Api::Window_FromPoint(mouse_struct -> pt);
    m_sendWinKeyTap = Win32Api::Digital_isWinKeyDown();
    
    if (!m_target_window) {
        return;
    }

    m_target_window = Win32Api::Window_getRoot(m_target_window);

    RECT _rect;
    if (Win32Api::Window_getRect(m_target_window, _rect)) {

        // Put offset on cursor so the window wont jump
        m_offsetX = mouse_struct -> pt.x - _rect.left;
        m_offsetY = mouse_struct -> pt.y - _rect.top;

        m_isActive = true;
    }
}


void DragController::Drag_update(const MSLLHOOKSTRUCT* mouse_struct) {

    if (!m_target_window) {
        return;
    }
    
    int _newX = mouse_struct -> pt.x - m_offsetX;
    int _newY = mouse_struct -> pt.y - m_offsetY;

    Win32Api::Window_moveNoSize(m_target_window, _newX, _newY);
}


void DragController::Drag_end() {

    if (m_isActive) {
        
        // Using 0xE8 because it is an "virtual" key on Windows. This
        // helps by preventing SUPER key to open the Windows menu while
        // the user is dragging.

        // Only send key tap if we started with the Win (SUPER) key.
        if (m_sendWinKeyTap) {
            Win32Api::Signal_VirtualKeyTap(0xE8);
        }

        m_isActive = false;
    }
    
    m_sendWinKeyTap = false;
    m_target_window = nullptr;
}