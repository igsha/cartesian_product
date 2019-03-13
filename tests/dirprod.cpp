#include <array>
#include <deque>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>

#include <catch2/catch.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/irange.hpp>

#include <dirprod/range.hpp>

namespace std
{
    template<class... Ts>
    std::ostream& operator<<(std::ostream& os, const std::tuple<Ts...>& t)
    {
        return std::apply([&os](const auto& x0, const auto&... xs) -> decltype(auto) {
            os << "(" << x0;
            (..., (os << "," << xs));
            return os << ")";
        }, t);
    }
}

TEST_CASE("iterator types")
{
    std::vector<int> i0{{7, 9, 24}};
    std::deque<char> i1{{'a', 'b', 'c'}};
    auto i2 = boost::irange(7, 50);

    auto r = dirprod::range(i0, i1, i2);
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

    auto range = dirprod::range(components, booleans, modes);

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
                    auto [tcompid, tboolean, tmode] = *it;
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

    auto range = dirprod::range(range0, range1, range2);
    auto it = range.begin();
    auto last = range.end();

    REQUIRE(it != last);

    CHECK(*it++ == std::make_tuple(0, 0, 0));
    CHECK(*it++ == std::make_tuple(1, 0, 0));
    CHECK(*it++ == std::make_tuple(0, 0, 1));
    CHECK(*it++ == std::make_tuple(1, 0, 1));
    CHECK(*it++ == std::make_tuple(0, 0, 2));
    CHECK(*it++ == std::make_tuple(1, 0, 2));

    CHECK(it == last);
}

TEST_CASE("with iterators")
{
    auto range0 = boost::irange(0, 10);
    auto range1 = boost::irange(7, 19);

    auto range = dirprod::range(range0, range1);
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

TEST_CASE("one element")
{
    SECTION("by reference")
    {
        std::vector x{4, 5, 6, 9, 47};
        auto rng = dirprod::range{x};
        auto it = rng.begin();

        CHECK(*it++ == 4);
        CHECK(*it++ == 5);
        CHECK(*it++ == 6);
        CHECK(*it++ == 9);
        CHECK(*it++ == 47);
        CHECK(it == rng.end());
    }

    SECTION("by prvalue")
    {
        auto rng = dirprod::range{std::vector{1, 2, 8}};
        auto it = rng.begin();

        CHECK(*it++ == 1);
        CHECK(*it++ == 2);
        CHECK(*it++ == 8);
        CHECK(it == rng.end());
    }
}

TEST_CASE("move ranges")
{
    auto rng = dirprod::range(boost::irange(0, 11), std::vector<double>({7, 55.2, 3.17, -8.5}));
    auto it = rng.begin();

    for (int i = 0; i < 11; ++i) //!< \todo std::advance(it2, std::make_tuple(0, 1))
        ++it;

    REQUIRE(it != rng.end());
    CHECK(*it == std::make_tuple(0, 55.2));
}

TEST_CASE("direct random access")
{
    auto rng = dirprod::range(boost::irange(0, 11), boost::irange(7, 55), boost::irange(13, 40));
    auto it = rng.begin();

    CHECK(*(it + (3 + 11 * 8 + 11 * (55 - 7) * 1)) == std::make_tuple(3, 15, 14));
    CHECK(*it == std::make_tuple(0, 7, 13));
    CHECK(*(it + ((40 - 13) * (55 - 7) * (11 - 0) - 1)) == std::make_tuple(10, 54, 39));
    CHECK(*(it + ((40 - 13) * (55 - 7) * (11 - 0))) == std::make_tuple(0, 7, 40));
}

TEST_CASE("const range")
{
    std::vector<int> r0{{1, 2, 3}};
    const std::vector<int> r1{{4, 5, 6}};

    auto rng = dirprod::range(r0, r1);
    auto it = rng.begin();

    REQUIRE(it != rng.end());
    CHECK(*(it + 2 + 3 * 1) == std::make_tuple(3, 5));
}
