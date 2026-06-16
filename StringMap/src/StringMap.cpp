
#include <bits/stdc++.h>


using namespace std;

template <typename T, typename V>
class StringMap
{
    struct Node{
        T key;
        V value;
    };

    vector<std::list<Node>> m_data;
    size_t m_nosOfElements;
    size_t m_bucketCount;

    size_t getIndex(const T& key) const
    {
        return std::hash<T>{}(key) % m_bucketCount; 
    }

    void checkload()
    {
        double currentLoad = static_cast<double>(m_nosOfElements)/m_bucketCount;
        if(currentLoad > 0.75)
        {
            vector<list<Node>> newData(m_bucketCount * 2);
            size_t newBucketCount = m_bucketCount * 2;

            for(auto& bucket : m_data)
            {
                for(auto& node : bucket)
                {
                    size_t idx = std::hash<T>{}(node.key) % newBucketCount;
                    newData[idx].push_back({std::move(node.key),std::move(node.value)});
                }
            }
            m_data = std::move(newData);
            m_bucketCount = newBucketCount;
        }
        return;
    }

public:

    StringMap():m_bucketCount(8) , m_nosOfElements(0){
        m_data.resize(8);
    }

    void insert(const T& key, V value){

        mapInsert(key,value);
        checkload();
    }


    void mapInsert(const T& key, V value){

        size_t val = getIndex(key);        
        auto& bucket = m_data[val];
        for(auto& node : bucket)
        {
            if(node.key == key)
            {
                node.value = value;
                return;
            }
        }
        m_data[val].push_back({key, std::move(value)});
        ++m_nosOfElements;
    }

    bool find(const T& key, V& value) const{
        size_t val = getIndex(key);
        
        const auto& listData = m_data[val];
        for(const auto& [lkey, lvalue] : listData)
        {
            if(lkey == key)
            {
                value = lvalue;
                return true;
            }
        }
        return false;
    }

    bool erase(const T& key){
        size_t val = getIndex(key);
        if(val >= m_data.size() )
            return false;
        
        auto& bucket = m_data[val];
        for(auto it = bucket.begin(); it != bucket.end(); ++it)
        {
            if(it->key == key)
            {
                bucket.erase(it);
                --m_nosOfElements;
                return true;
            }
        }
        return false;
    }

    size_t size() const{
        return m_nosOfElements;
    }
};


int main()
{
    return 0;
}