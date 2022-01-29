#include "LaughTaleEngine.h"
#include "6502.h"
#include "cpu.h"
#include "ram.h"
#include "bus.h"



static void keyDispatcher(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
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

int main() 
{
    LTE::app::init();
    //game systems init code go here;
    ram *r = new ram();

    std::stringstream ss;
    ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
    uint16_t nOffset = 0x8000;
    while (!ss.eof())
    {
        std::string b;
        ss >> b;
        r->write(nOffset++, (uint8_t)std::stoul(b, nullptr, 16));
    }

    r->write(0xFFFC, 0x00);
    r->write(0xFFFD, 0x80);


    LTE::entityTaleId id =  LTE::entityManger::addEntity(
        [&](LTE::gameObject::gameObjectBuilder *b)
        {
            b->setObjectName("system data")->
                addComponent(new cpu6502())->
                addComponent((new bus<uint8_t, uint16_t>())->pushDevice(r));
        });

    cpu<uint8_t, uint16_t>::init(id);

    LTE::windowManger::addWindow([](LTE::windowBuilder *b){ b->setTitle("nes emulator");});

    LTE::eventManger::startBuildingEvent()->setEntityID(id)->setEventRoute("Key pressed/cpu keyDispatcher")->setEventCallback(keyDispatcher)->add();


    LTE::app::run();
    //game systems close code go here;
    cpu<uint8_t, uint16_t>::close();
    LTE::app::close();
    return 0;
}
