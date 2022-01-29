#include <gtest/gtest.h>
#include "LaughTaleEngine.h"
#include "6502.h"
#include "cpu.h"
#include "ram.h"
#include "bus.h"


class cpuTest : public ::testing::Test
{
    public:
        ram *r;
        cpu6502 *c;

        void SetUp()
        {
            LTE::app::init();
            r = new ram();
            c = new cpu6502();
            r->write(0xFFFC, 0x00);
            r->write(0xFFFD, 0x80);


            LTE::entityTaleId id =  LTE::entityManger::addEntity(
                [&](LTE::gameObject::gameObjectBuilder *b)
                {
                    b->setObjectName("system data")->
                        addComponent((new bus<uint8_t, uint16_t>())->pushDevice(r))->
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


TEST_F(cpuTest, textSimpleCommandExac)
{
    std::stringstream ss;
    ss << "EE 00 70";
    uint16_t nOffset = 0x8000;
    while (!ss.eof())
    {
        std::string b;
        ss >> b;
        r->write(nOffset++, (uint8_t)std::stoul(b, nullptr, 16));
    }

    LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu reset/"));
    for (size_t i = 0; i < 8; i++)
        LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu clock/"));
    
    for (size_t i = 0; i < 6; i++)
        LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu clock/"));

    ASSERT_EQ((int)r->read(0X7000), 1);

}


TEST_F(cpuTest, inc)
{
    /*
        tested asm:
            *=$8000
            inc $7000
            inc $70
            inx
            inc $6FFF,X
            inc $6F,X
    */


    std::stringstream ss;
    ss << "EE 00 70 E6 70 E8 FE FF 6F F6 6F";
    uint16_t nOffset = 0x8000;
    while (!ss.eof())
    {
        std::string b;
        ss >> b;
        r->write(nOffset++, (uint8_t)std::stoul(b, nullptr, 16));
    }

    LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu reset/"));

    for (size_t i = 0; i < 8 + 6 + 5 + 2 + 7 + 6; i++)
        LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu clock/"));

    ASSERT_EQ((int)r->read(0X0070), 2);
    ASSERT_EQ((int)r->read(0X7000), 2);
    ASSERT_TRUE((cpu<uint8_t, uint16_t>::isCpuComplete()));
}
