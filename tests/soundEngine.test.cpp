#include "LaughTaleEngine.h"
#include <gtest/gtest.h>
#include <string.h>

using namespace LaughTaleEngine;

void beep(IEntity *eventEntity, IEventData *sendor)
{
    SoundsampleData *data = static_cast<SoundsampleData *>(sendor);
    data->amp = sin(data->time);
}

TEST(SoundEngine, soundInterface)
{
    app::init();
    eventManger::addEvent(events::onSoundsample, beep);
    app::run();
    app::close();
}