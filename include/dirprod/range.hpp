#ifndef __DIRPROD_RANGE_HPP__
#define __DIRPROD_RANGE_HPP__

#include <array>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>

#include "iterator.hpp"

namespace dirprod
{

template<class... Ranges>
class range
{
public:
    friend class range_iterator<Ranges...>;

    using iterator = range_iterator<Ranges...>;
    using const_iterator = iterator;
    using iterator_type = typename iterator::iterator_type;
    using value_type = typename iterator::value_type;
    using size_type = size_t;
    using difference_type = typename iterator::difference_type;

    range(Ranges&&... ranges):
        ranges_(std::forward<Ranges>(ranges)...),
        first_(std::apply([](auto&... rngs) { return iterator_type(boost::begin(rngs)...); }, ranges_)),
        last_(std::apply([](auto&... rngs) { return iterator_type(boost::end(rngs)...); }, ranges_)),
        dists_(std::apply([](auto&... rng) { return decltype(dists_){static_cast<typename iterator::difference_type>(boost::size(rng))...}; }, ranges_))
    {}

    auto begin()
    {
        return iterator(*this);
    }

    auto end()
    {
        auto it = begin();
        std::get<N - 1>(it.iterators_) = std::get<N - 1>(last_);
        return it;
    }

    auto begin() const
    {
        return const_iterator(*this);
    }

    auto end() const
    {
        auto it = begin();
        std::get<N - 1>(it.iterators_) = std::get<N - 1>(last_);
        return it;
    }

private:
    static const std::size_t N = sizeof...(Ranges);

    std::tuple<Ranges...> ranges_;
    iterator_type first_;
    iterator_type last_;
    std::array<typename iterator::difference_type, N> dists_;
};

template<class... Ranges>
range(Ranges&&...) -> range<Ranges...>;

// One element specialization.
template<class R>
class range<R>
{
public:
    range(R&& rng): range_(std::forward<R>(rng))
    {}

    auto begin()
    {
        return boost::begin(range_);
    }

    auto end()
    {
        return boost::end(range_);
    }

private:
    R range_;
};

} // namespace dirprod

#endif // __DIRPROD_RANGE_HPP__
