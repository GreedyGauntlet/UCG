#include "FileUtils.h"
#include "Flora.h"
#include <fstream>
#include <filesystem>

#ifdef BAZEL_BUILD
    #define SANDBOX "../../../../../" // note: doing this because bazel sandbox is quite a few layers deep
#else
    #define SANDBOX ""
#endif

namespace UCG {

    std::string FileUtils::Path(const std::string& filepath) {
        return SANDBOX + filepath;
    }

    std::vector<char> FileUtils::Read(const std::string& filename) {
        std::ifstream file(Path(filename), std::ios::ate | std::ios::binary);
        if (!file.is_open())
            FL_ERROR("Failed to open file \"" + filename + "\"");
        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }

    std::vector<std::string> FileUtils::List(const std::string& dirname) {
        std::vector<std::string> contents;
        try {
            for (const auto& entry : std::filesystem::directory_iterator((Path(dirname)).c_str()))
                contents.push_back(entry.path().filename());
        } catch (const std::filesystem::filesystem_error& ex) {
            FL_ERROR("Failed to open directory \"" + dirname + "\"");
        }
        return contents;
    }

}