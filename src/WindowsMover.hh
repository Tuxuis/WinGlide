#pragma once

#include <Windows.h>
#include <condition_variable>
#include <mutex>
#include <thread>


class WindowMover {

    public:
        WindowMover();
        ~WindowMover();

        WindowMover(const WindowMover&)            = delete;
        WindowMover& operator=(const WindowMover&) = delete;

        void Request_move(HWND hwnd, int x, int y);
        void Request_resize(HWND hwnd, int w, int h);

    private:
        enum class Action { Move, Resize };

        struct Request {
            HWND   hwnd   = nullptr;
            Action action = Action::Move;
            
            int x = 0, y = 0, w = 0, h = 0;
        };

        void Submit(const Request& request);
        void Run();

        static void Apply(const Request& request);

        std::mutex              m_mutex;
        std::condition_variable m_wake;

        Request m_pending;
        bool    m_hasPending = false;
        bool    m_stop       = false;


        // Declared last so it starts after the members it uses are constructed.
        std::thread m_worker;
};