#pragma once

#include <ostream>
#include <istream>
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
        std::string cwd = "~/";
        bool running = false;
    public:
        Interpreter(cr<line_t> cwd, std::ostream& os = std::cout);

        int exec(cr<line_t> line);
        void add_command(cr<cmd_t>, cr<command_f>);
        std::vector<int> from_stream(std::istream& is);
        
        void write(cr<line_t> line = "") { os << line; };
        void writeln(cr<line_t> line = "") { os << line << std::endl; };
        std::ostream& stream() const { return os; };

        std::string get_cwd();

        void run() { running = true; }
        void exit() { running = false; }
        bool is_running() const { return running; }
    };

    // Basic commands
    namespace basic {
        int echo    (CXXSH_COMMAND_ARGS_DEV);
        int clear   (CXXSH_COMMAND_ARGS_DEV);
        int exit    (CXXSH_COMMAND_ARGS_DEV);
        int alias   (CXXSH_COMMAND_ARGS_DEV);
        int system  (CXXSH_COMMAND_ARGS_DEV);
        int file    (CXXSH_COMMAND_ARGS_DEV);
        int cd      (CXXSH_COMMAND_ARGS_DEV);
        int ls      (CXXSH_COMMAND_ARGS_DEV);
    
        inline std::vector<cmd_t> names = {
            "echo",
            "clear",
            "exit",
            "alias",
            "system",
            "file",
            "cd",
            "ls"
        };

        inline std::vector<command_f> functions = {
            echo,
            clear,
            exit,
            alias,
            system,
            file,
            cd,
            ls
        };
    };
};