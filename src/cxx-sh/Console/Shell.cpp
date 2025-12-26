#include <cxx-sh/Console/Shell.hpp>
#include <cxx-sh/Filesystem/Filesystem.hpp>
#include <signal.h>

shell::Shell::Shell(int argc, char const *argv[]) {
    line_t cwd = fs::current_path() / fs::path();
    owned_sh = std::make_unique<Interpreter>(cwd, std::cout);
    sh = owned_sh.get();

    int code = exec_args(argc, argv);
    if (code == 0) { // normal launch
        return;
    } else if (code == -1) { // exit immedeatly with error
        std::exit(1);
    } else if (code == 1) { // do not launch shell
        skip_shell = true;
    }
}

int shell::Shell::exec_args(int argc, const char **argv) {
    if (argc > 1) {
        vec<line_t> args;
        for (int i = 1; i < argc; i++) args.push_back(argv[i]);
        
        line_t first = args[0];

        if (first[0] == '-') { // flags
            if (first == "--version" || first == "-v") {
                std::cout << "cxxsh " << version << std::endl;
                return 1;
            }
            else if (first == "--exec" || first == "-e") {
                if (args.size() < 2) {
                    std::cerr << "Usage: cxxsh " << first << " command_string" << std::endl;
                    return -1;
                }
                sh->add_queue(args[1]);
                return 0;
            }

            else {
                std::cerr << "Incorrect flag." << std::endl;
                return -1;
            }
        } else {
            sh->add_queue("file exec " + first);
            return 0;
        }
    } else return 0; // no args
}

shell::Shell::Shell(Interpreter& other) {
    sh = &other;
}

shell::Shell::~Shell() noexcept {
    try {
        if (sh && sh->is_running()) {
            send_thread("exit");
        }
        if (thr.joinable()) thr.join();
    } catch (...) {}
}

void shell::Shell::run() {    
    if (skip_shell) return;
    if (!sh->is_running()) sh->run();

    sh->exec_queue();

    line_t line;
    code_t code = "0";
    while (sh->is_running()) {
        sh->write("(" + file::name_only(sh->get_cwd()) + ")[" + code + "]" + invite + " ");

        std::getline(std::cin, line);
        if (!line.empty()) code = sh->run(line);
    }
}

void shell::Shell::run_thread() {
    if (sh->is_running() || thr.joinable()) return;
    thr = std::thread(&Shell::run, this);
}

void shell::Shell::send_thread(cr<line_t> line) {
    if (!sh->is_running()) return;
    
    std::lock_guard lock(mtx);
    send(line);
}

void shell::Shell::stop_thread() {
    if (!sh->is_running()) return;

    send_thread("exit");
    if (thr.joinable()) {
        thr.join();
    }
    std::cout << "Press ENTER to exit shell..." << std::flush;
}

shell::code_t shell::Shell::send(cr<line_t> line) {
    if (!sh->is_running()) return "-3";

    return sh->run(line);
}

void shell::Shell::set_invite(cr<line_t> invite) {
    this->invite = invite;
}