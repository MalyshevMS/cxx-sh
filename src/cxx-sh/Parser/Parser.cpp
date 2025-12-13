#include <cxx-sh/Parser/Parser.hpp>

std::vector<std::string> shell::Parser::split(cr<std::string> str, char sep = ' ') {
    std::vector<std::string> result;
    std::string current;

    for (char c : str) {
        if (c == sep) {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) {
        result.push_back(current);
    }

    return result;
}

shell::Parser::Parser(cr<std::string> line) : line(line) {
    parsed = split(line);
}

std::string shell::Parser::command() const {
    if (parsed.size() > 0) return parsed[0];
    else return "";
}

std::vector<std::string> shell::Parser::args() const {
    if (parsed.size() < 2) return {};

    std::vector<std::string> result;
    for (size_t i = 1; i < parsed.size(); i++) {
        auto& str = parsed[i];
        if (str[0] != '-') { // check if falg
            result.push_back(str);
        }
    }
    
    return result;
}

std::vector<std::string> shell::Parser::flags() const {
    if (parsed.size() < 2) return {};

    std::vector<std::string> result;
    for (size_t i = 1; i < parsed.size(); i++) {
        auto& str = parsed[i];
        if (str[0] == '-') { // check if falg
            result.push_back(str);
        }
    }

    return result;
}