#include <cxx-sh/Interpreter/Interpreter.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

int shell::basic::echo(CXXSH_COMMAND_ARGS_DEV) {
    sh->stream() << std::flush;
    for (auto a : args) {
        sh->write(a + ' ');
    }
    sh->writeln();

    return 0;
}

int shell::basic::exit(CXXSH_COMMAND_ARGS_DEV) {
    sh->writeln("Exiting interpreter...");
    sh->exit();
    return 0;
}

int shell::basic::clear(CXXSH_COMMAND_ARGS_DEV) {
    auto& os = sh->stream();
    auto cls = [](){
        #ifdef _WIN32
            std::system("CLS");
        #else
            std::system("clear");
        #endif
    };
    
    if (auto oss = dynamic_cast<std::ostringstream*>(&os)) {
        oss->str("");
    } else if (&os == &std::cout) {
        os << "\033[2J\033[H"; // set cursor pos to 0
        cls();
    } else if (&os == &std::cerr) {
        os << "\033[2J\033[H";
        cls();
    } else {
        sh->writeln("Unknown or unsupported stream type.");
        os.clear();
        return 1;
    }
    os.clear();
    return 0;
}

int shell::basic::alias(CXXSH_COMMAND_ARGS_DEV) {
    std::string alias_cmd = parse(line).other(2);
    std::string alias_name = args[0];
    
    sh->add_command(alias_name, [alias_cmd](CXXSH_COMMAND_ARGS_DEV){
        if (args.size() > 0)
        return sh->exec(alias_cmd + " " + Parser::join(args));
        else
        return sh->exec(alias_cmd);
    });
    
    sh->writeln("Aliased '" + alias_name + "' to '" + alias_cmd + "'.");
    return 0;
}

int shell::basic::system(CXXSH_COMMAND_ARGS_DEV) {
    if (args.size() > 0)
    return std::system(parse(line).other().c_str());
    else {
        sh->writeln("Too few arguments.");
        return 1;
    }
}