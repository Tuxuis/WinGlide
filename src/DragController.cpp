#include "DragController.hh"



void DragController::Drag_begin(const MSLLHOOKSTRUCT* mouse_struct) {
    
    m_target_window = WindowFromPoint(mouse_struct -> pt);

    if (!m_target_window) {
        return;
    }

    m_target_window = GetAncestor(m_target_window, GA_ROOT);

    RECT _rect;
    if (GetWindowRect(m_target_window, &_rect)) {
        m_offsetX = mouse_struct -> pt.x - _rect.left;
        m_offsetY = mouse_struct -> pt.y - _rect.top;

        m_isDragging = true;
    }
}


void DragController::Drag_update(const MSLLHOOKSTRUCT* mouse_struct) {

    if (!m_target_window) {
        return;
    }
    
    int _newX = mouse_struct -> pt.x - m_offsetX;
    int _newY = mouse_struct -> pt.y - m_offsetY;

    SetWindowPos(
        m_target_window, nullptr,
        _newX, _newY, 0, 0,
        SWP_NOSIZE | SWP_NOZORDER
    );
}


void DragController::Drag_end() {

    if (m_isDragging) {

        INPUT _input[2]      = {};

        // Using 0xE8 because it is an "virtual" key on Windows. This
        // helps by preventing SUPER key to open the Windows menu while
        // the user is dragging.  
        _input[0].type       = INPUT_KEYBOARD;
        _input[1].type       = INPUT_KEYBOARD;
        _input[0].ki.wVk     = 0xE8;  // Unassigned key
        _input[1].ki.wVk     = 0xE8;

        _input[1].ki.dwFlags = KEYEVENTF_KEYUP;

        SendInput(2, _input, sizeof(INPUT));
        m_isDragging = false;
    }
    
    m_target_window = nullptr;
}