#include <cxx-sh/Filesystem/Filesystem.hpp>
#include <algorithm>

shell::vec<shell::line_t> shell::file::list_directory(cr<fs::path> dir) {
    vec<line_t> res;
    try {
        if (!fs::exists(dir) || !fs::is_directory(dir)) return res;
        res.push_back(".");
        res.push_back("..");
        for (const auto& entry : fs::directory_iterator(dir)) {
            auto name = entry.path().filename().string();
            if (name.empty()) name = entry.path().string();
            if (fs::is_directory(entry.path())) name += '/';
            res.push_back(name);
        }
    } catch (...) {}
    return res;
}

shell::line_t shell::file::name_only(cr<fs::path> p) {
    auto fn = p.filename().string();
    if (!fn.empty()) return fn;
    // root or path ends with separator -> return string form without trailing '/'
    auto s = p.string();
    if (!s.empty() && (s.back() == '/' || s.back() == '\\')) s.pop_back();
    // if still empty, return original string
    if (s.empty()) return p.string();
    return fs::path(s).filename().string();
}

bool shell::file::is_dir(cr<fs::path> p) {
    return fs::is_directory(p);
}

bool shell::file::is_subdir(cr<fs::path> dir, cr<fs::path> subdir) {
    try {
        if (!fs::exists(dir) || !fs::is_directory(dir)) return false;
        for (const auto& entry : fs::directory_iterator(dir)) {
            if (entry.path().filename() == subdir) return true;
            if (entry.path() == subdir) return true;
        }
    } catch (...) {}
    return false;
}

shell::fs::path shell::file::expand(cr<line_t> cwd, cr<line_t> p) {
    if (p.empty()) return fs::path(cwd);
    if (p[0] == '~') {
        const char* home = std::getenv("HOME");
        if (!home) home = "";
        if (p.size() == 1) return fs::path(home);
        if (p[1] == '/' || p[1] == '\\') return fs::path(home) / p.substr(2);
    }
    fs::path pp(p);
    if (pp.is_absolute()) return pp;
    return fs::path(cwd) / pp;
}