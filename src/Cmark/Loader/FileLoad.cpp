#include <CMark.h>
#include "FileLoad.h"

namespace  CM
{
    std::shared_ptr<std::vector<unsigned char>> FileLoad::load(const std::filesystem::path& path)
    {
        std::locale::global(std::locale(""));

#ifdef _WIN32
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;  // NOLINT(clang-diagnostic-deprecated-declarations)
        std::wstring filePath = converter.from_bytes(path.string());
        std::ifstream picture(filePath, std::ios::binary);
#endif

#ifdef __linux__
        std::ifstream picture(path.string(), std::ios::binary);
#endif

#ifdef __APPLE__
        std::ifstream picture(path.string(), std::ios::binary);
#endif 


#ifndef _WIN32
#ifndef __linux__
#ifndef __APPLE__

        std::ifstream picture;
        picture.open(path.string(), std::ios::binary);

#endif
#endif
#endif


        if (!picture.is_open())
        {
            std::cerr << "Could not open file: " << path << std::endl;
            throw std::runtime_error("Error!");
        }

        std::vector<unsigned char> result;
        picture.seekg(0, std::ios::end);
        auto fileSize = picture.tellg();
        picture.seekg(0, std::ios::beg);

        result.clear();
        result.resize(static_cast<size_t>(fileSize));

        picture.read(reinterpret_cast<char *>(result.data()), fileSize);

        picture.close();

        return std::make_shared<std::vector<unsigned char>>(std::move(result));
    }
};
