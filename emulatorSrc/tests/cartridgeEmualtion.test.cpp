#include <gtest/gtest.h>
#include "LaughTaleEngine.h"
#include "6502.h"
#include "cpu.h"
#include "ram.h"
#include "bus.h"
#include "cpuBusCartridge.h"
#include "ppuBusCartridge.h"

class cartridgeEmualtionTest : public ::testing::Test
{
    public:
        ram *r;
        bus<uint8_t, uint16_t> *sysBus;
        cpu6502 *c;

        void SetUp()
        {
            LTE::app::init();
            r = new ram();
            c = new cpu6502();
            sysBus = (new bus<uint8_t, uint16_t>())->pushDevice(r);


            LTE::entityTaleId id =  LTE::entityManger::addEntity(
                [&](LTE::gameObject::gameObjectBuilder *b)
                {
                    b->setObjectName("system data")->
                        addComponent(sysBus)->
                        addComponent(c);
                });

            cpu<uint8_t, uint16_t>::init(id);
        }

        void TearDown()
        {
            cpu<uint8_t, uint16_t>::close();
            LTE::app::close();

        } 

};


TEST_F(cartridgeEmualtionTest, loadCartageToBus)
{
    cartridge *cart = new cartridge();
    cart->load("testRes/roms/nestest.nes");
    sysBus->pushDevice(new cpuBusCartridge(cart));
}


TEST_F(cartridgeEmualtionTest, loadCartageThrowAtUnvalidFile)
{
    cartridge *cart = new cartridge();
    
    try
    {
        cart->load("testRes/roms/empty.ne");
        FAIL();
    }
    catch(std::exception* e)
    {
        EXPECT_STREQ(e->what(), "testRes/roms/empty.ne rom file not found error") << "wrong error: " << e->what();
    }

    try
    {
        cart->load("testRes/roms/empty.nes");
        FAIL();
    }
    catch(std::exception* e)
    {
        EXPECT_STREQ(e->what(), "rom not start with NES error") << "wrong errot:"  << e->what();
    }
    
}


TEST_F(cartridgeEmualtionTest, loadCartageAndRun)
{
    cartridge *cart = new cartridge();
    cart->load("testRes/roms/nestest.nes");
    sysBus->pushDevice(new cpuBusCartridge(cart));


    LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu reset/"));
    for (size_t i = 0; i < 8; i++)
        LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu clock/"));

    for (size_t i = 0; i < 8 + 2 + 4 + 2 + 4 + 4 + 2 + 2 + (4 + 2 + 3) * 11 - 1 + 4; i++)
        LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu clock/"));
    
}
