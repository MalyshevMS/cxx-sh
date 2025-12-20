#pragma once

#include <cxx-sh/static/static>
#include <initializer_list>

namespace shell {
    class Pipeline {
    private:
        vec<cmd_t> m_commands;
        vec<line_t> m_lines;
        vec<args_t> m_args;
        vec<flags_t> m_flags;
        vec<char> m_seps;
    public:
        vec<line_t> split(cr<line_t> str, vec<char> seps);
        Pipeline(cr<line_t> line);

        vec<cmd_t> commands();
        vec<line_t> lines();
        vec<args_t> args();
        vec<flags_t> flags();
        vec<char> seps();
    };

    Pipeline parse_multi(cr<line_t> line);
};