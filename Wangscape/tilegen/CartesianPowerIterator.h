#pragma once
#include <vector>
#include <iterator>
#include <type_traits>
#include <algorithm>
#include <utility>
#include "common.h"
#include "CoordinatePacker.h"

namespace tilegen
{
template<typename InputIt>
class CartesianPowerIterator : public std::iterator<std::input_iterator_tag,
                                                    std::vector<typename InputIt::value_type>>
{
public:
    static_assert(std::is_base_of<std::input_iterator_tag,
                                  typename std::iterator_traits<InputIt>::iterator_category>::value,
                  "InputIt must be at least an InputIterator");
    typedef std::vector<typename InputIt::value_type> Values;
    typedef std::vector<InputIt> Iterators;

    CartesianPowerIterator(InputIt first_, InputIt last_, InputIt init, size_t power);
    CartesianPowerIterator& operator++();

    bool operator==(const CartesianPowerIterator& other) const;
    bool operator!=(const CartesianPowerIterator& other) const;

    const Values& operator*() const;
    const typename InputIt::value_type& get(size_t n) const;
    const Iterators& getIterators() const;
    const InputIt& getIterator(size_t n) const;
    const typename InputIt::difference_type coordinate(size_t n) const;
    const InputIt& getFirst() const;
    const InputIt& getLast() const;
    std::pair<size_t, size_t> coordinates_2d() const;
private:
    const InputIt mFirst;
    const InputIt mLast;
    std::vector<InputIt> mIterators;
    std::vector<typename InputIt::value_type> mValues;
};

template<typename InputIt>
inline CartesianPowerIterator<InputIt>::CartesianPowerIterator(InputIt first, InputIt last, InputIt init, size_t power) :
    mFirst(first), mLast(last),
    mIterators(power, init)
{
    if (init != last)
        for (const auto& it : mIterators)
            mValues.push_back(*it);
}

template<typename InputIt>
inline CartesianPowerIterator<InputIt>& CartesianPowerIterator<typename InputIt>::operator++()
{
    for (size_t i = 0; i < mIterators.size(); i++)
    {
        ++mIterators[i];
        if (i < mIterators.size() - 1)
        {
            if (mIterators[i] == getLast())
            {
                mIterators[i] = getFirst();
                mValues[i] = *mIterators[i];
            }
            else
            {
                mValues[i] = *mIterators[i];
                break;
            }
        }
        else if (mIterators[i] != getLast())
        {
            mValues[i] = *mIterators[i];
        }
    }
    return *this;
}

template<typename InputIt>
inline bool CartesianPowerIterator<InputIt>::operator==(const CartesianPowerIterator& other) const
{
    if (getFirst() != other.getFirst())
        return false; // different ranges
    if (getLast() != other.getLast())
        return false; // different ranges
    if (getIterators().size() != other.getIterators().size())
        return false; // different powers
    if (*getIterators().crbegin() == getLast() &&
        *other.getIterators().crbegin() == getLast())
        return true; // both are cend
    for (size_t i = 0; i < getIterators().size(); i++)
        if (getIterator(i) != other.getIterator(i))
            return false; // different phases
    return true;
}

template<typename InputIt>
inline bool CartesianPowerIterator<InputIt>::operator!=(const CartesianPowerIterator& other) const
{
    return !(*this == other);
}

template<typename InputIt>
inline const typename CartesianPowerIterator<InputIt>::Values& CartesianPowerIterator<InputIt>::operator*() const
{
    return mValues;
}

template<typename InputIt>
inline const typename InputIt::value_type& CartesianPowerIterator<InputIt>::get(size_t n) const
{
    return mValues[n];
}

template<typename InputIt>
inline const InputIt& CartesianPowerIterator<InputIt>::getIterator(size_t n) const
{
    return mIterators[n];
}

template<typename InputIt>
inline const typename CartesianPowerIterator<InputIt>::Iterators& CartesianPowerIterator<InputIt>::getIterators() const
{
    return mIterators;
}

template<typename InputIt>
inline const typename InputIt::difference_type CartesianPowerIterator<InputIt>::coordinate(size_t n) const
{
    return std::distance(getFirst(), mIterators[n]);
}

template<typename InputIt>
inline const InputIt& CartesianPowerIterator<InputIt>::getFirst() const
{
    return mFirst;
}

template<typename InputIt>
inline const InputIt& CartesianPowerIterator<InputIt>::getLast() const
{
    return mLast;
}

template<typename InputIt>
inline std::pair<size_t, size_t> CartesianPowerIterator<InputIt>::coordinates_2d() const
{
    size_t clique_size = std::distance(getFirst(), getLast());
    CoordinatePacker<size_t> x(clique_size);
    for (size_t i = 0; i < static_cast<size_t>(CORNERS); i += 2)
    {
        x.addCoordinate(coordinate(i));
    }
    CoordinatePacker<size_t> y(clique_size);
    for (size_t i = 1; i < static_cast<size_t>(CORNERS); i += 2)
    {
        y.addCoordinate(coordinate(i));
    }

    return{x.packed(), y.packed()};
}

} // namespace tilegen
