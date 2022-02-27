#include "LaughTaleEngine.h"
#include "6502.h"
#include "cpu.h"
#include "ram.h"
#include "bus.h"
#include "cpuBusCartridge.h"
#include "ppuBusCartridge.h"

#include "imgui.h"

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


static void ImGuiRender(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
{
    LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
    
    cpu6502 *c = eventEntity->getComponent<cpu6502>();
    bus<uint8_t, uint16_t> *sysBus = eventEntity->getComponent<bus<uint8_t, uint16_t>>();
    ram *r = new ram();

    ImGui::Text("test");
}

int main() 
{
    LTE::app::init();

    ram *r = new ram();
    cpu6502 *c = new cpu6502();
    bus<uint8_t, uint16_t> *sysBus = (new bus<uint8_t, uint16_t>())->pushDevice(r);

    cartridge *cart = new cartridge();
    cart->load("testRes/roms/nestest.nes");
    sysBus->pushDevice(new cpuBusCartridge(cart));

    LTE::entityTaleId id =  LTE::entityManger::addEntity(
        [&](LTE::gameObject::gameObjectBuilder *b)
        {
            b->setObjectName("system data")->
                addComponent(sysBus)->
                addComponent(c);
        });

    cpu<uint8_t, uint16_t>::init(id);

    LTE::windowPieceId winId =  LTE::windowManger::addWindow([](LTE::windowBuilder *b){ b->setTitle("nes emulator")->useImGui();});

    LTE::eventManger::startBuildingEvent()->
        setEntityID(id)->
        setEventRoute("Key pressed/cpu keyDispatcher")->
        setEventCallback(keyDispatcher)->
        add();

    LTE::eventManger::startBuildingEvent()->
        setEventRoute("ImGui render/print debug info")->
        setEventCallback(ImGuiRender)->
        setEntityID(id)->
        setWindowId(winId)->add();


    LTE::app::run();
    cpu<uint8_t, uint16_t>::close();
    LTE::app::close();
    return 0;
}
