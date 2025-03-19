#pragma once

#include <vector>
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace CCUtilities
{
    std::vector<char> readBinaryFile(std::filesystem::path filename);

    template <typename T> // I was going to call it Nullable
    struct Valueable      // Yes, I know about std::Optional
    {
    public:
        Valuable();
        T operator()
        {
            if (!assigned)
                throw std::runtime_error("Valueable not assigned!");
            return val;
        }

    private:
        T val{};
        bool assigned{false};
    };

    template <typename T>
    class Linkle
    {
    public:
        Linkle(T data) : data{data}
        {
            if (!tail && ++size == 1)
                tail = this;
            next = this;
        }

        ~Linkle()
        {
            Linkle *temp = next;
            next = nullptr;
            delete data;
        }

        T &operator~() { return data; }
        Linkle &operator++(int) { return *next; };

        T *data;
        Linkle *next{this};

        static uint32_t size{0};
        static Linkle *tail{nullptr};

        T *push(T &newElement)
        {
            return (next = new Linkle{std::move(newElement), next}).data;
        }
        Linkle *push(Linkle newLinkle) { return next = newLinkle; }

    private:
        Linkle(T data, Linkle &next) { ++size; }
        Linkle(T data, Linkle *next) : data{data}, next{next} { ++size; }
    };
}