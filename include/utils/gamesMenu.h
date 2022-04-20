#pragma once
#include "LaughTaleEngine.h"
#include "cartridge.h"
#include "imgui.h"

#include <filesystem>

class gamesMenu
{
    public:
        struct loadGameEvent: public LTE::coreEventData
        {
            loadGameEvent(const std::string& route): coreEventData(route){}

            std::string gamePath;
        };

    private:

        static inline int page;
        static inline bool showDebugInfo = false;
        static inline LTE::windowPieceId winId;
        static inline cartridge *cart;
        static inline loadGameEvent *sendorData;


        static void ImGuiRender(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
            
            if (!ImGui::Begin("game list"))
            {
                // Early out if the window is collapsed, as an optimization.
                ImGui::End();
                return;
            }

            std::filesystem::recursive_directory_iterator iter("./res/roms");
            std::filesystem::recursive_directory_iterator end;
            while (iter != end)
            {
                if (ImGui::Button(iter->path().stem().c_str()))
                {
                    sendorData->gamePath = iter->path().string();
                    LTE::eventManger::trigerEvent(sendorData);
                }

                std::error_code ec;
                iter.increment(ec);
                if (ec)
                    LAUGHTALE_ENGINR_LOG_ERROR("Error While Accessing : " << iter->path().string() << " :: " << ec.message());
            }

            ImGui::End();
        }

    public:
        static void init(LTE::windowPieceId winId, cartridge *cart){
            gamesMenu::winId = winId;
            gamesMenu::cart = cart;
            

            sendorData = new loadGameEvent("load game/");

            LTE::eventManger::startBuildingEvent()->
                setEventRoute("ImGui render/show games menu")->
                setEventCallback(ImGuiRender)->
                setWindowId(winId)->add();

            sendorData->gamePath = "./res/roms/Donkey Kong (World) (Rev A).nes";
            // sendorData->gamePath = "./res/roms/Galaga (U).nes";
            // sendorData->gamePath = "./res/roms/Ice Climber (USA, Europe).nes";
            // sendorData->gamePath = "./res/roms/Kung Fu (Japan, USA).nes";
            // sendorData->gamePath = "./res/roms/Pac-Man (USA) (Namco).nes";
            sendorData->gamePath = "./res/roms/Super Mario Bros (E).nes";
            LTE::eventManger::trigerEvent(sendorData);

        }
};

