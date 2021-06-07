#pragma once

#include "meteor\core\base.h"

#include <glm\glm.hpp>

/// <summary>
/// Noise algorithm from Hopson97's terrain generation
/// https://github.com/Hopson97/open-builder/blob/master/src/server/world/terrain_generation.cpp
/// </summary>

namespace meteor
{
    class Noise
    {
    public:
        Noise(mtrInt seed, mtrInt octaves, mtrFloat amplitude, mtrFloat smoothness, mtrFloat roughness, mtrFloat offset);

        mtrFloat            GetNoise(mtrFloat blockGlobalX, mtrFloat blockGlobalY);

        inline void         SetSeed(mtrInt seed) { m_Seed = seed; }
        inline void         SetOctaves(mtrInt octaves) { m_Octaves = octaves; }
        inline void         SetAmplitude(mtrFloat amplitude) { m_Amplitude = amplitude; }
        inline void         SetSmoothness(mtrFloat smoothness) { m_Smoothness = smoothness; }
        inline void         SetRoughness(mtrFloat roughness) { m_Roughness = roughness; }
        inline void         SetOffset(mtrFloat offset) { m_Offest = offset; }

        inline mtrInt       GetSeed() const { return m_Seed; }
        inline mtrInt       GetOctaves() const { return m_Octaves; }
        inline mtrFloat     GetAmplitude() const { return m_Amplitude; }
        inline mtrFloat     GetSmoothness() const { return m_Smoothness; }
        inline mtrFloat     GetRoughness() const { return m_Roughness; }
        inline mtrFloat     GetOffset() const { return m_Offest; }
    private:
        mtrInt              m_Seed;
        mtrInt              m_Octaves;
        mtrFloat            m_Amplitude;
        mtrFloat            m_Smoothness;
        mtrFloat            m_Roughness;
        mtrFloat            m_Offest;
    };
}