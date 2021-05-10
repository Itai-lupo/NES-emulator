#include "LaughTaleEngine.h"
#include <gtest/gtest.h>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "imgui.h"



using namespace LaughTaleEngine;

enum sondWaves
{
    Sin = 0,
    SowTooth = 1,
    squre = 2
};

class soundEntity : public IEntity
{
    public:
        sondWaves mod;
        int f;
};


const double dOctaveBaseFrequency = 110.0; 
const double d12thRootOf2 = pow(2.0, 1.0 / 12.0);

void beep(IEntity *eventEntity, IEventData *sendor)
{
    SoundsampleData *data = static_cast<SoundsampleData *>(sendor);
    soundEntity *e = static_cast<soundEntity *>(eventEntity);

    static bool s = true;
    static int j = 0;
    static int f = 75;
    double time = data->time;
    // if(e->beep)
        // (*data->amp) = s * 255 * 255;
    // else
    //     (*data->amp) = (glm::sin((110 * pow(pow(2.0, 1.0 / 12.0), 1) * 2.0 * 3.14159  * time)) + 1) * (pow(2, (2 * 8) - 1) - 1);
    j = (j + 1) % (f + 1);
    if(j == f)
    {
        s = !s;
    }
    if(e->mod == sondWaves::Sin)
        (*data->amp) = ((sin(dOctaveBaseFrequency * pow(d12thRootOf2, e->f) * 2.0 * 3.14159 * time)) * (pow(2, (2 * 8) - 1) - 1));
    else if(e->mod == sondWaves::SowTooth)
        (*data->amp) = ((2.0 / 3.14159) * (dOctaveBaseFrequency * pow(d12thRootOf2, e->f) * 3.14159 * fmod(time, 1.0 / dOctaveBaseFrequency * pow(d12thRootOf2, e->f)) - (3.14159 / 2.0))) * (pow(2, (2 * 8) - 1) - 1);
    else
        (*data->amp) = (sin(dOctaveBaseFrequency * pow(d12thRootOf2, e->f) * 2.0 * 3.14159 * time)) > 0 ? (pow(2, (2 * 8) - 1) - 1): -(pow(2, (2 * 8) - 1) - 1);
}

void onKeyPressed(IEntity *eventEntity, IEventData *sendor)
{
    // KeyData *e = static_cast<KeyData *>(sendor);
    // soundEntity *en = static_cast<soundEntity *>(eventEntity);
    // for (int k = 0; k < 16; k++)
    // {
    //     if (("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k]) == e->key)
    //     {	
    //         dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, k);			

    //     }
    // }
    // en->beep = true;
}

void onKeyReleased(IEntity *eventEntity, IEventData *sendor)
{
    // soundEntity *e = static_cast<soundEntity *>(eventEntity);
    // dFrequencyOutput = 0;
    // e->beep = false;
}

void onImGUI(IEntity *eventEntity, IEventData *sendor)
{
    soundEntity *e = static_cast<soundEntity *>(eventEntity);
    ImGui::Text("%s", "sound control");  
    ImGui::SliderInt("frequency: ", &(e->f), 0, 12);
    ImGui::SliderInt("mode: ", (int *)&(e->mod), 0, 2);
}

void closeApp(__attribute__((unused)) IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
{
    app::keepRunning = false;
}

TEST(SoundEngine, soundInterface)
{
    soundEntity *e = new soundEntity();
    // e->beep = false;
    app::init();

    entityTaleId id = entityManger::addEntity(e);
    windowPieceId win1 =  windowManger::addWindow("win 1", true);

    eventManger::addEvent(events::onSoundsample, beep, id);
    eventManger::addEvent(events::KeyPressed, onKeyPressed, id, win1);
    eventManger::addEvent(events::KeyReleased, onKeyReleased, id, win1);
    eventManger::addEvent(events::ImGuiRender, onImGUI, id);

    eventManger::addEvent(events::WindowClose, closeApp);

    app::run();
    app::close();
    exit(0);
}