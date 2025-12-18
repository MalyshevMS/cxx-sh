#include <cxx-sh/Console/Shell.hpp>

shell::Shell::Shell(cr<line_t> cwd) {
    sh = new Interpreter(cwd, std::cout);
}

shell::Shell::Shell(Interpreter& other) {
    sh = &other;
}

void shell::Shell::run() {
    if (sh->is_running()) {
        sh->writeln("Aborted attempt to run shell twice.");
        return;
    }
    
    line_t line;
    int code = 0;
    sh->run();
    while (sh->is_running()) {
        sh->write("(" + sh->get_cwd() + ")[" + std::to_string(code) + "]" + invite + " ");

        std::getline(std::cin, line);
        if (!line.empty()) code = sh->exec(line);
    }
}

void shell::Shell::run_thread() {
    if (sh->is_running()) return;
    
    thr = std::thread(&Shell::run, this);
    thr.detach();
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

int shell::Shell::send(cr<line_t> line) {
    if (!sh->is_running()) return -3;

    return sh->exec(line);
}

void shell::Shell::set_invite(cr<line_t> invite) {
    this->invite = invite;
}