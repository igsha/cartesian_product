#ifndef __DIRPROD_ITERATOR_HPP__
#define __DIRPROD_ITERATOR_HPP__

#include <iterator>
#include <tuple>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/begin.hpp>

namespace dirprod::detail
{

template<class Range>
using range_to_iterator_t = decltype(boost::begin(std::declval<Range>())); // iterator or const_iterator

template<class Range>
using range_to_value_t = typename std::iterator_traits<range_to_iterator_t<Range>>::value_type;

template<class Range>
using range_to_reference_t = typename std::iterator_traits<range_to_iterator_t<Range>>::reference;

} // namespace detail::dirprod

namespace dirprod
{

template<class... Ranges>
class range;

template<class... Ranges>
class range_iterator:
    public boost::iterator_facade<
        range_iterator<Ranges...>,
        std::tuple<detail::range_to_value_t<Ranges>...>,
        std::random_access_iterator_tag, // TODO: deduce tag
        std::tuple< detail::range_to_reference_t<Ranges>...>
    >
{
public:
    using iterator_type = std::tuple<detail::range_to_iterator_t<Ranges>...>;

    range_iterator(const range<Ranges...>& rng): parent_(&rng), iterators_(parent_->first_)
    {}

private:
    friend class boost::iterator_core_access;
    template<class...> friend class range;

    auto dereference() const
    {
        return std::apply([](auto&&... xs) { return typename range_iterator::reference(*xs...); }, iterators_);
    }

    void increment()
    {
        increment<0>();
    }

    bool equal(const range_iterator& x) const
    {
        return iterators_ == x.iterators_;
    }

    void advance(typename range_iterator::difference_type n)
    {
        increment<0>(n);
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
            increment<Index + 1>();
        }
        else
        {
            ++std::get<Index>(iterators_);
        }
    }

    template<std::size_t Index>
    void increment(typename range_iterator::difference_type n)
    {
        if constexpr(Index < sizeof...(Ranges) - 1)
        {
            auto& it = std::get<Index>(iterators_);
            const auto& first = std::get<Index>(parent_->first_);
            n += std::distance(first, it);
            it = first + n % parent_->dists_[Index];
            n /= parent_->dists_[Index];

            if (n != 0)
                increment<Index + 1>(n);
        }
        else
        {
            std::get<Index>(iterators_) += n;
        }
    }


private:
    const range<Ranges...>* parent_;
    iterator_type iterators_;
};

} // namespace dirprod

#endif // __DIRPROD_ITERATOR_HPP__
