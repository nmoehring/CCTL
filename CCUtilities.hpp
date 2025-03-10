#pragma once

#include <vector>
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace CCUtilities
{
    std::vector<char> readBinaryFile(std::filesystem::path filename);

    template <typename T>
    class ListWrapper
    {
    public:
        std::list<T> data;

        T &operator[](std::size_t index)
        {
            if (deviceIdx >= m_numDevices)
                throw std::runtime_error("Device index out of range.");
            std::list<VknDevice>::iterator it = m_devices.begin();
            std::advance(it, index);
            return *it;
        }

        const T &operator[](std::size_t index) const
        {
            if (deviceIdx >= m_numDevices)
                throw std::runtime_error("Device index out of range.");
            std::list<VknDevice>::iterator it = m_devices.begin();
            std::advance(it, index);
            return *it;
        }
    };
}