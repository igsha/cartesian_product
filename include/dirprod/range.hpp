#ifndef __DIRPROD_RANGE_HPP__
#define __DIRPROD_RANGE_HPP__

#include <iterator>
#include <tuple>

#include <boost/iterator/iterator_facade.hpp>

namespace dirprod::detail
{

template<class Range>
using range_to_iterator_t = typename std::decay_t<Range>::iterator;

template<class Range>
using range_to_value_t = typename std::iterator_traits<range_to_iterator_t<Range>>::value_type;

template<class Range>
using range_to_reference_t = typename std::iterator_traits<range_to_iterator_t<Range>>::reference;

} // namespace dirprod::detail

namespace dirprod
{

template<class... Ranges>
class range
{
    static const std::size_t last_index_ = sizeof...(Ranges) - 1;

public:
    class iterator:
        public boost::iterator_facade<
            iterator,
            std::tuple<detail::range_to_value_t<Ranges>...>,
            std::random_access_iterator_tag,
            std::tuple< detail::range_to_reference_t<Ranges>...>
        >
    {
    public:
        using iterator_type = std::tuple<detail::range_to_iterator_t<Ranges>...>;

        iterator(range& rng): parent_(&rng), iterators_(parent_->first_)
        {}

    private:
        friend class boost::iterator_core_access;
        friend class range;

        auto dereference() const
        {
            return std::apply([](auto&&... xs) { return typename iterator::reference(*xs...); }, iterators_);
        }

        void increment()
        {
            increment<0>();
        }

        bool equal(const iterator& x) const
        {
            return iterators_ == x.iterators_;
        }

        template<std::size_t Index>
        void increment()
        {
            if constexpr(Index < sizeof...(Ranges) - 1)
            {
                auto& it = std::get<Index>(iterators_);
                ++it;

                if (it != std::get<Index>(parent_->last_))
                    return;

                std::get<Index>(iterators_) = std::get<Index>(parent_->first_);
                increment<Index+1>();
            }
            else
            {
                ++std::get<Index>(iterators_);
            }
        }

    private:
        range* parent_;
        iterator_type iterators_;
    };

    using iterator_type = typename iterator::iterator_type;

    range(Ranges&&... ranges):
        ranges_(std::forward<Ranges>(ranges)...),
        first_(std::apply([](auto&... rngs) { return iterator_type(rngs.begin()...); }, ranges_)),
        last_(std::apply([](auto&... rngs) { return iterator_type(rngs.end()...); }, ranges_))
    {}

    auto begin()
    {
        return iterator(*this);
    }

    auto end()
    {
        auto it = begin();
        std::get<last_index_>(it.iterators_) = std::get<last_index_>(last_);
        return it;
    }

private:
    std::tuple<Ranges...> ranges_;
    iterator_type first_;
    iterator_type last_;
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
        return range_.begin();
    }

    auto end()
    {
        return range_.end();
    }

private:
    R range_;
};

} // namespace dirprod

#endif // __DIRPROD_RANGE_HPP__
