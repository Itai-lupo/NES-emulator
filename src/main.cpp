#include "LaughTaleEngine.h"
#include "6502.h"
#include "cpu.h"
#include "ram.h"
#include "bus.h"
#include "cpuBusCartridge.h"
#include "ppuBusCartridge.h"
#include "imgui.h"
#include "displayDebugInfo.h"
#include "gamesMenu.h"

void keyDispatcher(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
{
    LTE::KeyData *eventData = static_cast<LTE::KeyData *>(sendor);
    
    if(eventData->key == LT_KEY_SPACE)
        sendor->route = "cpu cmd/cpu clock/";

    if(eventData->key == LT_KEY_R)        
        sendor->route = "cpu cmd/cpu reset/";
            
    if(eventData->key == LT_KEY_I)
        sendor->route = "cpu cmd/cpu irq/";
        
    if(eventData->key == LT_KEY_N)
        sendor->route = "cpu cmd/cpu nmi/";

    if(sendor->route != "Key pressed/cpu keyDispatcher")
        LTE::eventManger::trigerEvent(sendor);
}

LTE::entityTaleId id;
cartridge *cart;
void initEmulationSystem()
{

    ram *r = new ram();
    cpu6502 *c = new cpu6502();
    bus<uint8_t, uint16_t> *sysBus = (new bus<uint8_t, uint16_t>())->pushDevice(r);

    cart = new cartridge();
    sysBus->pushDevice(new cpuBusCartridge(cart));

    id =  LTE::entityManger::addEntity(
        [&](LTE::gameObject::gameObjectBuilder *b)
        {
            b->setObjectName("system data")->
                addComponent(sysBus)->
                addComponent(c);
        });

    cpu<uint8_t, uint16_t>::init(id);

}

void loadCartageAndResetCpu(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
{
    gamesMenu::loadGameEvent *gameInfo = static_cast<gamesMenu::loadGameEvent*>(sendor);
    LAUGHTALE_ENGINR_LOG_INFO(gameInfo->gamePath)
    cart->load(gameInfo->gamePath);
    LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu reset/"));
    for (size_t i = 0; i < 8; i++)
        LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu clock/"));

}

int main() 
{
    LTE::app::init();
    
    initEmulationSystem();

    LTE::windowPieceId winId =  LTE::windowManger::addWindow([](LTE::windowBuilder *b){ b->setTitle("nes emulator")->useImGui();});
    
    displayDebugInfo::init(winId, id);

    LTE::eventManger::startBuildingEvent()->
        setEntityID(id)->
        setEventRoute("Key pressed/cpu keyDispatcher")->
        setEventCallback(keyDispatcher)->
        add();

    gamesMenu::init(winId, cart);



    LTE::eventManger::startBuildingEvent()->
        setEventRoute("load game/load cartage")->
        setEventCallback(loadCartageAndResetCpu)->add();

    LTE::app::run();
    cpu<uint8_t, uint16_t>::close();
    LTE::app::close();
    return 0;
}
