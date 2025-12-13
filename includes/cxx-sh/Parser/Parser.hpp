#pragma once

#include <string>
#include <vector>
#include <cxx-sh/static/static>

namespace shell {
    using args_t = std::vector<std::string>;
    using flags_t = std::vector<std::string>;
    using cmd_t = std::string;
    using line_t = std::string;

    class Parser {
        line_t line;
        std::vector<std::string> parsed;

    public:
        static std::vector<std::string> split(cr<line_t> str, char sep = ' ');
        static line_t join(std::vector<std::string> strs, char sep = ' ');

        Parser(cr<line_t> line);
        
        line_t string() const { return line; };
        line_t other(size_t spaces_count = 1) const; // whole line except the number of spaces given (default 1)
        cmd_t command() const;
        args_t args() const;
        flags_t flags() const;
    };

    static Parser parse(cr<line_t> line) { return Parser(line); }
};