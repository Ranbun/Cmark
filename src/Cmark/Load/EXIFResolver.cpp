#include "EXIFResolver.h"

namespace CM
{
    [[maybe_unused]] int EXIFResolver::resolver(const std::vector<unsigned char> &pictureData)
    {
        return m_EXIFResolver.parseFrom(pictureData.data(), pictureData.size());
    }
} // CM