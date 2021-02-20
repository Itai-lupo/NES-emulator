#include <gtest/gtest.h>
#include "raftelGraphicEngine.h"

TEST(initEngine, InitAndCloseEngineWork)
{
    try
    {
        raftelGraphicEngine::init();

        raftelGraphicEngine::close();
    }
    catch(const std::exception& e)
    {
        FAIL() << e.what() << "\n";
    }
}
