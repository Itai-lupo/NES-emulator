#pragma once
#include <cstdint>

namespace LaughTaleEngine
{
    typedef uint32_t eventLaughId;
    typedef uint32_t entityTaleId;
    typedef uint32_t vertexBufferId;
    typedef uint32_t indexBufferId;
    typedef uint32_t vertexArrayId;
    typedef uint32_t shaderId;
    typedef uint64_t windowPieceId;
    typedef uint32_t apoEnvelopeId;

    enum renderAPI
    {
        None = 0, OpenGL = 1
    };

    enum sondWaves
    {
        mute = 0,
        sin,
        sowTooth,
        analogSowTooth,
        squre,
        analogSqure,
        triangels,
        noise,
        speaker
    };
}