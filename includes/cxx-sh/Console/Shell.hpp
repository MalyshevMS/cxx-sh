#pragma once

#include <cxx-sh/Interpreter/Interpreter.hpp>
#include <thread>
#include <mutex>
#include <iostream>

namespace shell {
    class Shell {
    private:
        Interpreter* sh = nullptr;
        std::thread thr;
        std::mutex mtx;
        line_t invite = ">";
    public:
        Shell();
        Shell(Interpreter& other);
        void run();
        void set_invite(cr<line_t> invite);
        int send(cr<line_t> line);

        void run_thread();
        void send_thread(cr<line_t> line);
        void stop_thread();
    };
}