#pragma once

#include <vector>
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace CCTL
{
    // A std::optional that doesn't mess around
    template <typename T>
    struct Valueable // I was going to call it Nullable
    {
    private:
        T spark{};
        bool lit{false};

    public:
        Valueable() = default;
        Valueable(T newSpark) : spark{newSpark}, lit{true} {}
        Valueable(Valueable &other) = default;
        Valueable(Valueable &&other) noexcept
            : spark{std::move(other.spark)}, lit{other.lit}
        {
            other.lit = false;
        }
        Valueable &operator=(const Valueable &other) = default;
        Valueable &operator=(Valueable &&other)
        {
            spark = std::move(other.spark);
            lit = other.lit;
            other.lit = false;
            return *this;
        }
        void operator=(T spark)
        {
            this->spark = spark;
            lit = true;
        }
        explicit operator bool() const { return lit; }
        ~Valueable() = default;
        T &operator()()
        {
            if (!lit)
                throw std::runtime_error("Valueable not assigned!");
            return spark;
        }
    };

    // The Linkle's Duo
    template <typename V>
    struct Duo
    {
        Valueable<V> lume{};
        Duo<V> *bond{nullptr};

        Duo() = default;
        Duo(V spark) : lume{spark} { bond = this; }
        Duo(V spark, Duo *bondDuo) : lume{spark}, bond{bondDuo} {}
        Duo(const Duo &other) : lume{}, bond{other.bond} { other.bond = this; }
        Duo(Duo &&other) = delete;
        Duo &operator=(const Duo &other)
        {
            lume = other.lume;
            return *this;
        };
        Duo &operator=(Duo &&other) noexcept
        {
            lume = std::move(other.lume);
            bond = other.bond;
            other.bond = nullptr;
            return *this;
        }
        V &operator=(const V spark)
        {
            lume = spark;
            return lume();
        }
        V &operator()() { return lume(); }
    };

    // An easy linked list
    template <typename W>
    struct Linkle
    {
    private:
        Duo<W> *duo{new Duo<W>{}};
        uint64_t tally{0};

    public:
        Linkle() = default;
        Linkle(W spark) : duo{new Duo<W>{spark}}, tally{1} {}
        ~Linkle()
        {
            for (Duo<W> *next{nullptr}; tally != 0; --tally)
            {
                next = duo->bond;
                duo->bond = nullptr;
                delete duo;
            }
            duo = nullptr;
        }
        Linkle &operator=(Linkle &other)
        {
            (*duo) = (*other.duo);
            return *this;
        }
        Linkle &operator=(Linkle &&other) noexcept
        {
            (*duo) = std::move(*other.duo);
            return *this;
        }
        W &operator=(const W spark)
        {
            (*duo) = spark;
            return (*duo)();
        }
        W &operator()() { return (*duo)(); }
        W &operator++(int)
        {
            Duo<W> *it = duo;
            duo = duo->bond;
            return (*it)();
        }
        void cast()
        {
            duo->bond = new Duo<W>{*this};
            ++tally;
            duo = duo->bond;
        }
        void cast(W spark)
        {
            this->cast();
            duo->lume = spark;
        }
        Duo<W> *step(CCTL::Valueable<uint64_t> count = CCTL::Valueable<uint64_t>{})
        {
            if (tally == 0)
                throw std::runtime_error("Trying to step a Linkle with no duos!");
            if (!count)
                for (uint64_t i = 0; i < tally; ++i)
                    duo = duo->bond;
            else if (count() < tally)
                for (uint64_t i = 0; i < count(); ++i)
                    duo = duo->bond;
            return duo;
        }
        // TODO: Link Linkles without
        void unify(Linkle<W> &other)
        {
            Duo<W> *otherTop = other.duo;
            Duo<W> *otherBottom = other.step();
            Duo<W> *thisTop = duo;
            Duo<W> *thisBottom = this->step();
            this->step(1);
            otherTop->bond = thisBottom;
            thisTop->bond = otherBottom;
            other.duo = new Duo<W>{};
            tally += other.tally;
            other.tally = 0;
        }
        uint64_t size() { return tally; }
    };
} // namespace CCTL
