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

        static std::vector<std::string> split(cr<line_t> str, char sep);
    public:
        Parser(cr<line_t> line);
        
        line_t string() const { return line; };
        cmd_t command() const;
        args_t args() const;
        flags_t flags() const;
    };

    static Parser parse(cr<line_t> line) { return Parser(line); }
};