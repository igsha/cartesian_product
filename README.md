# dirprod

Implements cartesian product (direct product) to reduce for-loops in programs

```cxx
#include <dirprod/range.hpp>

enum Modes { DC, PLANAR, HORIZONTAL, VERTICAL, ANGLE45, MODES_SIZE };
enum Components { Y, Cb, Cr, COMPONENTS_SIZE };

// simple approach
for (int boolean = 0; boolean < 2; ++boolean)
{
    for (int mode = DC; mode < MODES_SIZE; ++mode)
    {
        for (int comp = Y; comp < COMPONENTS_SIZE; ++comp)
        {
            auto choice = std::make_tuple(Components(comp), bool(boolean), Modes(mode));
            // process choice
        }
    }
}

// using cartesian product
auto modes = boost::counting_range(int(DC), int(MODES_SIZE)) | boost::adaptors::transformed(+[](int x)
{
    return static_cast<Modes>(x);
});
std::list<bool> boolean{{false, true}};

auto enumeration = dirprod::range(std::vector{Y, Cb, Cr}, boolean, modes);
// iterate through all possible combinations
for (const auto& [comp, b, mode] : enumeration)
{
    // process choice
}
```
