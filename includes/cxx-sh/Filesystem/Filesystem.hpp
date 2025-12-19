#pragma once

#include <filesystem>
#include <vector>
#include <cxx-sh/static/static>

namespace shell {
    namespace fs = std::filesystem;
    namespace file {

        vec<line_t> list_directory (cr<fs::path> dir);
        line_t name_only(cr<fs::path> p);
        bool is_dir(cr<fs::path> p);
        bool is_subdir(cr<fs::path> dir, cr<fs::path> subdir);
        fs::path expand(cr<line_t> p, cr<line_t> cwd);
    };
};