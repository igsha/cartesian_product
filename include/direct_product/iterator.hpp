#ifndef __DIRECT_PRODUCT_ITERATOR_HPP__
#define __DIRECT_PRODUCT_ITERATOR_HPP__

#include <iterator>
#include <tuple>

#include <boost/iterator/iterator_facade.hpp>

namespace direct_product {

template<class... Iterators>
class iterator:
    public boost::iterator_facade<
        iterator<Iterators...>,
        std::tuple<typename std::iterator_traits<Iterators>::value_type...>,
        std::random_access_iterator_tag,
        std::tuple<typename std::iterator_traits<Iterators>::reference...>
    >
{
    static const std::size_t last_index_ = sizeof...(Iterators) - 1;
    using iterator_type = std::tuple<Iterators...>;

public:
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

private:
    friend class boost::iterator_core_access;

    auto dereference() const
    {
        return dereference(std::make_index_sequence<sizeof...(Iterators)>{});
    }

    void increment()
    {
        increment<0>();
    }

    bool equal(const iterator& x) const
    {
        return iterators_ == x.iterators_;
    }

    template<std::size_t... Indices>
    typename iterator::reference dereference(std::index_sequence<Indices...>) const
    {
        return {*(std::get<Indices>(iterators_))...};
    }

    template<std::size_t Index>
    void increment()
    {
        if constexpr(Index < last_index_)
        {
            auto& it = std::get<Index>(iterators_);
            ++it;

            if (it != std::get<Index>(lasts_iterators_))
                return;

            std::get<Index>(iterators_) = std::get<Index>(firsts_iterators_);
            increment<Index+1>();
        }
        else
        {
            ++std::get<Index>(iterators_);
        }
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
