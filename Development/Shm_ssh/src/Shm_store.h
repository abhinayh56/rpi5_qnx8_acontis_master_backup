#ifndef SHM_STORE_H
#define SHM_STORE_H

#include <iostream>
#include <stdint.h>
#include <map>
#include <string>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

// Flat shared structure stored in memory (mutex + value)
template <typename T>
struct DataBlock
{
    pthread_mutex_t mutex;
    T value;
};

class Shm_store
{
public:
    Shm_store()
    {
        m_shm_name = "";
        m_shm_size = 0;
        m_data_buffer = nullptr;
        global_mutex = nullptr;
    }

    Shm_store(const std::string &name, uint64_t size)
    {
        m_shm_name = "/" + name;
        m_shm_size = size;
        m_data_buffer = nullptr;
        global_mutex = nullptr;
    }

    ~Shm_store()
    {
    }

    void create(const std::string &name, uint64_t size)
    {
        m_shm_name = "/" + name;
        m_shm_size = size;

        int fd = shm_open(m_shm_name.c_str(), O_CREAT | O_RDWR, 0666);
        if (fd == -1)
        {
            perror("shm_open");
            exit(1);
        }
        {
            std::cout << "shm created" << std::endl;
        }
        if (ftruncate(fd, m_shm_size) == -1)
        {
            perror("ftruncate");
            exit(1);
        }
        {
            std::cout << "shm size set" << std::endl;
        }

        m_data_buffer = mmap(nullptr, m_shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (m_data_buffer == MAP_FAILED)
        {
            perror("mmap");
            exit(1);
        }
        close(fd);

        global_mutex = reinterpret_cast<pthread_mutex_t *>((uint8_t *)m_data_buffer);

        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(global_mutex, &attr);
        pthread_mutexattr_destroy(&attr);

        m_offset = sizeof(pthread_mutex_t);
    }

    void open(const std::string &name, uint64_t size)
    {
        m_shm_name = "/" + name;
        m_shm_size = size;
        
        bool shm_open_flag = false;

        int fd = 0;

        while (shm_open_flag == false)
        {
            fd = shm_open(m_shm_name.c_str(), O_RDWR, 0666);
            usleep(500000);

            if (fd == -1)
            {
                perror("shm_open");
                continue;
            }
            {
                std::cout << "shm created" << std::endl;
            }

            shm_open_flag = true;
        }

        m_data_buffer = mmap(nullptr, m_shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (m_data_buffer == MAP_FAILED)
        {
            perror("mmap");
            exit(1);
        }
        close(fd);

        global_mutex = reinterpret_cast<pthread_mutex_t *>((uint8_t *)m_data_buffer);
        m_offset = sizeof(pthread_mutex_t);
    }

    template <typename T>
    void register_element(const std::string &key, const std::string &path, const T &value, uint64_t &index, bool overwrite)
    {
        pthread_mutex_lock(global_mutex);

        std::string path_key = path + "/" + key;
        std::cout << "---" << std::endl;
        std::cout << path_key << std::endl;

        auto it = m_data_element_map.find(path_key);

        if (it == m_data_element_map.end())
        {
            size_t alignment = alignof(DataBlock<T>);
            uint64_t m_offset_required = (m_offset + alignment - 1) & ~(alignment - 1);
            size_t required_size = m_offset_required + sizeof(DataBlock<T>);

            if (required_size > m_shm_size)
            {
                std::cerr << "ERROR: Not enough shared memory for new data element." << std::endl;
                pthread_mutex_unlock(global_mutex);
                return;
            }

            m_offset = m_offset_required;
            index = m_offset;
            m_data_element_map[path_key] = index;
            m_offset += sizeof(DataBlock<T>);

            // Init mutex inside shared memory
            DataBlock<T> *ptr = reinterpret_cast<DataBlock<T> *>((uint8_t *)m_data_buffer + index);

            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
            pthread_mutex_init(&ptr->mutex, &attr);
            pthread_mutexattr_destroy(&attr);

            if (overwrite == true)
            {
                ptr->value = value;
            }

            std::cout << "INFO: Data element set.         Index: " << index << ", Key: " << key << ", Path: " << path << ", Value: " /*<< value*/ << std::endl;
        }
        else
        {
            index = m_data_element_map[path_key];

            DataBlock<T> *ptr = reinterpret_cast<DataBlock<T> *>((uint8_t *)m_data_buffer + index);
            pthread_mutex_lock(&ptr->mutex);
            ptr->value = value;
            pthread_mutex_unlock(&ptr->mutex);
            std::cout << "INFO: Data element already set. Index: " << index << ", Key: " << key << ", Path: " << path << ", Value: " /*<< value*/ << std::endl;
        }

        pthread_mutex_unlock(global_mutex);
    }

    template <typename T>
    void set_element(const T &value, uint64_t index)
    {
        DataBlock<T> *ptr = reinterpret_cast<DataBlock<T> *>((uint8_t *)m_data_buffer + index);
        if (pthread_mutex_trylock(&ptr->mutex) == 0)
        {
            ptr->value = value;
            pthread_mutex_unlock(&ptr->mutex);
            // std::cout << "W: " << value << std::endl;
        }
        else
        {
            // std::cerr << "WARN: Mutex busy during write. Skipped." << std::endl;
        }
    }

    template <typename T>
    void get_element(T &value, uint64_t index)
    {
        DataBlock<T> *ptr = reinterpret_cast<DataBlock<T> *>((uint8_t *)m_data_buffer + index);
        if (pthread_mutex_trylock(&ptr->mutex) == 0)
        {
            value = ptr->value;
            pthread_mutex_unlock(&ptr->mutex);
            // std::cout << "R: " << value << std::endl;
        }
        else
        {
            // std::cerr << "WARN: Mutex busy during read. Skipped." << std::endl;
        }
    }

    void destroy()
    {
        munmap(m_data_buffer, m_shm_size);
        shm_unlink(m_shm_name.c_str());
    }

private:
    std::string m_shm_name;
    uint64_t m_shm_size;

    void *m_data_buffer;
    uint64_t m_offset = 0;
    pthread_mutex_t *global_mutex;
    std::map<std::string, uint64_t> m_data_element_map;
};

#endif // SHM_STORE_H
