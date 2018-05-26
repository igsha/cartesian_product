#ifndef __DIRECT_PRODUCT_RANGE_HPP__
#define __DIRECT_PRODUCT_RANGE_HPP__

#include "iterator.hpp"

namespace direct_product {

template<class... Ranges>
class range
{
public:
    using iterator_type = iterator<decltype(std::declval<Ranges>().begin())...>;

    range(Ranges&... ranges): first_iterator_({ranges.begin(), ranges.end()}...)
    {}

    iterator_type begin() const
    {
        return first_iterator_;
    }

    iterator_type end() const
    {
        return iterator_type::create_end_iterator(first_iterator_);
    }

private:
    iterator_type first_iterator_;
};

template<class... Ranges>
range<Ranges...> make_range(Ranges&... ranges)
{
    return range<Ranges...>(ranges...);
}

} // namespace direct_product

#endif // __DIRECT_PRODUCT_RANGE_HPP__
