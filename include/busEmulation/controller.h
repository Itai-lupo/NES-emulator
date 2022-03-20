#pragma once
#include "busDevice.h"
#include "LaughTaleEngine.h"
#include <cstdint>

#include <array>

class controller: public busDevice<uint8_t, uint16_t>, public LTE::component
{
    private:
    
    	union
        {
            struct 
            {
                uint8_t a;
                uint8_t b;                
            };
            uint8_t reg[2];
        } controllerData;
        
    	uint8_t controller_state[2];

    public:

        static void keyDispatcher(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            controller *controllerContainer = eventEntity->getComponent<controller>();

            controllerContainer->controllerData.reg[0] = 0x00;
            controllerContainer->controllerData.reg[0] |= sendor->win->inputManger->isKeyPressed(LT_KEY_X) ?     0x80 : 0x00;
            controllerContainer->controllerData.reg[0] |= sendor->win->inputManger->isKeyPressed(LT_KEY_Z) ?     0x40 : 0x00;
            controllerContainer->controllerData.reg[0] |= sendor->win->inputManger->isKeyPressed(LT_KEY_A) ?     0x20 : 0x00;
            controllerContainer->controllerData.reg[0] |= sendor->win->inputManger->isKeyPressed(LT_KEY_S) ?     0x10 : 0x00;
            controllerContainer->controllerData.reg[0] |= sendor->win->inputManger->isKeyPressed(LT_KEY_UP) ?    0x08 : 0x00;
            controllerContainer->controllerData.reg[0] |= sendor->win->inputManger->isKeyPressed(LT_KEY_DOWN) ?  0x04 : 0x00;
            controllerContainer->controllerData.reg[0] |= sendor->win->inputManger->isKeyPressed(LT_KEY_LEFT) ?  0x02 : 0x00;
            controllerContainer->controllerData.reg[0] |= sendor->win->inputManger->isKeyPressed(LT_KEY_RIGHT) ? 0x01 : 0x00;
        }      

        controller()
        {

            
        }

        ~controller()
        {

        }


        virtual void init(LTE::gameObject *parent)
        {
            LTE::eventManger::startBuildingEvent()->
                setEntityID(parent->getId())->
                setEventRoute("window render/nes emulator/controller key handler")->
                setEventCallback(keyDispatcher)->
                add();
                

        }
        virtual void end(){}

        virtual bool isInRange(uint16_t addr) override
        {
        	return addr == 0x4016 || addr == 0x4017;
        }

        virtual uint8_t read(uint16_t addr, bool bReadOnly = false) override
        {
            uint8_t data = (controller_state[addr & 0x0001] & 0x80) > 0;
            controller_state[addr & 0x0001] <<= 1;
            LAUGHTALE_ENGINR_LOG_INFO((int)data);

            return data;
        }

        virtual void write(uint16_t addr, uint8_t data) override
        {
    		controller_state[addr & 0x0001] = controllerData.reg[addr & 0x0001];
            LAUGHTALE_ENGINR_LOG_INFO((int)controller_state[addr & 0x0001] );

        }

};