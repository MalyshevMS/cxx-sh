#pragma once

#include <ostream>
#include <istream>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <queue>
#include <cxx-sh/Parser/Parser.hpp>

#define CXXSH_COMMAND_ARGS_DEV class Interpreter* sh, cr<args_t> args, cr<flags_t> flags, cr<line_t> line
#define CXXSH_COMMAND_ARGS shell::Interpreter* sh, shell::cr<shell::args_t> args, shell::cr<shell::flags_t> flags, shell::cr<shell::line_t> line

namespace shell {
    using command_f = std::function<code_t (CXXSH_COMMAND_ARGS_DEV)>;
    
    class Interpreter {
    private:
        std::ostream& os;
        std::unordered_map<cmd_t, command_f> command_map;
        line_t cwd = "~/";
        std::queue<line_t> queue;
        vec<line_t> command_history;
        bool running = false;
    public:
        Interpreter(cr<line_t> cwd, std::ostream& os = std::cout);

        code_t run(cr<line_t> line);
        void add_queue(cr<line_t> line);
        vec<code_t> exec_queue();
        code_t exec(cr<line_t> line);
        void add_command(cr<cmd_t>, cr<command_f>);
        vec<code_t> from_stream(std::istream& is);

        void add_history(cr<line_t> line) { command_history.push_back(line); }
        vec<line_t> get_history() { return command_history; }
        
        void write(cr<line_t> line = "") { os << line; };
        void writeln(cr<line_t> line = "") { os << line << std::endl; };
        std::ostream& stream() const { return os; };

        line_t get_cwd();
        void set_cwd(cr<line_t> cwd);

        void run() { running = true; }
        void exit() { running = false; }
        bool is_running() const { return running; }
    };

    // Basic commands
    namespace basic {
        code_t echo    (CXXSH_COMMAND_ARGS_DEV);
        code_t clear   (CXXSH_COMMAND_ARGS_DEV);
        code_t exit    (CXXSH_COMMAND_ARGS_DEV);
        code_t alias   (CXXSH_COMMAND_ARGS_DEV);
        code_t system  (CXXSH_COMMAND_ARGS_DEV);
        code_t file    (CXXSH_COMMAND_ARGS_DEV);
        code_t cd      (CXXSH_COMMAND_ARGS_DEV);
        code_t ls      (CXXSH_COMMAND_ARGS_DEV);
        code_t history (CXXSH_COMMAND_ARGS_DEV);
    
        inline vec<cmd_t> names = {
            "echo",
            "clear",
            "exit",
            "alias",
            "system",
            "file",
            "cd",
            "ls",
            "history"
        };

        inline vec<command_f> functions = {
            echo,
            clear,
            exit,
            alias,
            system,
            file,
            cd,
            ls,
            history
        };
    };
};