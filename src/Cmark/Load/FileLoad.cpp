#include "FileLoad.h"
#include <fstream>

std::vector<unsigned char> CM::FileLoad::Load(const std::string &path)
{
    std::ifstream picture(path,std::ios::binary);
    if(!picture.is_open())
    {
        std::cerr << "Could not open file: " << path << std::endl;
        throw std::runtime_error("Error!");
    }

    std::vector<unsigned char> result;
    picture.seekg(0,std::ios::end);
    auto fileSize = picture.tellg();
    picture.seekg(0,std::ios::beg);

    result.clear();
    result.resize(static_cast<size_t>(fileSize));

    picture.read(reinterpret_cast<char *>(result.data()), fileSize);

    picture.close();

    return std::move(result);
}
