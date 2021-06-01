#include "LaughTaleEngine.h"
#include "harmonica.h"
#include "bell.h"
#include <gtest/gtest.h>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "imgui.h"
#include "drum.h"
#include "guitar.h"

using namespace LaughTaleEngine;


class soundEntity : public IEntity
{
    public:
        soundEntity()
        {
            for(int i = 0; i < 17; i++)
                for(int j = 0; j < 8; j++)
                    envIds[i][j] = 1000;
        }

        apoEnvelopeId envIds[17][8];
        apoSequencerAndRythemManer::drum *g;
        sondWaves mod;
        int f;
        int key;
};


void onImGUI(IEntity *eventEntity, IEventData *sendor)
{
    soundEntity *e = static_cast<soundEntity *>(eventEntity);
    ImGui::Text("sound control");
    ImGui::Text("modes: 1 = sin, 2,3 = sowTooth, 4,5 = squre, 6 = triangel, 7 = noise, 8 = speaker and 0 = mute");
    ImGui::Text( "%s", (
                std::string("|   |   |   |   |   | |   |   |   |   | |   | |   | |   |   |\n") +
                std::string("|   | S |   |   | F | | G |   |   | J | | K | | L | | ; |   |\n") +
                std::string("|   |___|   |   |___| |___|   |   |___| |___| |___| |___|   |\n") +
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
    return;
    app::init();
    soundEntity *e = new soundEntity();
    
    entityTaleId id = entityManger::addEntity(e);
    windowPieceId win1 =  windowManger::addWindow("win 1", true);

    eventManger::addEvent(events::AppRender, onAppRender, id, win1);
    eventManger::addEvent(events::ImGuiRender, onImGUI, id);

    eventManger::addEvent(events::WindowClose, closeApp);
    e->g = new apoSequencerAndRythemManer::drum();
    apoSequencerAndRythemManer::bellRing* bell = new apoSequencerAndRythemManer::bellRing();
    apoSequencerAndRythemManer::guitar* g = new apoSequencerAndRythemManer::guitar();
    app::run();
    app::close();
}