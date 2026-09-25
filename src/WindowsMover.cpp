#include "WindowsMover.hh"
#include "Win32.hh"



WindowMover::WindowMover() : m_worker(&WindowMover::Run, this) {}


WindowMover::~WindowMover() {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stop = true;
    }

    m_wake.notify_one();
    m_worker.join();
}


void WindowMover::Request_move(HWND hwnd, int x, int y) {
    Submit({ hwnd, Action::Move, x, y, 0, 0 });
}


void WindowMover::Request_resize(HWND hwnd, int w, int h) {
    Submit({ hwnd, Action::Resize, 0, 0, w, h });
}


void WindowMover::Submit(const Request& request) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pending    = request;
        m_hasPending = true;
    }

    m_wake.notify_one();
}


void WindowMover::Run() {
    std::unique_lock<std::mutex> lock(m_mutex);

    while (true) {

        m_wake.wait(lock, [this] { return m_hasPending || m_stop; });

        if (m_stop) {
            return;
        }

        const Request request = m_pending;
        m_hasPending = false;

        lock.unlock();
        Apply(request);
        lock.lock();
    }
}


void WindowMover::Apply(const Request& request) {

    switch (request.action) {

        case Action::Move:
            Win32Api::Window_moveNoSize(request.hwnd, request.x, request.y);
            break;

        case Action::Resize:
            Win32Api::Window_resizeNoMove(request.hwnd, request.w, request.h);
            break;
    }
}