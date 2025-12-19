#include <cxx-sh/Interpreter/Interpreter.hpp>

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

int shell::Interpreter::exec(cr<line_t> line) {
    if (!is_running()) return -1;
    auto prs = parse(line);
    auto it = command_map.find(prs.command());
    if (it != command_map.end()) {
        return it->second(this, prs.args(), prs.flags(), prs.string());
    } else {
        writeln("Command not found: '" + prs.command() + "'.");
        return 127;
    }
}

std::string shell::Interpreter::get_cwd() {
    if (cwd[cwd.size() - 1] != '/') cwd.push_back('/');

    return cwd;
}

void shell::Interpreter::set_cwd(cr<line_t> cwd) {
    this->cwd = cwd;
    get_cwd();
}

std::vector<int> shell::Interpreter::from_stream(std::istream& is) {
    std::vector<int> result_codes;
    
    line_t line;
    while(std::getline(is, line)) result_codes.push_back(exec(line));

    return result_codes;
}