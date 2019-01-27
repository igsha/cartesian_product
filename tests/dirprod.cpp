#include <array>
#include <deque>
#include <iterator>
#include <tuple>
#include <vector>

#include <catch2/catch.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/irange.hpp>

#include <dirprod/range.hpp>

TEST_CASE("iterator types")
{
    std::vector<int> i0{{7, 9, 24}};
    std::deque<char> i1{{'a', 'b', 'c'}};
    auto i2 = boost::irange(7, 50);

    auto r = dirprod::make_range(i0, i1, i2);
    using iterator_t = decltype(r.begin());

    static_assert(std::is_same_v<typename std::iterator_traits<iterator_t>::value_type, std::tuple<int, char, int>>);
    static_assert(std::is_same_v<typename std::iterator_traits<iterator_t>::reference, std::tuple<int&, char&, int>>);

    REQUIRE(r.begin() != r.end());
}

TEST_CASE("different types")
{
    enum Components { Y, Cb, Cr, COMPONENTS_SIZE };
    enum Modes { DC, PLANAR, HORIZONTAL, VERTICAL, ANGLE45, MODES_SIZE };

    std::vector<Components> components{{Y, Cb, Cr}};
    std::deque<bool> booleans{false, true};
    std::array<Modes, MODES_SIZE> modes{{DC, PLANAR, HORIZONTAL, VERTICAL, ANGLE45}};

    auto range = dirprod::make_range(components, booleans, modes);

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
                    CHECK(tcompid == compid);
                    CHECK(tboolean == boolean);
                    CHECK(tmode == mode);
                }
            }
        }

        REQUIRE(it == last);
    }

    SECTION("for loop")
    {
        for (const auto& [component, boolean, mode] : range)
        {
            (void)boolean; // suppress warning
            CHECK(component < COMPONENTS_SIZE);
            CHECK(mode < MODES_SIZE);
        }
    }
}

TEST_CASE("ranges")
{
    auto range0 = boost::irange(0, 2);
    auto range1 = boost::irange(0, 1);
    auto range2 = boost::irange(0, 3);

    auto range = dirprod::make_range(range0, range1, range2);
    auto it = range.begin();
    auto last = range.end();

    REQUIRE(it != last);

    REQUIRE(*it++ == std::make_tuple(0, 0, 0));
    REQUIRE(*it++ == std::make_tuple(1, 0, 0));
    REQUIRE(*it++ == std::make_tuple(0, 0, 1));
    REQUIRE(*it++ == std::make_tuple(1, 0, 1));
    REQUIRE(*it++ == std::make_tuple(0, 0, 2));
    REQUIRE(*it++ == std::make_tuple(1, 0, 2));

    REQUIRE(it == last);
}

TEST_CASE("with iterators")
{
    auto range0 = boost::irange(0, 10);
    auto range1 = boost::irange(7, 19);

    auto range = dirprod::make_range(range0, range1);
    auto it = range.begin();
    auto last = range.end();

    for (int i0 = 0, i1 = 7; it != last; ++it)
    {
        auto [r0, r1] = *it;
        CHECK(r0 == i0);
        CHECK(r1 == i1);

        if (++i0 >= 10)
        {
            ++i1;
            i0 = 0;
        }
    }
}

TEST_CASE("const iterator")
{
    const std::vector<int> x{{7, 15, 24, 33, 17}};

    auto it = dirprod::make_range(x);

    auto ref = boost::make_transform_iterator(x.begin(), [](const auto x)
    {
        return std::make_tuple(x);
    });

    REQUIRE(std::equal(it.begin(), it.end(), ref));
}

TEST_CASE("move ranges")
{
    auto rng = dirprod::make_range(boost::irange(0, 11), std::vector<double>({7, 55.2, 3.17, -8.5}));
    auto it = rng.begin();

    for (int i = 0; i < 11; ++i) //!< \todo std::advance(it2, std::make_tuple(0, 1))
        ++it;

    REQUIRE(it != rng.end());
    CHECK(*it == std::make_tuple(0, 55.2));
}
