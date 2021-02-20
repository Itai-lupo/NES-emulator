#include <gtest/gtest.h>

#include <vector> 
#include "raftelGraphicEngine.h"
#include "logger.h"

using namespace raftelGraphicEngine;

TEST(initEngine, InitAndCloseEngineWork)
{
    try
    {
        init();

        close();
    }
    catch(const std::exception& e)
    {
        FAIL() << e.what() << "\n";
    }
}

TEST(eventSystem, addAndCallEvent)
{

    try
    {
        std::function<void(IEntity eventEntity, IEventData sendor)> callback = [](IEntity eventEntity, IEventData sendor){
            EXPECT_EQ(10, eventEntity.getId()) << "wrong entity id";
            EXPECT_EQ(1, sendor.getId()) << "wrong event id";
            std::string msg("event trigerd at test addAndCallEvent");
            logger::LogInfo(msg);
        };
        
        init();
        eventManger::addEvent(events::manualEvent, callback, 10);

        IEventData testEvent(1);
        eventManger::trigerEvent(events::manualEvent, testEvent);
        close();
    }
    catch(const std::exception& e)
    {
        FAIL() << e.what() << "\n";
    }
}
