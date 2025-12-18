#include <cxx-sh/Interpreter/Interpreter.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

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
        sh->writeln("Usage:");
        sh->writeln("\t system <command to be executed in system shell>");
        return 1;
    }
}

int shell::basic::file(CXXSH_COMMAND_ARGS) {
    auto usage = [sh](){
        sh->writeln("Usage:");
        sh->writeln("\t file read <input> [output=stdout]");
        sh->writeln("\t file exec <input>");
    };

    auto open_stream = [sh](cr<line_t> input)->std::ifstream&{
        auto ifs = new std::ifstream();

        if (!std::filesystem::exists(input)) {
            sh->writeln("file not exists: '" + input + "'.");
        }
        else ifs->open(input, std::ios::app);

        return *ifs;
    };

    if (args.size() < 2) {
        usage();
        return 1;
    }

    if (args[0] == "read") {
        line_t s_input = sh->get_cwd() + args[1];
        line_t s_output;

        if (args.size() < 3) s_output = "stdout";
        else s_output = args[2];

        std::ostream* os;
        std::ifstream& ifs = open_stream(s_input);

        if (s_output == "stdout") {
            os = &std::cout;
        } else {
            os = new std::ofstream(s_output);
        }

        try {
            if (std::filesystem::file_size(s_input) == 0)  // empty file
                (*os) << std::flush;
            else  (*os) << ifs.rdbuf() << std::flush;
        } catch (...) { return 3; } // broken ostream
        return 0;
    } else if (args[0] == "exec") {
        auto input = args[1];
        auto& stream = open_stream(input);
        sh->from_stream(stream);
        return 0;
    } else {
        sh->writeln("Unknown option: '" + args[0] + ".");
        usage();
        return -1;
    }
}

auto name_only = [](const std::string& path) {
    if (std::filesystem::is_directory(path))
        return std::filesystem::path(path).parent_path().filename().string() + "/";
    else return std::filesystem::path(path).filename().string();
};

auto ls_dir = [](const std::string& dir){
    std::vector<std::string> res;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        res.push_back(name_only(entry.path()));
    }
    return res;
};

int shell::basic::ls(CXXSH_COMMAND_ARGS) {
    std::string dir;
    if (args.size() == 0)  // current dir (cwd)
        dir = sh->get_cwd();
    else dir = args[0];

    sh->writeln("Directory '" + name_only(dir) + "':");
    for (auto i : ls_dir(dir)) {
        sh->writeln("\t" + i);
    }
    return 0;
}

int shell::basic::cd(CXXSH_COMMAND_ARGS) {
    // TODO: implement this
    return 0;
}