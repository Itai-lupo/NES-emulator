#include <gtest/gtest.h>

#include <vector> 
#include "LaughTaleEngine.h"

using namespace LaughTaleEngine;

struct testEventData: public IEventData
{
    public:
        testEventData(int id):id(id){}
        int id;
};

struct testEntity: public IEntity
{
    int x;
};

TEST(initEngine, InitAndCloseEngineWork)
{
    try
    {
        app::init();
    }
    catch(const std::exception& e)
    {
        FAIL() << "init " << e.what() << "\n";
    }

    try
    {
        app::close();
    }
    catch(const std::exception& e)
    {
        FAIL() << "close " << e.what() << "\n";
    }
}

TEST(eventSystem, addAndCallEvent)
{

    try
    {
        eventCallbackFunc callback = [](IEntity *eventEntity, IEventData *sendor){
            EXPECT_EQ(0, eventEntity->id) << "wrong entity id";
            EXPECT_EQ(1, dynamic_cast<testEventData*>(sendor)->id) << "wrong event id";
            dynamic_cast<testEntity*>(eventEntity)->x = 10;
        };
        
        app::init();
        testEntity testE;
        entityTaleId id = entityManger::addEntity(&testE);
        eventManger::addEvent(events::manualEvent, callback, id);

        LAUGHTALE_ENGINR_LOG_INFO("event added sucssfly");

        testEventData *testEvent = new testEventData(1);
        eventManger::trigerEvent(events::manualEvent, testEvent);
        EXPECT_EQ(10, dynamic_cast<testEntity*>(&testE)->x) << "wrong event id";

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
            dynamic_cast<testEntity*>(eventEntity)->x++;
        };
        
        app::init();
        IEntity *testE = new testEntity();
        // testE->x = 0;
        // testE->id = 0;
        entityTaleId id =  entityManger::addEntity(testE);
        eventManger::addEvent(events::manualEvent, callback, id);
        eventManger::addEvent(events::manualEvent, callback, id);
        eventManger::addEvent(events::manualEvent, callback, id);

        LAUGHTALE_ENGINR_LOG_INFO(
            "event added sucssfly: " + 
            std::to_string(id));

        testEventData *testEvent = new testEventData(1);

        eventManger::trigerEvent(events::manualEvent, testEvent);
        EXPECT_EQ(3, dynamic_cast<testEntity *>(testE)->x) << "entity didnt change";

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
            testEntity *t = dynamic_cast<testEntity*>(eventEntity);

            EXPECT_EQ(0, eventEntity->id) << "wrong entity id on 1";
            EXPECT_EQ(1,  dynamic_cast<testEventData*>(sendor)->id) << "wrong event id on 1";
            t->x++;
        };
        
        eventCallbackFunc callback2 = [](IEntity *eventEntity, IEventData *sendor){
            testEntity *t = dynamic_cast<testEntity*>(eventEntity);
            EXPECT_EQ(1, eventEntity->id) << "wrong entity id on 2";
            EXPECT_EQ(1,  dynamic_cast<testEventData*>(sendor)->id) << "wrong event id on 2";
            t->x++;
        };

        eventCallbackFunc callback3 = [](IEntity *eventEntity, IEventData *sendor){
            testEntity *t = dynamic_cast <testEntity*>(eventEntity);
            EXPECT_EQ(2, eventEntity->id) << "wrong entity id on 3";
            EXPECT_EQ(1, dynamic_cast<testEventData*>(sendor)->id) << "wrong event id pn 3";
            t->x++;
        };

        app::init();

        IEntity *testEntity1 = new testEntity();
        IEntity *testEntity2 = new testEntity();
        IEntity *testEntity3 = new testEntity();

        entityTaleId id =  entityManger::addEntity(testEntity1);
        entityTaleId id2 =  entityManger::addEntity(testEntity2);
        entityTaleId id3 =  entityManger::addEntity(testEntity3);
        
        eventManger::addEvent(events::manualEvent, callback, id);
        eventManger::addEvent(events::manualEvent, callback, id);
        eventManger::addEvent(events::manualEvent, callback, id);
        eventManger::addEvent(events::manualEvent, callback2, id2);
        eventManger::addEvent(events::manualEvent, callback2, id2);
        eventManger::addEvent(events::manualEvent, callback2, id2);
        eventManger::addEvent(events::manualEvent, callback3, id3);
        eventManger::addEvent(events::manualEvent, callback3, id3);
        eventManger::addEvent(events::manualEvent, callback3, id3);

    
        LAUGHTALE_ENGINR_LOG_INFO("event added sucssfly: " + std::to_string(id));

        testEventData *testEvent = new testEventData(1);
        eventManger::trigerEvent(events::manualEvent, testEvent);
        
        EXPECT_EQ(3, dynamic_cast<testEntity*>(testEntity1)->x) << "entity didnt change on 1";
        EXPECT_EQ(3, dynamic_cast<testEntity*>(testEntity2)->x) << "entity didnt change on 2";
        EXPECT_EQ(3, dynamic_cast<testEntity*>(testEntity3)->x) << "entity didnt change on 3";

        app::close();
    }
    catch(const std::exception& e)
    {
        FAIL() << e.what() << "\n";
    }
}

