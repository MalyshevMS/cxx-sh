#include <cxx-sh/Parser/Pipeline.hpp>
#include <cxx-sh/Parser/Parser.hpp>
#include <vector>

shell::vec<shell::line_t> shell::Pipeline::split(cr<line_t> str, vec<char> seps) {
    vec<line_t> result;
    line_t current;
    bool added = false;

    for (char c : str) {
        if (c == '#') break; // comment
        for (char s : seps) {
            if (c == s) {
                if (!current.empty())
                result.push_back(current);
                current.clear();
                added = true;
                break;
            }
        }
        if (!added) {
            current += c;
        }
        added = false;
    }

    if (!current.empty()) {
        result.push_back(current);
    }

    return result;
}

shell::Pipeline::Pipeline(cr<line_t> line) {
    m_lines = split(line, { ';', '&', '|' });
    if (m_lines.size() == 0) { // comment line
        comment = true;
        m_commands = {};
        m_args = {};
        m_flags = {};
        m_seps = {};
        return;
    }
    
    for (auto l : m_lines) {
        auto p = parse(l);

        m_commands.push_back(p.command());
        m_args.push_back(p.args());
        m_flags.push_back(p.flags());
    }

    for (auto c : line) {
        if (c == ';' || c == '&' || c == '|') {
            m_seps.push_back(c);
        }
    }
}

shell::vec<shell::cmd_t> shell::Pipeline::commands() {
    return m_commands;
}

shell::vec<shell::line_t> shell::Pipeline::lines() {
    return m_lines;
}

shell::vec<shell::args_t> shell::Pipeline::args() {
    return m_args;
}

shell::vec<shell::flags_t> shell::Pipeline::flags() {
    return m_flags;
}

shell::vec<char> shell::Pipeline::seps() {
    return m_seps;
}

shell::Pipeline shell::parse_multi(cr<line_t> line) {
    return Pipeline(line);
}