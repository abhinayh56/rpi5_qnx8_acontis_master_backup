#ifndef SHM_ELEMENT_H
#define SHM_ELEMENT_H

#include "Shm_store.h"

template <typename T>
class Shm_element
{
public:
    Shm_element(const std::string &key, const std::string &path, const T& value, Shm_store& shm_store) : m_key(key), m_path(path), m_value(value), m_shm_store(&shm_store)
    {
    }

    ~Shm_element()
    {
    }

    void setStore(Shm_store& shmStore)
    {
        m_shm_store = &shmStore;
    }

    void register_element()
    {
        m_shm_store->register_element(m_key, m_path, m_value, m_index, true);
    }

    void access_element()
    {
        m_shm_store->register_element(m_key, m_path, m_value, m_index, false);
    }

    void set(const T &value)
    {
        m_value = value;
        m_shm_store->set_element(m_value, m_index);
    }

    void get(T &value)
    {
        m_shm_store->get_element(m_value, m_index);
        value = m_value;
    }

private:
    std::string m_key = "";
    std::string m_path = "/";
    T m_value;
    uint64_t m_index = 0;

    Shm_store *m_shm_store = nullptr;
};

#endif // SHM_ELEMENT_H
