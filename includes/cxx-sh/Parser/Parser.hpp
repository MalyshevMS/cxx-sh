#pragma once

#include <string>
#include <vector>
#include <cxx-sh/static/static>

namespace shell {
    class Parser {
        line_t line;
        vec<line_t> parsed;

    public:
        static vec<line_t> split(cr<line_t> str, char sep = ' ');
        static line_t join(vec<line_t> strs, char sep = ' ');

        Parser(cr<line_t> line);
        
        line_t string() const { return line; };
        line_t other(size_t spaces_count = 1) const; // whole line except the number of spaces given (default 1)
        cmd_t command() const;
        args_t args() const;
        flags_t flags() const;
    };

    static Parser parse(cr<line_t> line) { return Parser(line); }
};