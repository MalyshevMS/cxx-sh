#include <cxx-sh/Console/Shell.hpp>
#include <cxx-sh/Filesystem/Filesystem.hpp>

shell::Shell::Shell(cr<line_t> cwd) {
    owned_sh = std::make_unique<Interpreter>(cwd, std::cout);
    sh = owned_sh.get();
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
    if (!sh->is_running()) sh->run();

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