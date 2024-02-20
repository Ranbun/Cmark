#include "CMark.h"

#include "LogoManager.h"

#include <QMessageBox>
#include <QPixmap>

namespace CM
{
    std::unordered_map<CameraIndex, std::shared_ptr<QPixmap>> LogoManager::m_Logos{};
    std::unordered_map<std::string, CameraIndex> LogoManager::m_CameraMakerMap;
    std::once_flag LogoManager::m_InitFlag;

    CameraIndex LogoManager::resolverCameraIndex(const std::string& cameraMake)
    {
        QString Make = cameraMake.c_str();
        if (Make.isEmpty())
        {
            Make = QString("empty");
        }

        /// TODO : 需要更兼容的获取camera maker的方法

        auto lists = Make.split(" ");
        const auto maker = lists[0].toLower().toStdString();

        const auto res = m_CameraMakerMap.find(maker);
        if (res != m_CameraMakerMap.end())
        {
            return res->second;
        }

        QMessageBox::about(nullptr, "Warning", ("We Don't supported: " + maker).c_str());
        throw std::runtime_error("Can't found Camera Maker!");
    }

    void LogoManager::loadCameraLogo(const CameraIndex& cameraIndex)
    {
        auto loadLogo = [](CameraIndex index,
                           const std::string& path)-> std::pair<CameraIndex, std::shared_ptr<QPixmap>>
        {
            /// TODO: maybe use QImageReader get pixmap
            auto logo = std::make_shared<QPixmap>(path.c_str());
            return {index, logo};
        };

        if (m_Logos.count(cameraIndex)) ///< the logo loaded
        {
            return;
        }

        switch (cameraIndex)
        {
        case CameraIndex::Nikon:
            m_Logos.insert(loadLogo(CameraIndex::Nikon, "./sources/logos/nikon.png"));
            break;
        case CameraIndex::Sony:
            m_Logos.insert(loadLogo(CameraIndex::Sony, "./sources/logos/sony.png"));
            break;
        case CameraIndex::Canon:
            m_Logos.insert(loadLogo(CameraIndex::Canon, "./sources/logos/canon.png"));
            break;
        case CameraIndex::Panasonic:
            m_Logos.insert(loadLogo(CameraIndex::Panasonic, "./sources/logos/panasonic.png"));
            break;
        case CameraIndex::Leica:
            m_Logos.insert(loadLogo(CameraIndex::Leica, "./sources/logos/leica_logo.png"));
            break;
        case CameraIndex::Hasselblad:
            m_Logos.insert(loadLogo(CameraIndex::Hasselblad, "./sources/logos/hasselblad.png"));
            break;
        case CameraIndex::Apple:
            m_Logos.insert(loadLogo(CameraIndex::Apple, "./sources/logos/apple.png"));
            break;
        case CameraIndex::Fujifilm:
            m_Logos.insert(loadLogo(CameraIndex::Fujifilm, "./sources/logos/fujifilm.png"));
            break;
        case CameraIndex::None:
            {
                auto logo = std::make_shared<QPixmap>(64, 64);
                logo->fill(Qt::transparent);
                m_Logos.insert({CameraIndex::None, logo});
            }
            break;
        /// TODO: need add others......
        default:
            throw std::runtime_error("Can't support current camera, add logo");
        }
    }

    std::shared_ptr<QPixmap> LogoManager::getCameraMakerLogo(const CameraIndex& cameraIndex)
    {
        loadCameraLogo(cameraIndex); ///< load logo again

        if (m_Logos.count(cameraIndex)) ///< get loaded logo
        {
            return m_Logos.at(cameraIndex);
        }

        throw std::runtime_error("Can't found Camera Maker Logo!");
    }

    void LogoManager::Init()
    {
        std::call_once(m_InitFlag, []()
        {
            m_CameraMakerMap.insert({"nikon", CameraIndex::Nikon});
            m_CameraMakerMap.insert({"sony", CameraIndex::Sony});
            m_CameraMakerMap.insert({"canon", CameraIndex::Canon});
            m_CameraMakerMap.insert({"panasonic", CameraIndex::Panasonic});
            m_CameraMakerMap.insert({"hassel", CameraIndex::Hasselblad});
            m_CameraMakerMap.insert({"leica", CameraIndex::Leica});
            m_CameraMakerMap.insert({"fujifilm", CameraIndex::Fujifilm});
            m_CameraMakerMap.insert({"empty", CameraIndex::None});
        });
    }
} // CM
