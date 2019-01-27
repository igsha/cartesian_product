#ifndef __DIRECT_PRODUCT_RANGE_HPP__
#define __DIRECT_PRODUCT_RANGE_HPP__

#include <boost/range/iterator_range.hpp>

#include "iterator.hpp"

namespace direct_product {

template<class... Ranges>
class range
{
public:
    using iterator = direct_product::iterator<decltype(std::declval<Ranges>().begin())...>;
    using const_iterator = direct_product::iterator<decltype(std::declval<Ranges>().begin())...>;

    range(Ranges&&... ranges):
        ranges_(std::forward<Ranges>(ranges)...),
        first_iterator_(std::apply(make_iterator_from_range<Ranges...>, ranges_))
    {}

    iterator begin() const
    {
        return first_iterator_;
    }

    iterator end() const
    {
        return iterator::create_end_iterator(first_iterator_);
    }

private:
    std::tuple<Ranges...> ranges_;
    iterator first_iterator_;
};

template<class... Ranges>
range<Ranges...> make_range(Ranges&&... ranges)
{
    return range<Ranges...>(std::forward<Ranges>(ranges)...);
}

template<class... Ts>
auto make_range(std::initializer_list<Ts>&&... lsts)
{
    return range<std::initializer_list<Ts>...>(std::forward<std::initializer_list<Ts>>(lsts)...);
}

} // namespace direct_product

#endif // __DIRECT_PRODUCT_RANGE_HPP__
