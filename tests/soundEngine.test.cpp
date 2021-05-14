#include "LaughTaleEngine.h"
#include <gtest/gtest.h>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "imgui.h"



using namespace LaughTaleEngine;


class soundEntity : public IEntity
{
    public:
        sondWaves mod;
        int f;
        int key;
};


const double dOctaveBaseFrequency = 110.0; 
const double d12thRootOf2 = pow(2.0, 1.0 / 12.0);

void onKeyPressed(IEntity *eventEntity, IEventData *sendor)
{
    KeyData *e = static_cast<KeyData *>(sendor);
    soundEntity *data = static_cast<soundEntity *>(eventEntity);
    for (int k = 0; k < 17; k++)
    {
        if (("ZSXCFVGBNJMK,L.;/"[k]) == e->key)
        {	
            data->key = e->key;
            data->f = dOctaveBaseFrequency * pow(d12thRootOf2, k);
            soundEngine::setSoundWave(data->f, data->mod);
        }
    }

    for (int k = 0; k < 8; k++)
    {
        if (("01234567"[k]) == e->key)
        {	
            data->mod = (sondWaves)k;
            soundEngine::setSoundWave(data->f, data->mod);
        }
    }
}

void onKeyReleased(IEntity *eventEntity, IEventData *sendor)
{
    KeyData *e = static_cast<KeyData *>(sendor);
    soundEntity *data = static_cast<soundEntity *>(eventEntity);
    if(data->key == e->key)
        data->f = 0;
    soundEngine::setSoundWave(data->f, data->mod);
    
}

void onImGUI(IEntity *eventEntity, IEventData *sendor)
{
    soundEntity *e = static_cast<soundEntity *>(eventEntity);
    ImGui::Text("sound control");
    ImGui::Text("modes: 1 = sin, 2,3 = sowTooth, 4,5 = squre, 6 = triangel, 7 = speaker and 0 = mute");
    ImGui::Text( "%s", (
                std::string("|   |   |   |   |   | |   |   |   |   | |   | |   | |   |   |\n") +
                std::string("|   | S |   |   | F | | G |   |   | J | | K | | L | | ; |   |\n") +
                std::string("|   |___|   |   |___| |___|   |   |___| |___| |___| |___|   |__\n") +
                std::string("|  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |\n") +
                std::string("|     |     |     |     |     |     |     |     |     |     |\n") +
                std::string("|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|\n")).c_str());
}


void onAppRender(IEntity *eventEntity, IEventData *sendor)
{
    renderApi *api = windowManger::getRenderApi(sendor->windowId);
    api->SetClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 0.7f));
    api->Clear();

}

void closeApp(__attribute__((unused)) IEntity *eventEntity, __attribute__((unused)) IEventData *sendor)
{
    app::keepRunning = false;
}

TEST(SoundEngine, soundInterface)
{
    soundEntity *e = new soundEntity();
    app::init();

    entityTaleId id = entityManger::addEntity(e);
    windowPieceId win1 =  windowManger::addWindow("win 1", true);

    eventManger::addEvent(events::KeyPressed, onKeyPressed, id, win1);
    eventManger::addEvent(events::KeyReleased, onKeyReleased, id, win1);
    eventManger::addEvent(events::AppRender, onAppRender, id, win1);
    eventManger::addEvent(events::ImGuiRender, onImGUI, id);

    eventManger::addEvent(events::WindowClose, closeApp);

    app::run();
    app::close();
    exit(0);
}