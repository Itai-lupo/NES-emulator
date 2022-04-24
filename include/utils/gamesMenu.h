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
        static inline LTE::coreEventData *qSendorData;
        static inline bool showMenu = true;


        static void quiteGame(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            LTE::KeyData *e = dynamic_cast<LTE::KeyData *>(sendor);
            if(e->key == LT_KEY_ESCAPE)
            {
                LTE::eventManger::trigerEvent(qSendorData);
                cart->unload();
                showMenu = true;
            }
        }

        static void ImGuiRender(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            if(!showMenu) return;

            LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
            
            if (!ImGui::Begin("game list"))
            {
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
                    showMenu = false;
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
            
            LTE::eventManger::addCoustemEventsRoute("load game/");
            LTE::eventManger::addCoustemEventsRoute("quite game/");
            

            sendorData = new loadGameEvent("load game/");
            qSendorData = new LTE::coreEventData("quite game/");

            LTE::eventManger::startBuildingEvent()->
                setEventRoute("ImGui render/show games menu")->
                setEventCallback(ImGuiRender)->
                setWindowId(winId)->add();

            LTE::eventManger::startBuildingEvent()->
                setEventRoute("Key pressed/check if quite game")->
                setEventCallback(quiteGame)->
                setWindowId(winId)->add();

        }
};

