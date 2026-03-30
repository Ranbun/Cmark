#include "LogoManager.h"

#include <QImage>
#include <QImageReader>
#include <QList>

namespace CM
{
    /// 集中定义所有支持的相机厂商
    static const std::vector<CameraInfo> kCameraInfoTable = {
        {"nikon",      CameraIndex::Nikon,      ":/logos/nikon.png"},
        {"sony",       CameraIndex::Sony,       ":/logos/sony.png"},
        {"canon",      CameraIndex::Canon,      ":/logos/canon.png"},
        {"panasonic",  CameraIndex::Panasonic,  ":/logos/panasonic.png"},
        {"leica",      CameraIndex::Leica,      ":/logos/leica_logo.png"},
        {"hassel",     CameraIndex::Hasselblad, ":/logos/hasselblad.png"},
        {"fujifilm",   CameraIndex::Fujifilm,   ":/logos/fujifilm.png"},
        {"apple",      CameraIndex::Apple,      ":/logos/apple.png"},
        {"empty",      CameraIndex::None,       ""},
    };

    LogoManager& LogoManager::instance()
    {
        static LogoManager inst;
        return inst;
    }

    LogoManager::LogoManager()
    {
        for (const auto& info : kCameraInfoTable)
        {
            m_cameraMakerMap[info.makerName] = info.index;
            if (!info.logoPath.empty())
            {
                m_logoPathMap[info.index] = info.logoPath;
            }
        }
    }

    CameraIndex LogoManager::resolveCameraIndex(const std::string& cameraMake)
    {
        QString make = cameraMake.c_str();
        if (make.isEmpty())
        {
            make = QString("empty");
        }

        /// TODO : 需要更兼容的获取 camera maker 的方法

        const auto lists = make.split(" ");
        const auto maker = lists[0].toLower().toStdString();

        std::scoped_lock lock(m_dataMutex);
        const auto it = m_cameraMakerMap.find(maker);
        if (it != m_cameraMakerMap.end())
        {
            return it->second;
        }

#ifdef _DEBUG
        throw std::runtime_error("Can't find Camera Maker!");
#else
        return CameraIndex::None;
#endif
    }

    void LogoManager::loadCameraLogo(CameraIndex cameraIndex)
    {
        // 快速检查：已加载则跳过
        {
            std::scoped_lock lock(m_dataMutex);
            if (m_logos.count(cameraIndex))
            {
                return;
            }
        }

        // 在锁外查找路径并执行 I/O 加载
        std::string logoPath;
        {
            std::scoped_lock lock(m_dataMutex);
            auto pathIt = m_logoPathMap.find(cameraIndex);
            if (pathIt != m_logoPathMap.end())
            {
                logoPath = pathIt->second;
            }
        }

        std::shared_ptr<QImage> logo;
        if (!logoPath.empty())
        {
            QImageReader logoReader(QString::fromStdString(logoPath));
            auto temp = logoReader.read();
            logo = std::make_shared<QImage>(std::move(temp));
        }
        else
        {
#ifdef _DEBUG
            throw std::runtime_error("Can't support current camera, please add.");
#else
            logo = std::make_shared<QImage>(64, 64, QImage::Format_ARGB32);
            logo->fill(Qt::transparent);
#endif
        }

        // 加锁插入（double-check）
        std::scoped_lock lock(m_dataMutex);
        if (!m_logos.count(cameraIndex))
        {
            m_logos[cameraIndex] = std::move(logo);
        }
    }

    std::shared_ptr<QImage> LogoManager::getCameraMakerLogo(CameraIndex cameraIndex)
    {
        loadCameraLogo(cameraIndex);

        std::scoped_lock lock(m_dataMutex);
        auto it = m_logos.find(cameraIndex);
        if (it != m_logos.end())
        {
            return it->second;
        }

#ifdef _DEBUG
        throw std::runtime_error("Can't find Camera Maker Logo!");
#else
        return nullptr;
#endif
    }

    void LogoManager::destroy()
    {
        std::scoped_lock lock(m_dataMutex);
        m_logos.clear();
    }
} // namespace CM
