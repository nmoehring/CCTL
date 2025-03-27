#include <gtest/gtest.h>
#include "CCTL.hpp"

TEST(ValueableTest, DefaultConstructor)
{
    CCTL::Valueable<int> v;
    ASSERT_FALSE(v);
}

TEST(ValueableTest, ValueConstructor)
{
    CCTL::Valueable<int> v(42);
    ASSERT_TRUE(v);
    ASSERT_EQ(v(), 42);
}

// ... more Valueable tests ...

TEST(DuoTest, DefaultConstructor)
{
    CCTL::Duo<int> d;
    ASSERT_EQ(d.bond, nullptr);
}

// ... more Duo tests ...

TEST(LinkleTest, DefaultConstructor)
{
    CCTL::Linkle<int> l;
    ASSERT_EQ(l.size(), 0);
}

// ... more Linkle tests ...

TEST(LinkleTest, Unify)
{
    CCTL::Linkle<int> l1;
    l1 = 1;
    l1.cast(2);
    CCTL::Linkle<int> l2;
    l2 = 3;
    l2.cast(4);
    l1.unify(l2);
    ASSERT_EQ(l1.size(), 4);
    ASSERT_EQ(l2.size(), 0);
}

// ... more Linkle tests ...