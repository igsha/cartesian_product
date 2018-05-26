#define CATCH_CONFIG_MAIN

#include <array>
#include <deque>
#include <iterator>
#include <vector>

#include <catch.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/irange.hpp>
#include <direct_product/range.hpp>

using namespace std;

TEST_CASE("iterator types")
{
    vector<int> i0{{7, 9, 24}};
    deque<char> i1{{'a', 'b', 'c'}};
    auto i2 = boost::irange(7, 50);

    auto r = direct_product::make_range(i0, i1, i2);
    using iterator_t = decltype(r.begin());

    static_assert(is_same_v<typename iterator_traits<iterator_t>::value_type, tuple<int, char, int>>);
    static_assert(is_same_v<typename iterator_traits<iterator_t>::reference, tuple<int&, char&, int>>);

    REQUIRE(r.begin() != r.end());
}

TEST_CASE("different types")
{
    enum Components { Y, Cb, Cr, COMPONENTS_SIZE };
    enum Modes { DC, PLANAR, HORIZONTAL, VERTICAL, ANGLE45, MODES_SIZE };

    vector<Components> components{{Y, Cb, Cr}};
    deque<bool> booleans{false, true};
    array<Modes, MODES_SIZE> modes{{DC, PLANAR, HORIZONTAL, VERTICAL, ANGLE45}};

    auto range = direct_product::make_range(components, booleans, modes);

    SECTION("iterators")
    {
        auto it = range.begin();
        auto last = range.end();

        for (int mode = DC; mode < MODES_SIZE; ++mode)
        {
            for (int boolean = 0; boolean < 2; ++boolean)
            {
                for (int compid = Y; compid < COMPONENTS_SIZE; ++compid, ++it)
                {
                    REQUIRE(it != last);
                    const auto& [tcompid, tboolean, tmode] = *it;
                    REQUIRE(tcompid == compid);
                    REQUIRE(tboolean == boolean);
                    REQUIRE(tmode == mode);
                }
            }
        }

        REQUIRE(it == last);
    }

    SECTION("for loop")
    {
        for (const auto& t : range)
        {
            auto [component, boolean, mode] = t;
            REQUIRE(component < COMPONENTS_SIZE);
            REQUIRE(mode < MODES_SIZE);
        }
    }
}

TEST_CASE("ranges")
{
    auto range0 = boost::irange(0, 2);
    auto range1 = boost::irange(0, 1);
    auto range2 = boost::irange(0, 3);

    auto range = direct_product::make_range(range0, range1, range2);
    auto it = range.begin();
    auto last = range.end();

    REQUIRE(it != last);

    REQUIRE(*it++ == make_tuple(0, 0, 0));
    REQUIRE(*it++ == make_tuple(1, 0, 0));
    REQUIRE(*it++ == make_tuple(0, 0, 1));
    REQUIRE(*it++ == make_tuple(1, 0, 1));
    REQUIRE(*it++ == make_tuple(0, 0, 2));
    REQUIRE(*it++ == make_tuple(1, 0, 2));

    REQUIRE(it == last);
}

TEST_CASE("with iterators")
{
    auto range0 = boost::irange(0, 10);
    auto range1 = boost::irange(7, 19);

    auto range = direct_product::make_range(range0, range1);
    auto it = range.begin();
    auto last = range.end();

    for (int i0 = 0, i1 = 7; it != last; ++it)
    {
        auto [r0, r1] = *it;
        REQUIRE(r0 == i0);
        REQUIRE(r1 == i1);

        if (++i0 >= 10)
        {
            ++i1;
            i0 = 0;
        }
    }
}

TEST_CASE("const iterator")
{
    const vector<int> x{{7, 15, 24, 33, 17}};

    auto it = direct_product::make_range(x);

    auto ref = boost::make_transform_iterator(x.begin(), [](auto x)
    {
        return std::make_tuple(x);
    });

    REQUIRE(std::equal(it.begin(), it.end(), ref));
}
