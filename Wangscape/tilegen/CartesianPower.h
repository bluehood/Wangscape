#pragma once
#include "CartesianPowerIterator.h"
#include <utility>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include "ipow.h"

namespace tilegen
{

template<typename ForwardIt>
class CartesianPower
{
public:
    static_assert(std::is_base_of<std::forward_iterator_tag,
                                  typename std::iterator_traits<ForwardIt>::iterator_category>::value,
                  "ForwardIt must be at least a ForwardIterator");
    typedef CartesianPowerIterator<ForwardIt> const_iterator;
    typedef const_iterator iterator;

    CartesianPower(ForwardIt first_, size_t base_size_, size_t power_);
    template<typename Container>
    CartesianPower(const Container& container, size_t power_);

    const_iterator cbegin() const;
    const_iterator cend() const;
    iterator begin() const;
    iterator end() const;

    size_t size() const;
    std::pair<size_t, size_t> size2D(sf::Vector2u resolution) const;

    ForwardIt first;
    size_t baseSize;
    size_t power;
};

template<typename ForwardIt>
inline CartesianPower<ForwardIt>::CartesianPower(ForwardIt first_, size_t base_size_, size_t power_) :
    first(first_),
    baseSize(base_size_),
    power(power_)
{
    switch (power_)
    {
    case static_cast<size_t>(Corners::Triangle) :
    case static_cast<size_t>(Corners::Square) :
    case static_cast<size_t>(Corners::Hexagon) :
        break;
    default:
        std::cerr << "Warning: CartesianPower constructed with power not 3, 4, or 6";
    }
}

template<typename ForwardIt>
template<typename Container>
inline CartesianPower<ForwardIt>::CartesianPower(const Container & container_, size_t power_) :
    CartesianPower(container_.cbegin(), container_.size(), power_)
{
    static_assert(std::is_same<typename Container::const_iterator,
                               ForwardIt>::value,
                  "Container::const_iterator must be the same as ForwardIt");
}

template<typename ForwardIt>
typename CartesianPower<ForwardIt>::const_iterator CartesianPower<ForwardIt>::cbegin() const
{
    return const_iterator(first, baseSize, 0, power);
}

template<typename ForwardIt>
typename CartesianPower<ForwardIt>::const_iterator CartesianPower<ForwardIt>::cend() const
{
    return const_iterator(first, baseSize, baseSize, power);
}

template<typename ForwardIt>
typename CartesianPower<ForwardIt>::const_iterator CartesianPower<ForwardIt>::begin() const
{
    return cbegin();
}

template<typename ForwardIt>
typename CartesianPower<ForwardIt>::const_iterator CartesianPower<ForwardIt>::end() const
{
    return cend();
}

template<typename ForwardIt>
inline size_t CartesianPower<ForwardIt>::size() const
{
    return ipow(baseSize, power);
}

template<typename ForwardIt>
inline std::pair<size_t, size_t> CartesianPower<ForwardIt>::size2D(sf::Vector2u resolution) const
{
    std::div_t div_mod = div((int)power, 2);
    size_t tiles_y = ipow(baseSize, (size_t)div_mod.quot);
    size_t tiles_x = tiles_y;
    if (div_mod.rem != 0)
        tiles_x *= baseSize;
    return{resolution.x*tiles_x, resolution.y*tiles_y};
}

} // namespace tilegen
