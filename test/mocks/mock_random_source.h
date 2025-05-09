#ifndef TEST_MOCKS_MOCK_RANDOM_H
#define TEST_MOCKS_MOCK_RANDOM_H

#include "core/interfaces/random_source.h"
#include <cstdint>
#include <map>
#include <random>

class MockRandom : public RandomSource
{
    public:
    MockRandom()
    {
        m_seed = 0;
        m_gen = std::mt19937(m_seed);
        m_dist = std::uniform_int_distribution<>(0, 1);
    }
    void randomSeed(uint32_t seed) override 
    { 
        m_seed = seed;
        m_gen = std::mt19937(m_seed);
        m_dist = std::uniform_int_distribution<>(0, 1);
    }
    int32_t random(int32_t min, int32_t max) override {
        // If we have fake random numbers, return the next one in the sequence and pop it from the vector
        if (m_fake_random_numbers.size() > 0) {
            auto it = m_fake_random_numbers.begin();
            int32_t val = *it;
            m_fake_random_numbers.erase(it);
            return val;
        }
        // Otherwise we generate a bespoke bona fide random number using our engine
        m_dist = std::uniform_int_distribution<>(min, max);
        return m_dist(m_gen);
    }
    int32_t random(int32_t max) override {
        return random(0, max);
    }
    

    //! @brief "Seed" the random number generator with a map of fake random numbers at a given seed
    //! @note Remember to call randomSeed() with the same seed before calling this function
    //! @note The returned numbers will not match min and max in random(), but only be the ones you set in the map
    //! @param fake_random_numbers Map of fake random numbers
    //! @param seed Seed to use for the random number generator
    void set_fake_random_numbers(const std::vector<int32_t>& fake_random_numbers) {
        m_fake_random_numbers = fake_random_numbers;
    }


    private:
    std::vector<int32_t> m_fake_random_numbers;
    std::mt19937 m_gen;
    std::uniform_int_distribution<> m_dist;
    uint32_t m_seed;

    //! @brief Delete the copy constructor and assignment operator
    //! @note Copying a mt19937 engine is not something you want to do
    MockRandom(const MockRandom& other) = delete;
    MockRandom& operator=(const MockRandom& other) = delete;
};

#endif // TEST_MOCKS_MOCK_RANDOM_H
