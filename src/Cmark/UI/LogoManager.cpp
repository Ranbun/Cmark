#include "LogoManager.h"

#include <QMessageBox>

namespace CM
{
    std::unordered_map<CameraIndex,std::shared_ptr<QPixmap>>  LogoManager::m_logos{};
    std::unordered_map<std::string,CameraIndex> LogoManager::cameraMakerMap;
    std::once_flag LogoManager::m_initFlag;

    CameraIndex LogoManager::resolverCameraIndex(const std::string &cameraMake)
    {
        if(cameraMake.empty())
        {
            ///TODO： 考虑使用别的信息替代
            throw std::runtime_error("Camera Make empty!");
        }

        QString Make = cameraMake.c_str();

        /// TODO : 需要更兼容的获取camera maker的方法

        auto lists = Make.split(" ");
        auto maker = lists[0].toLower().toStdString();

        auto res = cameraMakerMap.find(maker);
        if(res != cameraMakerMap.end())
        {
            return res->second;
        }

        QMessageBox::about(nullptr,"Warning",("We Don't supported: " + maker).c_str());
        throw std::runtime_error("Can't found Camera Maker!");
    }

    void LogoManager::loadCameraLogo(const CameraIndex &cameraIndex)
    {
        auto loadLogo = [](CameraIndex index, const std::string & path)->std::pair<CameraIndex,std::shared_ptr<QPixmap>>
        {
            /// TODO: maybe use QImageReader get pixmap
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
            case CameraIndex::Panasonic:
                m_logos.insert(loadLogo(CameraIndex::Panasonic,"./sources/logos/panasonic.png"));
                break;
            case CameraIndex::Leica:
                m_logos.insert(loadLogo(CameraIndex::Leica,"./sources/logos/leica_logo.png"));
                break;
            case CameraIndex::Hasselblad:
                m_logos.insert(loadLogo(CameraIndex::Hasselblad,"./sources/logos/hasselblad.png"));
                break;
            case CameraIndex::Apple:
                m_logos.insert(loadLogo(CameraIndex::Apple,"./sources/logos/apple.png"));
                break;
            case CameraIndex::Fujifilm:
                m_logos.insert(loadLogo(CameraIndex::Fujifilm,"./sources/logos/fujifilm.png"));
                break;
            /// TODO: need add others......
            default:
                throw std::runtime_error("Can't support current camera, add logo");
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

    void LogoManager::Init()
    {
        std::call_once(m_initFlag,[]()
        {
            cameraMakerMap.insert({"nikon",CameraIndex::Nikon});
            cameraMakerMap.insert({"sony",CameraIndex::Sony});
            cameraMakerMap.insert({"canon",CameraIndex::Canon});
            cameraMakerMap.insert({"panasonic",CameraIndex::Panasonic});
            cameraMakerMap.insert({"hassel",CameraIndex::Hasselblad});
            cameraMakerMap.insert({"leica",CameraIndex::Leica});
            cameraMakerMap.insert({"fujifilm",CameraIndex::Fujifilm});

        });

    }
} // CM

