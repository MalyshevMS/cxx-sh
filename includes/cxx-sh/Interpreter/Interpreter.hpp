#pragma once

#include <ostream>
#include <unordered_map>
#include <functional>
#include <cxx-sh/Parser/Parser.hpp>

#define CXXSH_COMMAND_ARGS class Interpreter* sh, cr<args_t> args, cr<flags_t> flags, cr<line_t> line

namespace shell {
    using command_f = std::function<int (CXXSH_COMMAND_ARGS)>;
    
    class Interpreter {
    private:
        std::ostream& os;
        std::unordered_map<cmd_t, command_f> command_map;
    public:
        Interpreter(std::ostream& os);

        int exec(cr<line_t> line);
        void add_command(cr<cmd_t>, cr<command_f>);
        void write(cr<line_t> line = "") { os << line; };
        void writeln(cr<line_t> line = "") { os << line << std::endl; };
        std::ostream& stream() { return os; };
        void exit() { std::exit(0); }
    };

    // Basic commands
    namespace basic {
        int echo    (CXXSH_COMMAND_ARGS);
        int clear   (CXXSH_COMMAND_ARGS);
        int exit    (CXXSH_COMMAND_ARGS);
        int alias   (CXXSH_COMMAND_ARGS);
    
        inline std::vector<cmd_t> names = {
            "echo",
            "clear",
            "exit",
            "alias"
        };

        inline std::vector<command_f> functions = {
            echo,
            clear,
            exit,
            alias
        };
    };
};