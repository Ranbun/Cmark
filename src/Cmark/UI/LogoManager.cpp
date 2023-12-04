#include "LogoManager.h"

namespace CM
{
    std::map<std::string,CameraIndex> cameraMakerMap{{"nikon",CameraIndex::Nikon}};
    std::unordered_map<CameraIndex,std::shared_ptr<QPixmap>>  LogoManager::m_logos{};

    CameraIndex LogoManager::resolverCameraIndex(const std::string &cameraMake)
    {
        if(cameraMake.empty())
        {
            throw std::runtime_error("Camera Make empty!");
        }

        QString Make = cameraMake.c_str();

        auto lists = Make.split(" ");
        auto maker = lists[0].toLower().toStdString();

        auto res = cameraMakerMap.find(maker);
        if(res != cameraMakerMap.end())
        {
            return res->second;
        }
        throw std::runtime_error("Can't found Camera Maker!");
    }

    void LogoManager::loadCameraLogo(const CameraIndex &cameraIndex)
    {
        auto loadLogo = [](CameraIndex index, const std::string & path)->std::pair<CameraIndex,std::shared_ptr<QPixmap>>
        {
            auto logo = std::make_shared<QPixmap>(path.c_str());
            return {index,logo};
        };

        switch (cameraIndex)
        {
            case CameraIndex::Nikon:
                m_logos.insert(loadLogo(CameraIndex::Nikon,"./sources/logos/nikon.png"));
                break;
            case CameraIndex::Sony:
                m_logos.insert(loadLogo(CameraIndex::Sony,"./sources/logos/sony.png"));
                break;
            case CameraIndex::Canon:
                m_logos.insert(loadLogo(CameraIndex::Canon,"./sources/logos/canon.png"));
                break;
            default:
                throw std::runtime_error("load error!");
                break;
        }
    }

    std::shared_ptr<QPixmap> LogoManager::getCameraMakerLogo(const CameraIndex &cameraIndex)
    {
        auto res = m_logos.find(cameraIndex);
        if(res != m_logos.end())
        {
            return res->second;
        }

        throw std::runtime_error("Can't found Camera Maker Logo!");
    }
} // CM

