#pragma once

#include <vector>
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace CCTL
{
    // I don't like reading binary files
    std::vector<char> readBinaryFile(std::filesystem::path filename);

    // A std::optional that doesn't mess around
    template <typename T>
    struct Valueable // I was going to call it Nullable
    {
    public:
        T &operator()
        {
            if (!assigned)
                throw std::runtime_error("Valueable not assigned!");
            return val;
        }
        Valueable(const Valueable &other) = delete;
        Valueable(Valueable &&other) noexcept : val(std::move(other.val)), assigned(other.assigned)
        {
            other.assigned = false;
        }
        Valueable &operator=(const Valeable &other)
        {
            if (this == &other)
                throw std::runtime_error("NO.");
            val = other.val;
            assigned = other.assigned;
            return *this;
        }
        Valueable &operator=(Valueable &&other) = delete;
        ~Valueable() = default;

    private:
        T val{};
        bool assigned{false};
    };

    // The Linkle's Duo
    template <typename V>
    struct Duo
    {
        Duo(V newVal, Duo *nextDuo) : val{newVal}, next{nextDuo} {}
        Valueable<V> val{};
        Duo<V> *next{this};
    };

    // An easy linked list
    template <typename T>
    struct Linkle
    {
        Linkle() = default;
        Linkle(T val) : val{new T{val}}, size{1} {}
        ~Linkle()
        {
            for (Duo *next{nullptr}; size != 0; --size)
            {
                next = duo.next;
                duo.next = nullptr;
                delete duo;
            }
            duo = nullptr;
        }
        uint64_t size() { return size; }
        T &link(T val)
        {
            duo.next = new Duo<T>(val, duo.next);
            duo = duo.next;
            ++size;
        }
        T &operator++(int)
        {
            Duo<T> *thisOne = duo;
            duo = duo.next;
            return thisOne->val();
        }
        T &operator--(int)
        {
            T goneVal = duo.val();
            Duo<T> *gone = duo;
            Duo<T> *thisOne = duo.next;
            while (thisOne->next != gone)
                thisOne = thisOne->next;
            thisOne->next = gone->next;
            delete gone;
            --size;
            return goneVal;
        }

    private:
        uint64_t size{0};
        Duo<T> *duo{nullptr};
    };
}