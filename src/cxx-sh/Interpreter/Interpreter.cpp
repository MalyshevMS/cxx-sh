#include <cxx-sh/Interpreter/Interpreter.hpp>

shell::Interpreter::Interpreter(std::ostream& os) : os(os) {
    // Apply basic commands
    for (int i = 0; i < basic::functions.size(); i++) {
        command_map.emplace(basic::names[i], basic::functions[i]);
    }
}

void shell::Interpreter::add_command(cr<cmd_t> cmd, cr<command_f> function) {
    command_map.emplace(cmd, function);
} 

int shell::Interpreter::exec(cr<line_t> line) {
    auto prs = parse(line);
    auto it = command_map.find(prs.command());
    if (it != command_map.end()) {
        return it->second(this, prs.args(), prs.flags(), prs.string());
    } else {
        writeln("Command not found: '" + line + "'.");
        return 127;
    }
}