#pragma once
#include <string>
#include <vector>

namespace UCG {
    class FileUtils {
    public:
        static std::string Path(const std::string& filename);
        static std::vector<char> Read(const std::string& filename);
        static std::vector<std::string> List(const std::string& dirname = ".");
    };
}