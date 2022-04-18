#include "LaughTaleEngine.h"
#include "6502.h"
#include "2c02.h"
#include "cpu.h"
#include "ppu.h"
#include "ram.h"
#include "bus.h"
#include "cpuBusCartridge.h"
#include "ppuBusCartridge.h"
#include "imgui.h"
#include "displayDebugInfo.h"
#include "gamesMenu.h"
#include "controller.h"
#include "DMA.h"

#include <thread>
void sysClock(LTE::gameObject *eventEntity, LTE::coreEventData *sendor);

LTE::entityTaleId id;
void runCpu(LTE::coreEventData *sendor)
{
    sendor->route = "cpu cmd/cpu clock/";
    LTE::gameObject *eventEntity = LTE::entityManger::getEntityById(id);
    while (true)
        sysClock(eventEntity, sendor);
}

std::thread *t;
void keyDispatcher(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
{
    LTE::KeyData *eventData = static_cast<LTE::KeyData *>(sendor);
    
    if(eventData->key == LT_KEY_SPACE)    
    {
        sendor->route = "cpu cmd/cpu clock/";
        LTE::eventManger::trigerEvent(sendor);

        return;
    }

    if(eventData->key == LT_KEY_F2)
    {
        t = new std::thread(runCpu, sendor);
    }
    

    if(eventData->key == LT_KEY_F)        
    {
        sendor->route = "cpu cmd/cpu clock/";
        while (!ppu::frameComplete)
            LTE::eventManger::trigerEvent(sendor);
        ppu::frameComplete = false;
        return;
    }

    if(eventData->key == LT_KEY_P)        
        displayDebugInfo::changePallate();


    if(eventData->key == LT_KEY_R)        
        sendor->route = "cpu cmd/cpu reset/";
            
    if(eventData->key == LT_KEY_I)
        sendor->route = "cpu cmd/cpu irq/";
        
    if(eventData->key == LT_KEY_N)
        sendor->route = "cpu cmd/cpu nmi/";

    if(sendor->route != "Key pressed/cpu keyDispatcher")
        LTE::eventManger::trigerEvent(sendor);
}

cartridge *cart;
LTE::windowPieceId winId;

float tilePostions[12] =
{
    0.0, 1.0f, 0.0f,
    0.0, 0.0f, 0.0f,
    1.0, 0.0f, 0.0f,
    1.0, 1.0f, 0.0f
};

unsigned int tileIndices[6] =
{
    0,
    1,
    2,
    0,
    3,
    2,
};

void sysClock(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
{
    ppu::clock(eventEntity, sendor);
    cpu<uint8_t, uint16_t>::clock(eventEntity, sendor);

    if(ppu::nmi)
    {
        ppu::nmi = false;
        sendor->route = "cpu cmd/cpu nmi/";
        LTE::eventManger::trigerEvent(sendor);
    }
}

void initEmulationSystem()
{

    ram *r = new ram();
    cart = new cartridge();
    cpu6502 *c = new cpu6502();
    ppu2c02 *p = new ppu2c02(new ppuBusCartridge(cart));
    controller *ctrl = new controller();
    bus<uint8_t, uint16_t> *sysBus = (new bus<uint8_t, uint16_t>());
    DMA *d = new DMA(p, sysBus);

    LTE::texture *t = LTE::windowManger::getWindow(winId)->context->getMeshFactory()->createCustemTexture({256, 240});

    LTE::windowManger::getWindow(winId)->assetLibrary->saveAsset(t, "res/NEStexture"); 
    LTE::windowManger::getWindow(winId)->assetLibrary->loadAssetFromFile("res/shaders/Basic.glsl");

    sysBus->
        pushDevice(new cpuBusCartridge(cart))->
        pushDevice(r)->
        pushDevice(ctrl)->
        pushDevice(d)->
        pushDevice(p);

    id =  LTE::entityManger::addEntity(
        [&](LTE::gameObject::gameObjectBuilder *b)
        {
            b->setObjectName("system data")->
                setWindowId(winId)->
                setObjectTransform({{-232.0f / 256.0f * 2.0f, -1, 0}, {0, 0, 0}, {232.0f / 256.0f * 2.0f, 2, 0}})->
                addComponent(sysBus)->
                addComponent(p)->
                addComponent(c)->
                addComponent(d)->
                addComponent(ctrl)->
                addComponent(LTE::mesh::build([&](LTE::mesh::meshBuilder *builder)
                    { 
                        builder->
                            setIndexBuffer(tileIndices, 6)->
                            setShaderName("res/shaders/Basic.glsl")->
                            setVertices(tilePostions, 12); 
                    }))->
                addComponent(new LTE::material("res/NEStexture", glm::vec4(0, 0, 0, 1.0f)));
        });

    cpu<uint8_t, uint16_t>::init(id);
    ppu::init(id);
    LTE::eventManger::startBuildingEvent()->setEntityID(id)->setEventRoute("cpu cmd/cpu clock/system")->setEventCallback(sysClock)->add();


}

void loadCartageAndResetCpu(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
{
    gamesMenu::loadGameEvent *gameInfo = static_cast<gamesMenu::loadGameEvent*>(sendor);
    cart->load(gameInfo->gamePath);
    eventEntity->getComponent<ppu2c02>()->reset();
    LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu reset/"));

}


void WindowClose(__attribute__((unused)) LTE::gameObject *eventEntity, __attribute__((unused)) LTE::coreEventData *sendor)
{
    LTE::app::keepRunning = false;
}


int main() 
{
    LTE::app::init();
    
    winId =  LTE::windowManger::addWindow([](LTE::windowBuilder *b){ b->setTitle("nes emulator")->useImGui();});
    initEmulationSystem();

    
    displayDebugInfo::init(winId, id);

    LTE::eventManger::startBuildingEvent()->
        setEntityID(id)->
        setEventRoute("Key pressed/cpu keyDispatcher")->
        setEventCallback(keyDispatcher)->
        add();

    gamesMenu::init(winId, cart);



    LTE::eventManger::startBuildingEvent()->
        setEntityID(id)->
        setEventRoute("load game/load cartage")->
        setEventCallback(loadCartageAndResetCpu)->add();

 
    LTE::eventManger::startBuildingEvent()->
        setEventRoute("Window close/close app")->
        setEventCallback(WindowClose)->add();

    LTE::app::run();
    cpu<uint8_t, uint16_t>::close();
    LTE::app::close();
    return 0;
}
