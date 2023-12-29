#ifndef CMARK_PICTUREMANAGER_H_
#define CMARK_PICTUREMANAGER_H_


#include <unordered_map>
#include <queue>
#include <memory>
#include <QPixmap>

namespace CM
{
    template<typename T1, typename T2>
    class FixMap
    {
    public:
        FixMap() = default;
        ~FixMap() { m_maps.clear(); }

        FixMap(const FixMap&) = delete;
        FixMap(const FixMap&&) = delete;

        FixMap & operator=(const FixMap &) = delete;
        FixMap & operator=(const FixMap &&) = delete;

    private:
        int maxSize = 20;
        std::unordered_map<T1, T2> m_maps;

    };

    template<>
    class FixMap<size_t,std::shared_ptr<QPixmap>>
    {
    public:
        FixMap() = default;
        ~FixMap()
        {
            m_Maps.clear();
        }

        FixMap(const FixMap&) = delete;
        FixMap(const FixMap&&) = delete;

        FixMap& operator=(const FixMap&) = delete;
        FixMap& operator=(const FixMap&&) = delete;


        void insert(const std::pair<size_t, std::shared_ptr<QPixmap>> & d)
        {
            if (m_Maps.size() > m_MaxSize)
            {
                const auto front = m_IndexCodeQueue.front();
                m_Maps.erase(front);
                m_IndexCodeQueue.pop();
            }

            m_IndexCodeQueue.emplace(d.first);
            m_Maps.insert(d);
        }

        void insert(const size_t key, const std::shared_ptr<QPixmap>& value)
        {
            insert({ key,value });
        }


        std::shared_ptr<QPixmap> getPixmap(const size_t indexCode)
        {
            if (m_Maps.count(indexCode))
            {
                return m_Maps.at(indexCode);
            }
            return nullptr;
        }


        void remove(const size_t indexCode)
        {
            if (m_Maps.count(indexCode))
            {
                m_Maps.erase(indexCode);

                std::queue<size_t> tempQueue;
                tempQueue.swap(m_IndexCodeQueue);

                while (!tempQueue.empty())
                {
                    if (auto f = tempQueue.front(); f != indexCode)
                    {
                        m_IndexCodeQueue.emplace(f);
                    }
                }
            }
        }

        void clear()
        {
            while(m_IndexCodeQueue.empty())
            {
                m_IndexCodeQueue.pop();
            }
            m_Maps.clear();
        }

        std::unordered_map<size_t, std::shared_ptr<QPixmap>>::iterator begin() { return m_Maps.begin(); }
        std::unordered_map<size_t, std::shared_ptr<QPixmap>>::iterator end() { return m_Maps.end(); }

    private:
        uint64_t m_MaxSize = 20;
        std::queue<size_t> m_IndexCodeQueue;
        std::unordered_map<size_t, std::shared_ptr<QPixmap>> m_Maps;
    };

    namespace Tools
    {
        class ResourcesTools;
    }

    class PictureManager
    {
        friend class Tools::ResourcesTools;
    public:
        PictureManager() = default;

        static void insert(const std::pair<size_t, std::shared_ptr<QPixmap>>& d)
        {
            m_Maps.insert(d);
        }

        static void insert(const size_t key, const std::shared_ptr<QPixmap>& value);

        static std::shared_ptr<QPixmap> getImage(const size_t key);

        static void remove(const size_t index);

        static size_t loadImage(const std::string& path);

    private:
        static void destory();
    private:
        static FixMap<size_t,std::shared_ptr<QPixmap>> m_Maps;

    };
}

#endif
