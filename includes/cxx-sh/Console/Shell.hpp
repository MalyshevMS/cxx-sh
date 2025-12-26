#pragma once

#include <cxx-sh/Interpreter/Interpreter.hpp>
#include <thread>
#include <mutex>
#include <iostream>
#include <memory>

namespace shell {
    class Shell {
    private:
        std::unique_ptr<Interpreter> owned_sh;
        Interpreter* sh = nullptr;
        std::thread thr;
        std::mutex mtx;
        line_t invite = ">";
        bool skip_shell = false;

        int exec_args(int argc, const char** argv);
    public:
        Shell(const Shell&) = delete;
        Shell& operator=(const Shell&) = delete;
        Shell(Shell&&) = delete;
        Shell& operator=(Shell&&) = delete;

        ~Shell() noexcept;
        Shell(int argc, char const *argv[]);
        Shell(Interpreter& other);
        
        void run();
        void set_invite(cr<line_t> invite);
        code_t send(cr<line_t> line);

        void run_thread();
        void send_thread(cr<line_t> line);
        void stop_thread();
    };
}