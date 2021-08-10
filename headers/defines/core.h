#pragma once
#include <cstdint>

namespace LTE
{
    typedef uint32_t eventLaughId;
    typedef uint32_t entityTaleId;
    typedef uint64_t windowPieceId;
    typedef uint32_t apoEnvelopeId;
    typedef uint32_t materialId;
    typedef uint32_t textureId;
    typedef uint32_t packetId;
    typedef uint32_t connectionId;

    enum renderAPI
    {
       OpenGL
    };

    
    enum cameraControler
    {
        orthographicCameraControlerType
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