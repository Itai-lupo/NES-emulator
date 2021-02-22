#include <gtest/gtest.h>

#include <vector> 
#include "raftelGraphicEngine.h"

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
        std::function<void(IEntity *eventEntity, IEventData sendor)> callback = [](IEntity *eventEntity, IEventData sendor){
            EXPECT_EQ(0, eventEntity->id) << "wrong entity id";
            EXPECT_EQ(1, sendor.getId()) << "wrong event id";
            eventEntity->x = 10;
            std::string msg("event trigerd at test addAndCallEvent");
            logger::LogInfo(msg);
        };
        
        init();
        IEntity testEntity;
        int id = entityManger::addEntity(&testEntity);
        eventManger::addEvent(events::manualEvent, callback, id);
        std::string msg("event added sucssfly");

        logger::LogInfo(msg);

        IEventData testEvent(1);
        eventManger::trigerEvent(events::manualEvent, testEvent);
        EXPECT_EQ(10, testEntity.x) << "wrong event id";

        close();
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
        std::function<void(IEntity *eventEntity, IEventData sendor)> callback = [](IEntity *eventEntity, IEventData sendor){
            EXPECT_EQ(0, eventEntity->id) << "wrong entity id";
            EXPECT_EQ(1, sendor.getId()) << "wrong event id";
            eventEntity->x++;
            std::string msg("event trigerd at test EventWithMoreThenOneCallback");
            logger::LogInfo(msg);
        };
        
        init();
        IEntity testEntity;
        int id =  entityManger::addEntity(&testEntity);
        eventManger::addEvent(events::manualEvent, callback, id);
        eventManger::addEvent(events::manualEvent, callback, id);
        eventManger::addEvent(events::manualEvent, callback, id);

        std::string msg("event added sucssfly: " + std::to_string(id));
    
        logger::LogInfo(msg);

        IEventData testEvent(1);
        eventManger::trigerEvent(events::manualEvent, testEvent);
        EXPECT_EQ(3, testEntity.x) << "entity didnt change";

        close();
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
        std::function<void(IEntity *eventEntity, IEventData sendor)> callback = [](IEntity *eventEntity, IEventData sendor){
            EXPECT_EQ(0, eventEntity->id) << "wrong entity id on 1";
            EXPECT_EQ(1, sendor.getId()) << "wrong event id on 1";
            eventEntity->x++;
            std::string msg("event trigerd at test fewEntityWithFewEvents on 1");
            logger::LogInfo(msg);
        };
        
        std::function<void(IEntity *eventEntity, IEventData sendor)> callback2 = [](IEntity *eventEntity, IEventData sendor){
            EXPECT_EQ(1, eventEntity->id) << "wrong entity id on 2";
            EXPECT_EQ(1, sendor.getId()) << "wrong event id on 2";
            eventEntity->x++;
            std::string msg("event trigerd at test fewEntityWithFewEvents on 2");
            logger::LogInfo(msg);
        };

        std::function<void(IEntity *eventEntity, IEventData sendor)> callback3 = [](IEntity *eventEntity, IEventData sendor){
            EXPECT_EQ(2, eventEntity->id) << "wrong entity id on 3";
            EXPECT_EQ(1, sendor.getId()) << "wrong event id pn 3";
            eventEntity->x++;
            std::string msg("event trigerd at test fewEntityWithFewEvents on 3");
            logger::LogInfo(msg);
        };

        init();
        IEntity testEntity;
        IEntity testEntity2;
        IEntity testEntity3;
        int id =  entityManger::addEntity(&testEntity);
        int id2 =  entityManger::addEntity(&testEntity2);
        int id3 =  entityManger::addEntity(&testEntity3);
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

        IEventData testEvent(1);
        eventManger::trigerEvent(events::manualEvent, testEvent);
        EXPECT_EQ(3, testEntity.x) << "entity didnt change on 1";
        EXPECT_EQ(3, testEntity2.x) << "entity didnt change on 2";
        EXPECT_EQ(3, testEntity3.x) << "entity didnt change on 3";

        close();
    }
    catch(const std::exception& e)
    {
        FAIL() << e.what() << "\n";
    }
}

