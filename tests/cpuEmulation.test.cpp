#include <gtest/gtest.h>
#include "LaughTaleEngine.h"
#include "6502.h"
#include "cpu.h"
#include "bus.h"

class testRam: public busDevice<uint8_t, uint16_t>
{
    private:
    	std::array<uint8_t, 0xFFFF> ramData;

    public:
        testRam()
        {
            for (auto &i : ramData) i = 0x00;
        }

        ~testRam()
        {

        }

        virtual bool isInRange(uint16_t addr) override
        {
        	return addr >= 0x0000 && addr <= 0xFFFF;
        }

        virtual uint8_t read(uint16_t addr, bool bReadOnly = false) override
        {
            return ramData[addr];
        }

        virtual void write(uint16_t addr, uint8_t data) override
        {
            ramData[addr] = data;
        }

};

class cpuTest : public ::testing::Test
{
    public:
        testRam *r;
        cpu6502 *c;

        void SetUp()
        {
            LTE::app::init();
            r = new testRam();
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


TEST_F(cpuTest, testSimpleCommandExac)
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


TEST_F(cpuTest, INC)
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

TEST_F(cpuTest, LDA)
{
/*
        tested asm:
        LOC   CODE         LABEL     INSTRUCTION
        8000                         * = $8000
        8000  A9 46                  LDA #$46
        8002  A9 2D                  LDA #$2D
        8004  85 46                  STA $46
        8006  A9 00                  LDA #$00
        8008  A5 46                  LDA $46
        800A  A9 32                  LDA #$32
        800C  A2 05                  LDX #$05
        800E  95 46                  STA $46,X
        8010  A9 00                  LDA #$00
        8012  B5 46                  LDA $46,X
        8014  EE 58 1B               INC $1B58
        8017  AD 58 1B               LDA $1B58
        801A  A2 05                  LDX #$05
        801C  8E 00 71               STX $7100
        801F  BD FB 70               LDA $70FB,X
        8022  A0 03                  LDY #$03
        8024  8C 00 72               STY $7200
        8027  B9 FD 71               LDA $71FD,Y
        802A  A2 0A                  LDX #$0A
        802C  86 50                  STX $50
        802E  86 0A                  STX $0A
        8030  E6 0A                  INC $0A
        8032  A1 46                  LDA ($46,X)
        8034  A0 0D                  LDY #$0D
        8036  84 50                  STY $50
        8038  84 1A                  STY $1A
        803A  E6 1A                  INC $1A
        803C  B1 50                  LDA ($50),Y
*/


    std::stringstream ss;
    ss << "A9 46 A9 2D 85 46 A9 00 A5 46 A9 32 A2 05 95 46 A9 00 B5 46 EE 58 1B AD 58 1B A2 05 8E 00 71 BD FB 70 A0 03 8C 00 72 B9 FD 71 A2 0A 86 50 86 0A E6 0A A1 46 A0 0D 84 50 84 1A E6 1A B1 50";
    uint16_t nOffset = 0x8000;
    while (!ss.eof())
    {
        std::string b;
        ss >> b;
        r->write(nOffset++, (uint8_t)std::stoul(b, nullptr, 16));
    }
    std::pair<int, int> testExpectedRes[] = {
        {2, 70},
        {2 + 3 + 2 + 3, 45},
        {2 + 2 + 4 + 2 + 4, 50},
        {6 + 4, 1},
        {2 + 4 + 5, 5},
        {2 + 4 + 5, 3},
        {2 + 3 + 3 + 5 + 6, 11},
        {2 + 3 + 3 + 5 + 5, 14},
    };
 
    LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu reset/"));

    for (size_t i = 0; i < 8; i++)
        LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu clock/"));

    for(size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < testExpectedRes[i].first; j++)
            LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu clock/"));
        ASSERT_EQ((int)c->a, testExpectedRes[i].second);
        ASSERT_TRUE((cpu<uint8_t, uint16_t>::isCpuComplete()));
    }

}

TEST_F(cpuTest, MULProgram)
{
/*
        tested asm:
        *=$8000
        LDX #10
        STX $0000
        LDX #3
        STX $0001
        LDY $0000
        LDA #0
        CLC
        loop
        ADC $0001
        DEY
        BNE loop
        STA $0002

        LOC   CODE         LABEL     INSTRUCTION

*/


    std::stringstream ss;
    ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00";
    uint16_t nOffset = 0x8000;
    while (!ss.eof())
    {
        std::string b;
        ss >> b;
        r->write(nOffset++, (uint8_t)std::stoul(b, nullptr, 16));
    }

    LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu reset/"));

    for (size_t i = 0; i < 8 + 2 + 4 + 2 + 4 + 4 + 2 + 2 + (4 + 2 + 3) * 11 - 1 + 4; i++)
        LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu clock/"));

    ASSERT_EQ((int)r->read(2), 30);
    ASSERT_TRUE((cpu<uint8_t, uint16_t>::isCpuComplete()));
}
