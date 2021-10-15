#include "LaughTaleEngine.h"
#include <gtest/gtest.h>
#include "guitar.h"
#include "bell.h"

TEST(soundModule, basicSondTest)
{
    return;
    LTE::app::init();

    LTE::entityManger::addEntity([](LTE::gameObject::gameObjectBuilder *builder)
    {
        builder->setObjectName("soundTest");
    });


    LTE::envelope *e = LTE::entityManger::getEntityByName("soundTest")->getComponent<LTE::envelope>();
    
    e->noteOn();
    // e->end();

    // LTE::guitar *test = new LTE::guitar();
    // LTE::bellRing *b = new LTE::bellRing();
    LTE::app::run();
    LTE::app::close();
}