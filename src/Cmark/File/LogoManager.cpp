#include "CMark.h"

#include "LogoManager.h"

#include <unordered_map>
#include <QImageReader>
#include <QMessageBox>
#include <QPixmap>

namespace CM
{
    std::unordered_map<CameraIndex, std::shared_ptr<QPixmap>> LogoManager::m_Logos{};
    std::unordered_map<std::string, CameraIndex> LogoManager::m_CameraMakerMap;
    std::once_flag LogoManager::m_initFlag;

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

#if _DEBUG
        QMessageBox::about(nullptr, "Warning", ("We Don't supported: " + maker).c_str());
        throw std::runtime_error("Can't found Camera Maker!");
#else
        return CameraIndex::None;
#endif 
    }

    void LogoManager::loadCameraLogo(const CameraIndex& cameraIndex)
    {
        auto loadLogo = [](CameraIndex index, const std::string& path)-> std::pair<CameraIndex, std::shared_ptr<QPixmap>>
        {
            QImageReader logoReader(QString::fromStdString(path));
            auto temp = QPixmap::fromImageReader(&logoReader);

            auto logo = std::make_shared<QPixmap>(std::move(temp));
            return {index, logo};
        };

        if (m_Logos.count(cameraIndex)) ///< the logo loaded
        {
            return;
        }

        switch (cameraIndex)
        {
        case CameraIndex::Nikon:
            m_Logos.insert(loadLogo(CameraIndex::Nikon, ":/logos/nikon.png"));
            break;
        case CameraIndex::Sony:
            m_Logos.insert(loadLogo(CameraIndex::Sony, ":/logos/sony.png"));
            break;
        case CameraIndex::Canon:
            m_Logos.insert(loadLogo(CameraIndex::Canon, ":/logos/canon.png"));
            break;
        case CameraIndex::Panasonic:
            m_Logos.insert(loadLogo(CameraIndex::Panasonic, ":/logos/panasonic.png"));
            break;
        case CameraIndex::Leica:
            m_Logos.insert(loadLogo(CameraIndex::Leica, ":/logos/leica_logo.png"));
            break;
        case CameraIndex::Hasselblad:
            m_Logos.insert(loadLogo(CameraIndex::Hasselblad, ":/logos/hasselblad.png"));
            break;
        case CameraIndex::Apple:
            m_Logos.insert(loadLogo(CameraIndex::Apple, ":/logos/apple.png"));
            break;
        case CameraIndex::Fujifilm:
            m_Logos.insert(loadLogo(CameraIndex::Fujifilm, ":/logos/fujifilm.png"));
            break;
        case CameraIndex::None:
#if _DEBUG
            throw std::runtime_error("Can't support current camera, please add.");
#else
            {
                auto logo = std::make_shared<QPixmap>(64, 64);
                logo->fill(Qt::transparent);
                m_Logos.insert({CameraIndex::None, logo});
            }
#endif
            break;
        }
    }

    std::shared_ptr<QPixmap> LogoManager::getCameraMakerLogo(const CameraIndex& cameraIndex)
    {
        loadCameraLogo(cameraIndex); ///< load logo again
        if (m_Logos.count(cameraIndex)) ///< get loaded logo
        {
            return m_Logos.at(cameraIndex);
        }

#if _DEBUG
        throw std::runtime_error("Can't found Camera Maker Logo!");
#else
        return nullptr;
#endif
    }

    void LogoManager::destroy()
    {
        m_Logos.clear();
        m_CameraMakerMap.clear();
    }

    void LogoManager::Init()
    {
        std::call_once(m_initFlag, []()
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
