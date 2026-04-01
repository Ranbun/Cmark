#ifndef CAMERAMARK_LRUCACHE_H
#define CAMERAMARK_LRUCACHE_H

#include <list>
#include <mutex>
#include <unordered_map>

namespace CM
{
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

        if (m_Maps.size() > m_capacity)
        {
            const auto oldKey = m_order.back();
            m_order.pop_back();
            m_Maps.erase(oldKey);
        }
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

private:
    std::list<K> m_order;
    using ListItem = typename std::list<K>::iterator;
    std::unordered_map<K, std::pair<V, ListItem>> m_Maps;
    size_t m_capacity{30};

    std::mutex m_mutex;
};

}  // namespace CM

#endif  // CAMERAMARK_LRUCACHE_H
