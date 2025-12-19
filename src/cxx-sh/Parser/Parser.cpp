#include <cxx-sh/Parser/Parser.hpp>
#include <sstream>

std::vector<std::string> shell::Parser::split(cr<line_t> str, char sep) {
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

shell::Parser::Parser(cr<shell::line_t> line) : line(line) {
    parsed = split(line);
}

shell::cmd_t shell::Parser::command() const {
    if (parsed.size() > 0) return parsed[0];
    else return "";
}

shell::args_t shell::Parser::args() const {
    if (parsed.size() < 2) return {};

    shell::args_t result;
    for (size_t i = 1; i < parsed.size(); i++) {
        auto& str = parsed[i];
        if (str[0] != '-') { // check if falg
            result.push_back(str);
        }
    }
    
    return result;
}

shell::flags_t shell::Parser::flags() const {
    if (parsed.size() < 2) return {};

    shell::flags_t result;
    for (size_t i = 1; i < parsed.size(); i++) {
        auto& str = parsed[i];
        if (str[0] == '-') { // check if falg
            result.push_back(str);
        }
    }

    return result;
}

shell::line_t shell::Parser::other(size_t spaces_count) const {
    std::string result;
    int space_cnt = 0;
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ' ') {
            space_cnt++;
            if (space_cnt == spaces_count) {
                result = line.substr(++i);
                return result;
            }
        }
    }
    return line;
}

shell::line_t shell::Parser::join(std::vector<std::string> strs, char sep) {
    std::stringstream ss;
    if (strs.empty()) return std::string();
    ss << strs[0];
    for (size_t i = 1; i < strs.size(); ++i) ss << sep << strs[i];
    return ss.str();
}