#include "LogoManager.h"

#include <QImageReader>
#include <QPixmap>

namespace CM
{
std::unordered_map<CameraIndex, std::shared_ptr<QPixmap>> LogoManager::m_Logos{};
std::unordered_map<std::string, CameraIndex> LogoManager::m_CameraMakerMap;
std::once_flag LogoManager::m_initFlag;

static std::mutex m_DataMutex;

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

    std::scoped_lock local(m_DataMutex);
    const auto res = m_CameraMakerMap.find(maker);
    if (res != m_CameraMakerMap.end())
    {
        return res->second;
    }

#if _DEBUG
    throw std::runtime_error("Can't found Camera Maker!");
#else
    return CameraIndex::None;
#endif
}

void LogoManager::loadCameraLogo(const CameraIndex& cameraIndex)
{
    auto loadLogo = [](CameraIndex index, const std::string& path) -> std::pair<CameraIndex, std::shared_ptr<QPixmap>>
    {
        QImageReader logoReader(QString::fromStdString(path));
        auto temp = QPixmap::fromImageReader(&logoReader);

        auto logo = std::make_shared<QPixmap>(std::move(temp));
        return {index, logo};
    };

    static std::unordered_map<CameraIndex, std::string> logPath{
        {CameraIndex::Nikon, ":/logos/nikon.png"},      {CameraIndex::Sony, ":/logos/sony.png"},
        {CameraIndex::Canon, ":/logos/canon.png"},      {CameraIndex::Panasonic, ":/logos/panasonic.png"},
        {CameraIndex::Leica, ":/logos/leica_logo.png"}, {CameraIndex::Hasselblad, ":/logos/hasselblad.png"},
        {CameraIndex::Apple, ":/logos/apple.png"},      {CameraIndex::Fujifilm, ":/logos/fujifilm.png"},
    };

    {
        std::scoped_lock lock(m_DataMutex);
        if (m_Logos.count(cameraIndex))  ///< the logo loaded
        {
            return;
        }
    }

    if (logPath.count(cameraIndex))
    {
        auto res = std::move(loadLogo(cameraIndex, logPath.at(cameraIndex)));
        std::scoped_lock lock(m_DataMutex);
        if (!m_Logos.count(cameraIndex))
        {
            m_Logos.insert(std::move(res));
        }
    }
    else
    {
#if _DEBUG
        throw std::runtime_error("Can't support current camera, please add.");
#else
        auto logo = std::make_shared<QPixmap>(64, 64);
        logo->fill(Qt::transparent);
        std::scoped_lock lock(m_DataMutex);
        if (!m_Logos.count(cameraIndex))
        {
            m_Logos.insert({cameraIndex, logo});
        }
#endif
    }
}

std::shared_ptr<QPixmap> LogoManager::getCameraMakerLogo(const CameraIndex& cameraIndex)
{
    loadCameraLogo(cameraIndex);  ///< load logo again

    std::scoped_lock local(m_DataMutex);
    if (m_Logos.count(cameraIndex))  ///< get loaded logo
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
    std::scoped_lock local(m_DataMutex);
    m_Logos.clear();
    // m_CameraMakerMap.clear();
}

void LogoManager::Init()
{
    std::call_once(m_initFlag,
                   []()
                   {
                       std::scoped_lock local(m_DataMutex);
                       m_CameraMakerMap.insert({"nikon", CameraIndex::Nikon});
                       m_CameraMakerMap.insert({"sony", CameraIndex::Sony});
                       m_CameraMakerMap.insert({"canon", CameraIndex::Canon});
                       m_CameraMakerMap.insert({"panasonic", CameraIndex::Panasonic});
                       m_CameraMakerMap.insert({"hassel", CameraIndex::Hasselblad});
                       m_CameraMakerMap.insert({"leica", CameraIndex::Leica});
                       m_CameraMakerMap.insert({"fujifilm", CameraIndex::Fujifilm});
                       m_CameraMakerMap.insert({"apple", CameraIndex::Apple});
                       m_CameraMakerMap.insert({"empty", CameraIndex::None});
                       /// add new camera maker here
                   });
}
}  // namespace CM
