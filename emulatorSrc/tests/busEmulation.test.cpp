#include <gtest/gtest.h>
#include "LaughTaleEngine.h"
#include "6502.h"
#include "cpu.h"
#include "ram.h"
#include "bus.h"


class busTest : public ::testing::Test
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
            r->write(0xFFFC, 0x00);
            r->write(0xFFFD, 0x80);


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


TEST_F(busTest, loadCartageToBus)
{

    
}
