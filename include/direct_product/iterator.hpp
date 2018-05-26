#ifndef __DIRECT_PRODUCT_ITERATOR_HPP__
#define __DIRECT_PRODUCT_ITERATOR_HPP__

#include <iterator>
#include <tuple>
#include <boost/operators.hpp>

namespace direct_product {

template<class... Iterators>
class iterator:
    public boost::equality_comparable<iterator<Iterators...>>,
    public boost::incrementable<iterator<Iterators...>>
{
    static const std::size_t last_index_ = sizeof...(Iterators) - 1;
    using iterator_type = std::tuple<Iterators...>;

public:
    using iterator_category = std::forward_iterator_tag; // min of categories?
    using value_type = std::tuple<typename std::iterator_traits<Iterators>::value_type...>;
    using difference_type = std::tuple<typename std::iterator_traits<Iterators>::difference_type...>;
    using pointer = std::tuple<typename std::iterator_traits<Iterators>::pointer...>; // ?
    using reference = std::tuple<typename std::iterator_traits<Iterators>::reference...>;

    iterator(const std::tuple<Iterators, Iterators>&... iterators):
        firsts_iterators_(std::get<0>(iterators)...),
        lasts_iterators_(std::get<1>(iterators)...),
        iterators_(firsts_iterators_)
    {}

    static iterator create_end_iterator(const iterator& first_iterator)
    {
        iterator it(first_iterator);
        std::get<last_index_>(it.iterators_) = std::get<last_index_>(it.lasts_iterators_);
        return it;
    }

    reference operator*() const
    {
        return dereference(std::make_index_sequence<sizeof...(Iterators)>());
    }

    iterator& operator++()
    {
        return increment<0>();
    }

    bool operator == (const iterator& other) const
    {
        value_type t;
        return iterators_ == other.iterators_;
    }

private:
    template<std::size_t... Indices>
    reference dereference(std::index_sequence<Indices...>) const
    {
        return {*(std::get<Indices>(iterators_))...};
    }

    template<std::size_t Index, typename std::enable_if_t<Index < last_index_, int> = 0>
    iterator& increment()
    {
        auto& it = std::get<Index>(iterators_);
        ++it;

        if (it != std::get<Index>(lasts_iterators_))
            return *this;

        std::get<Index>(iterators_) = std::get<Index>(firsts_iterators_);
        return increment<Index+1>();
    }

    template<std::size_t Index, typename std::enable_if_t<Index == last_index_, int> = 0>
    iterator& increment()
    {
        ++std::get<Index>(iterators_);
        return *this;
    }

    iterator_type firsts_iterators_;
    iterator_type lasts_iterators_;
    iterator_type iterators_;
};

template<class... Iterators>
iterator<Iterators...> make_iterator(const std::tuple<Iterators, Iterators>&... its)
{
    return iterator<Iterators...>(its...);
}

template<class... Ranges>
iterator<decltype(std::declval<Ranges>().begin())...> make_iterator_from_range(Ranges&... rngs)
{
    return iterator<decltype(std::declval<Ranges>().begin())...>({rngs.begin(), rngs.end()}...);
}

} // namespace direct_product

#endif // __DIRECT_PRODUCT_ITERATOR_HPP__
