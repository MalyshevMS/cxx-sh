#include <cxx-sh/Interpreter/Interpreter.hpp>
#include <cxx-sh/Parser/Pipeline.hpp>

shell::Interpreter::Interpreter(cr<line_t> exePath, std::ostream& os) : os(os) {
    // Apply basic commands
    for (int i = 0; i < basic::functions.size(); i++) {
        command_map.emplace(basic::names[i], basic::functions[i]);
    }

    this->cwd = exePath.substr(0, exePath.find_last_of("/\\") + 1);
    get_cwd();
}

void shell::Interpreter::add_command(cr<cmd_t> cmd, cr<command_f> function)
{
    command_map[cmd] = function;
}

shell::code_t shell::Interpreter::exec(cr<line_t> line) {
    if (!is_running()) return "-1";
    auto prs = parse(line);
    auto it = command_map.find(prs.command());
    if (it != command_map.end()) {
        return it->second(this, prs.args(), prs.flags(), prs.string());
    } else {
        writeln("Command not found: '" + prs.command() + "'.");
        return "127";
    }
}

shell::code_t shell::Interpreter::run(cr<line_t> line) {
    auto p = parse_multi(line);

    if (p.is_comment()) return "#";

    vec<code_t> codes(p.commands().size());

    for (size_t i = 0; i < codes.size(); i++) {
        codes[i] = exec(p.lines()[i]);
    }


    code_t result = "0";
    if (codes.size() > 0) result = codes[0];
    for (size_t i = 1; i < codes.size(); i++) {
        result += p.seps()[i - 1] + codes[i];
    }

    return result;
}

shell::line_t shell::Interpreter::get_cwd() {
    if (cwd[cwd.size() - 1] != '/') cwd.push_back('/');

    return cwd;
}

void shell::Interpreter::set_cwd(cr<line_t> cwd) {
    this->cwd = cwd;
    get_cwd();
}

shell::vec<shell::code_t> shell::Interpreter::from_stream(std::istream& is) {
    vec<code_t> result_codes;
    
    line_t line;
    while(std::getline(is, line)) result_codes.push_back(run(line));

    return result_codes;
}

void shell::Interpreter::add_queue(cr<line_t> line) {
    queue.push(line);
}

shell::vec<shell::code_t> shell::Interpreter::exec_queue() {
    vec<code_t> res;
    while (queue.size() > 0) {
        auto code = run(queue.front());
        res.push_back(code);
        queue.pop();
    }
    return res;
}