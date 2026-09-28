#ifndef DATA_STORE_ELEMENT_H_
#define DATA_STORE_ELEMENT_H_

#include <string>
#include "Data_store.h"

template <typename T>
class Data_store_element
{
public:
    Data_store_element()
    {
    }
    
    Data_store_element(std::string key_, std::string path_, T data_, bool overwrite_ = true)
    {
        m_key = key_;
        m_path = path_;
        m_size = sizeof(T);
        m_data = data_;

        // data_store.register_element<T>(m_key, m_path, data_, m_size, overwrite_, m_index_data, m_index_mutex);
    }

    ~Data_store_element()
    {
    }

    inline void set_info(std::string key_, std::string path_, T data_, bool overwrite_ = true)
    {
        m_key = key_;
        m_path = path_;
        m_size = sizeof(T);
        m_data = data_;
    }

    inline void publish()
    {
    	m_isPublished = true;
        data_store.register_element<T>(m_key, m_path, m_data, m_size, true, m_index_data, m_index_mutex);
    }

    inline void subscribe()
    {
    	m_isSubscribed = true;
        data_store.register_element<T>(m_key, m_path, m_data, m_size, false, m_index_data, m_index_mutex);
    }

    inline bool get(T &data_)
    {
    	if(m_isSubscribed)
    	{
    		return data_store.get<T>(m_index_data, m_index_mutex, data_, m_size);
    	}

		std::cout << "Data store element not subscribed. " << "m_key: " << m_key << ", m_path: " << m_path << ", m_size: " << m_size << std::endl;
		return false;
    }

    inline bool set(const T &data_)
    {
    	if(m_isPublished)
		{
    		return data_store.set<T>(m_index_data, m_index_mutex, data_, m_size);
		}

		std::cout << "Data store element not published. " << "m_key: " << m_key << ", m_path: " << m_path << ", m_size: " << m_size << std::endl;
		return false;
    }

    inline uint64_t get_index_data()
    {
        return m_index_data;
    }

    inline uint64_t get_index_mutex()
    {
        return m_index_mutex;
    }

private:
    std::string m_key;
    std::string m_path;
    T m_data;
    std::size_t m_size;
    uint64_t m_index_data = 0;
    uint64_t m_index_mutex = 0;

    bool m_isPublished = false;
    bool m_isSubscribed = false;

    Data_store &data_store = Data_store::getInstance();
};

#endif // DATA_STORE_ELEMENT_H
