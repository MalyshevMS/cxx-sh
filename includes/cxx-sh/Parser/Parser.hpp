#pragma once

#include <string>
#include <vector>
#include <cxx-sh/static/static>

namespace shell {
    class Parser {
        std::string line;
        std::vector<std::string> parsed;

        static std::vector<std::string> split(cr<std::string> str, char sep);
    public:
        Parser(cr<std::string> line);
        
        std::string string() const { return line; };
        std::string command() const;
        std::vector<std::string> args() const;
        std::vector<std::string> flags() const;
    };

    static Parser parse(cr<std::string> line) { return Parser(line); }
};