#pragma once

#include <ostream>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <cxx-sh/Parser/Parser.hpp>

#define CXXSH_COMMAND_ARGS_DEV class Interpreter* sh, cr<args_t> args, cr<flags_t> flags, cr<line_t> line
#define CXXSH_COMMAND_ARGS shell::Interpreter* sh, shell::cr<shell::args_t> args, shell::cr<shell::flags_t> flags, shell::cr<shell::line_t> line

namespace shell {
    using command_f = std::function<int (CXXSH_COMMAND_ARGS_DEV)>;
    
    class Interpreter {
    private:
        std::ostream& os;
        std::unordered_map<cmd_t, command_f> command_map;
        bool running = false;
    public:
        Interpreter(std::ostream& os = std::cout);

        int exec(cr<line_t> line);
        void add_command(cr<cmd_t>, cr<command_f>);
        
        void write(cr<line_t> line = "") { os << line; };
        void writeln(cr<line_t> line = "") { os << line << std::endl; };
        std::ostream& stream() { return os; };

        void run() { running = true; }
        void exit() { running = false; }
        bool is_running() { return running; }
    };

    // Basic commands
    namespace basic {
        int echo    (CXXSH_COMMAND_ARGS_DEV);
        int clear   (CXXSH_COMMAND_ARGS_DEV);
        int exit    (CXXSH_COMMAND_ARGS_DEV);
        int alias   (CXXSH_COMMAND_ARGS_DEV);
        int system  (CXXSH_COMMAND_ARGS_DEV);
    
        inline std::vector<cmd_t> names = {
            "echo",
            "clear",
            "exit",
            "alias",
            "system"
        };

        inline std::vector<command_f> functions = {
            echo,
            clear,
            exit,
            alias,
            system
        };
    };
};