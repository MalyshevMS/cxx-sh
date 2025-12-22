#include <cxx-sh/Interpreter/Interpreter.hpp>
#include <cxx-sh/Filesystem/Filesystem.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <cstdlib>

shell::code_t shell::basic::echo(CXXSH_COMMAND_ARGS_DEV) {
    sh->stream() << std::flush;
    for (auto a : args) {
        sh->write(a + ' ');
    }
    sh->writeln();

    return "0";
}

shell::code_t shell::basic::exit(CXXSH_COMMAND_ARGS_DEV) {
    sh->writeln("Exiting interpreter...");
    sh->exit();
    return "0";
}

shell::code_t shell::basic::clear(CXXSH_COMMAND_ARGS_DEV) {
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
        return "1";
    }
    os.clear();
    return "0";
}

shell::code_t shell::basic::alias(CXXSH_COMMAND_ARGS_DEV) {
    std::string alias_cmd = parse(line).other(2);
    std::string alias_name = args[0];
    
    sh->add_command(alias_name, [alias_cmd](CXXSH_COMMAND_ARGS_DEV){
        if (args.size() > 0)
        return sh->run(alias_cmd + " " + Parser::join(args));
        else
        return sh->run(alias_cmd);
    });
    
    sh->writeln("Aliased '" + alias_name + "' to '" + alias_cmd + "'.");
    return "0";
}

shell::code_t shell::basic::system(CXXSH_COMMAND_ARGS_DEV) {
    if (args.size() > 0)
    return std::to_string(std::system(parse(line).other().c_str()));
    else {
        sh->writeln("Usage:");
        sh->writeln("\t system <command to be executed in system shell>");
        return "1";
    }
}

shell::code_t shell::basic::file(CXXSH_COMMAND_ARGS) {
    auto usage = [sh](){
        sh->writeln("Usage:");
        sh->writeln("\t file read <input> [output=stdout]");
        sh->writeln("\t file exec <input>");
    };

    auto open_istream = [sh](cr<line_t> input) {
        std::ifstream ifs(input, std::ios::in);
        if (!ifs.is_open()) {
            sh->writeln("File not exists: '" + input + "'.");
        }
        return ifs;
    };

    auto open_ostream = [sh](cr<line_t> output) {
        std::ofstream ofs(output, std::ios::out);
        if (!ofs.is_open()) {
            sh->writeln("File not exists: '" + output + "'.");
        }
        return ofs;
    };

    if (args.size() < 2) {
        usage();
        return "1";
    }

    if (args[0] == "read") {
        line_t s_input = file::expand(sh->get_cwd(), args[1]);
        line_t s_output = "stdout";

        if (args.size() > 2) s_output = file::expand(sh->get_cwd(), args[2]);

        std::istream* isp = nullptr;
        std::ostream* osp = nullptr;

        if (!file::is_file(s_input)) {
            sh->writeln("The path '" + s_input + "' is not file.");
            return "1";
        }
        if (!file::is_file(s_output) && s_output != "stdout") {
            sh->writeln("The path '" + s_output + "' is not file.");
            return "1";
        }

        isp = new std::ifstream(open_istream(s_input));

        if (s_output == "stdout") osp = &std::cout;
        else osp = new std::ofstream(open_ostream(s_output));

        (*osp) << isp->rdbuf() << std::endl;

        return "0";
    } else if (args[0] == "exec") {
        auto input = file::expand(sh->get_cwd(), args[1]);

        std::ifstream stream = open_istream(input);
        if (!stream.is_open()) return "2";
        
        auto codes = sh->from_stream(stream);
        code_t res = codes[0];

        for(int i = 1; i < codes.size(); i++) {
            res += ';' + codes[i];
        }

        return res;
    } else {
        sh->writeln("Unknown option: '" + args[0] + "'.");
        usage();
        return "-1";
    }
}

shell::code_t shell::basic::ls(CXXSH_COMMAND_ARGS) {
    fs::path target;
    if (args.size() == 0) target = file::expand(sh->get_cwd(), "");
    else target = file::expand(sh->get_cwd(), args[0]);

    if (!fs::exists(target)) {
        sh->writeln("No such file or directory: '" + (args.size() ? args[0] : target.string()) + "'.");
        return "2";
    }

    if (!file::is_dir(target)) {
        sh->writeln("Not a directory: '" + (args.size() ? args[0] : target.string()) + "'.");
        return "3";
    }

    sh->writeln("Directory '" + file::name_only(target) + "':");
    for (auto &i : file::list_directory(target)) {
        sh->write(i + " ");
    }
    sh->writeln();
    return "0";
}

shell::code_t shell::basic::cd(CXXSH_COMMAND_ARGS) {
    if (args.size() == 0) {
        // no arg: go to HOME if available
        const char* home = std::getenv("HOME");
        if (home) { sh->set_cwd(std::string(home)); return "0"; }
        sh->writeln("No path specified and $HOME not set.");
        return "1";
    }

    fs::path newpath = file::expand(sh->get_cwd(), args[0]);
    try {
        newpath = fs::weakly_canonical(newpath);
    } catch (...) {}

    if (!fs::exists(newpath) || !fs::is_directory(newpath)) {
        sh->writeln("No such file or directory: '" + args[0] + "'.");
        return "2";
    }

    sh->set_cwd(newpath.string());
    return "0";
}