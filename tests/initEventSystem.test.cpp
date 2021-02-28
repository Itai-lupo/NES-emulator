#include <gtest/gtest.h>

#include <vector> 
#include "raftelGraphicEngine.h"

using namespace raftelGraphicEngine;

struct testEventData: public IEventData
{
    public:
        testEventData(int id):id(id){}
        int id;
};

TEST(initEngine, InitAndCloseEngineWork)
{
    try
    {
        app::init();

        app::close();
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
        eventCallbackFunc callback = [](IEntity *eventEntity, IEventData *sendor){
            EXPECT_EQ(0, eventEntity->id) << "wrong entity id";
            EXPECT_EQ(1, dynamic_cast<testEventData*>(sendor)->id) << "wrong event id";
            eventEntity->x = 10;
        };
        
        app::init();
        IEntity testEntity;
        raftelId id = entityManger::addEntity(&testEntity);
        eventManger::addEvent(events::manualEvent, callback, id);
        std::string msg("event added sucssfly");

        logger::LogInfo(msg);

        testEventData *testEvent = new testEventData(1);
        eventManger::trigerEvent(events::manualEvent, testEvent);
        EXPECT_EQ(10, testEntity.x) << "wrong event id";

        app::close();
    }
    catch(const std::exception& e)
    {
        FAIL() << e.what() << "\n";
    }
}

TEST(eventSystem, EventWithMoreThenOneCallback)
{

    try
    {
        eventCallbackFunc callback = [](IEntity *eventEntity, IEventData *sendor){
            EXPECT_EQ(0, eventEntity->id) << "wrong entity id";
            EXPECT_EQ(1, dynamic_cast<testEventData*>(sendor)->id) << "wrong event id";
            eventEntity->x++;
        };
        
        app::init();
        IEntity testEntity;
        raftelId id =  entityManger::addEntity(&testEntity);
        eventManger::addEvent(events::manualEvent, callback, id);
        eventManger::addEvent(events::manualEvent, callback, id);
        eventManger::addEvent(events::manualEvent, callback, id);

        std::string msg("event added sucssfly: " + std::to_string(id));
    
        logger::LogInfo(msg);

        testEventData *testEvent = new testEventData(1);

        eventManger::trigerEvent(events::manualEvent, testEvent);
        EXPECT_EQ(3, testEntity.x) << "entity didnt change";

        app::close();
    }
    catch(const std::exception& e)
    {
        FAIL() << e.what() << "\n";
    }
}



TEST(eventSystem, fewEntityWithFewEvents)
{

    try
    {
        eventCallbackFunc callback = [](IEntity *eventEntity, IEventData *sendor){
            EXPECT_EQ(0, eventEntity->id) << "wrong entity id on 1";
            EXPECT_EQ(1,  dynamic_cast<testEventData*>(sendor)->id) << "wrong event id on 1";
            eventEntity->x++;
        };
        
        eventCallbackFunc callback2 = [](IEntity *eventEntity, IEventData *sendor){
            EXPECT_EQ(1, eventEntity->id) << "wrong entity id on 2";
            EXPECT_EQ(1,  dynamic_cast<testEventData*>(sendor)->id) << "wrong event id on 2";
            eventEntity->x++;
        };

        eventCallbackFunc callback3 = [](IEntity *eventEntity, IEventData *sendor){
            EXPECT_EQ(2, eventEntity->id) << "wrong entity id on 3";
            EXPECT_EQ(1, dynamic_cast<testEventData*>(sendor)->id) << "wrong event id pn 3";
            eventEntity->x++;
        };

        app::init();

        IEntity testEntity;
        IEntity testEntity2;
        IEntity testEntity3;

        raftelId id =  entityManger::addEntity(&testEntity);
        raftelId id2 =  entityManger::addEntity(&testEntity2);
        raftelId id3 =  entityManger::addEntity(&testEntity3);
        
        eventManger::addEvent(events::manualEvent, callback, id);
        eventManger::addEvent(events::manualEvent, callback, id);
        eventManger::addEvent(events::manualEvent, callback, id);
        eventManger::addEvent(events::manualEvent, callback2, id2);
        eventManger::addEvent(events::manualEvent, callback2, id2);
        eventManger::addEvent(events::manualEvent, callback2, id2);
        eventManger::addEvent(events::manualEvent, callback3, id3);
        eventManger::addEvent(events::manualEvent, callback3, id3);
        eventManger::addEvent(events::manualEvent, callback3, id3);

        std::string msg("event added sucssfly: " + std::to_string(id));
    
        logger::LogInfo(msg);

        testEventData *testEvent = new testEventData(1);
        eventManger::trigerEvent(events::manualEvent, testEvent);
        
        EXPECT_EQ(3, testEntity.x) << "entity didnt change on 1";
        EXPECT_EQ(3, testEntity2.x) << "entity didnt change on 2";
        EXPECT_EQ(3, testEntity3.x) << "entity didnt change on 3";

        app::close();
    }
    catch(const std::exception& e)
    {
        FAIL() << e.what() << "\n";
    }
}

