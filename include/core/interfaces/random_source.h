#ifndef CORE_INTERFACES_RANDOM_SOURCE_H
#define CORE_INTERFACES_RANDOM_SOURCE_H

#include <cstdint>

//! @brief Interface for a random number generator
//! @note Matching the Arduino Random library
class RandomSource
{
    public:
    //! @brief Seed the random number generator
    //! @param seed Seed to use for the random number generator
    //! @note This is not the same as the Arduino randomSeed() function, which seeds the random number generator with a random value
    //! @note The Arduino library implementation does nothing with a 0 value
    virtual void randomSeed(uint32_t seed) = 0;

    //! @brief Generate a random number
    //! @param min Minimum value, inclusive
    //! @param max Maximum value, exclusive
    virtual int32_t random(int32_t min, int32_t max) = 0;
    //! @brief Generate a random number
    //! @param max Maximum value, exclusive
    //! @note This is equivalent to random(0, max)
    virtual int32_t random(int32_t max) = 0;
    virtual ~RandomSource() = default;
    protected:
    RandomSource() = default;
};


#endif // CORE_INTERFACES_RANDOM_SOURCE_H

