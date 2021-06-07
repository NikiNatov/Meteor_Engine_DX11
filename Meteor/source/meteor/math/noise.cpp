#include "pch.h"
#include "noise.h"

#include <glm\gtc\noise.hpp>

namespace meteor
{
    // --------------------------------------------------------------------------------------------------------------------------------------
    Noise::Noise(mtrInt seed, mtrInt octaves, mtrFloat amplitude, mtrFloat smoothness, mtrFloat roughness, mtrFloat offset)
        : m_Seed(seed), m_Octaves(octaves), m_Amplitude(amplitude), m_Smoothness(smoothness), m_Roughness(roughness), m_Offest(offset)
    {
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    mtrFloat Noise::GetNoise(mtrFloat blockGlobalX, mtrFloat blockGlobalY)
    {
        mtrFloat value = 0;
        mtrFloat accumulatedAmps = 0;

        for (int i = 0; i < m_Octaves; i++)
        {
            mtrFloat frequency = glm::pow(2.0f, i);
            mtrFloat amplitude = glm::pow(m_Roughness, i);

            mtrFloat x = blockGlobalX * frequency / m_Smoothness;
            mtrFloat y = blockGlobalY * frequency / m_Smoothness;

            mtrFloat noise = glm::simplex(glm::vec3{ m_Seed + x, m_Seed + y, m_Seed });
            noise = (noise + 1.0f) / 2.0f;
            value += noise * amplitude;
            accumulatedAmps += amplitude;
        }

        return value / accumulatedAmps;
    }
}
