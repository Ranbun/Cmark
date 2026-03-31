#ifndef CMARK_FIXMAP_H_
#define CMARK_FIXMAP_H_

#include <list>
#include <mutex>
#include <queue>
#include <unordered_map>

namespace CM
{
template <typename T1, typename T2>
class FixMap
{
public:
    FixMap() = default;
    ~FixMap() = default;

    FixMap(const FixMap&) = delete;
    FixMap(const FixMap&&) = delete;

    FixMap& operator=(const FixMap&) = delete;
    FixMap& operator=(const FixMap&&) = delete;

protected:
    int m_MaxSize = 20;
};

template <typename T>
class FixMap<size_t, T>
{
public:
    FixMap() = default;
    ~FixMap()
    {
        m_Maps.clear();
    }

    FixMap(const FixMap&) = delete;
    FixMap(FixMap&&) = delete;

    FixMap& operator=(const FixMap&) = delete;
    FixMap& operator=(FixMap&&) = delete;

    void insert(const std::pair<size_t, T>& d)
    {
        if (m_Maps.count(d.first))
        {
            /// remove the old item
            remove(d.first);
            m_Maps[d.first] = d.second;

            return;
        }

        /// insert the new item
        m_IndexCodeQueue.emplace(d.first);
        m_Maps.insert(d);

        /// remove the oldest item
        if (m_Maps.size() > m_MaxSize)
        {
            const auto front = m_IndexCodeQueue.front();
            m_Maps.erase(front);
            m_IndexCodeQueue.pop();
        }
    }

    void insert(const size_t key, const T& value)
    {
        insert({key, value});
    }

    T getItem(const size_t indexCode)
    {
        if (m_Maps.count(indexCode))
        {
            return m_Maps.at(indexCode);
        }
        return T{};
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
                    tempQueue.pop();
                }
                else
                {
                    tempQueue.pop();
                }
            }
        }
    }

    void clear()
    {
        while (!m_IndexCodeQueue.empty())
        {
            m_IndexCodeQueue.pop();
        }
        m_Maps.clear();
    }

    typename std::unordered_map<size_t, T>::iterator begin()
    {
        return m_Maps.begin();
    }
    typename std::unordered_map<size_t, T>::iterator end()
    {
        return m_Maps.end();
    }

private:
    uint64_t m_MaxSize = 20;

    std::queue<size_t> m_IndexCodeQueue;
    std::unordered_map<size_t, T> m_Maps;
};

/// 作为缓存使用
/// 实现一个LRUCache，使用list来存储顺序，使用unordered_map来存储键值对
/// 不设置固定大小，每次使用的时候 给现有的成员加一个标记， 等到一定的次数尝试移除没有使用的对象
/// 每次插入的对象都要把它的索引放到最前面，保证最新使用的对象在前面
/// 每次使用过的对象也都要把数据插入最前面
template <typename K, typename V>
class LRUCache
{
public:
    LRUCache() = default;
    ~LRUCache()
    {
        m_Maps.clear();
        m_order.clear();
    }

    LRUCache(const LRUCache&) = delete;
    LRUCache(LRUCache&&) = delete;

    LRUCache& operator=(const LRUCache&) = delete;
    LRUCache& operator=(LRUCache&&) = delete;

    void insert(const K key, const V& value)
    {
        std::scoped_lock local(m_mutex);
        auto it = m_Maps.find(key);
        if (it != m_Maps.end())
        {
            it->second.first = value;
            m_order.splice(m_order.begin(), m_order, it->second.second);
        }
        else
        {
            /// update value
            m_order.push_front(key);
            m_Maps.insert({key, {value, m_order.begin()}});
        }

        /// TODO: remove old
    }

    bool getItem(const K& key, V& value)
    {
        std::scoped_lock local(m_mutex);
        auto it = m_Maps.find(key);
        if (it == m_Maps.end())
        {
            return false;
        }

        m_order.splice(m_order.begin(), m_order, it->second.second);
        value = it->second.first;

        return true;
    }

    void remove(const K& key)
    {
        std::scoped_lock local(m_mutex);
        auto it = m_Maps.find(key);
        if (it != m_Maps.end())
        {
            m_order.erase(it->second.second);
            m_Maps.erase(it);
        }
    }

    void clear()
    {
        m_order.clear();
        m_Maps.clear();
    }

    [[deprecated]] typename std::unordered_map<K, V>::iterator begin()
    {
        return m_Maps.begin();
    }
    [[deprecated]] typename std::unordered_map<K, V>::iterator end()
    {
        return m_Maps.end();
    }

private:
    std::list<K> m_order;
    using ListItem = typename std::list<K>::iterator;
    std::unordered_map<K, std::pair<V, ListItem>> m_Maps;

    std::mutex m_mutex;
    
};

}  // namespace CM

#endif  // !CMARK_FIXMAP_H_
