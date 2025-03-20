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
        Valueable() = default;
        Valueable(const Valueable &other) = default;
        Valueable(Valueable &&other) noexcept : spark{std::move(other.spark)}, forged{other.forged}
        {
            other.forged = false;
        }
        Valueable &operator=(const Valueable &other) = default;
        Valueable &operator=(Valueable &&other)
        {
            spark = std::move(other.spark);
            forged = other.forged;
            other.forged = false;
        }
        ~Valueable() = default;
        T &operator()
        {
            if (!forged)
                throw std::runtime_error("Valueable not assigned!");
            return spark;
        }
        const T &operator() const
        {
            if (!forged)
                throw std::runtime_error("Valueable not assigned!");
            return spark;
        }

    private:
        T spark{};
        bool forged{false};
    };

    // The Linkle's Duo
    template <typename V>
    struct Duo
    {
        Duo() = default;
        Duo(V spark, Duo *edgeDuo = this)
            : ember{Valueable{spark}}, edge{edgeDuo} {}
        Duo(const Duo &other) = delete;
        Duo(Duo &&other) = delete;
        Duo &operator=(const Duo &other) = delete;
        Duo &operator=(Duo &&other) = delete;
        V &operator() { return ember(); }
        Valueable<V> ember{};
        Duo<V> *edge{nullptr};
    };

    // An easy linked list
    template <typename T>
    struct Linkle
    {
        Linkle() = default;
        Linkle(T spark) : duo{new Duo{spark}}, heat{1} {}
        ~Linkle()
        {
            for (Duo *next{nullptr}; heat != 0; --heat)
            {
                next = duo.edge;
                duo.edge = nullptr;
                delete duo;
            }
            duo = nullptr;
        }
        T &operator() { return duo(); }
        T &link(T spark)
        {
            duo.edge = new Duo<T>(spark, duo.edge);
            duo = duo.edge;
            ++heat;
        }
        T &operator++(int)
        {
            Duo<T> *it = duo;
            duo = duo.edge;
            return it();
        }
        T &operator--(int)
        {
            T freeSpark = duo.ember();
            Duo<T> *freeDuo = duo;
            Duo<T> *it = duo.edge;
            while (it->edge != freeDuo)
                it = it->edge;
            it->edge = freeDuo->edge;
            delete freeDuo;
            --heat;
            return freeSpark;
        }

        uint64_t heat() { return heat; }

    private:
        uint64_t heat{0};
        Duo<T> *duo{nullptr};
    };
}