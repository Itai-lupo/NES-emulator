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
                uint8_t right:  1;
                uint8_t left:   1;                
                uint8_t down:   1;                
                uint8_t up:     1;

                uint8_t start:  1;
                uint8_t select: 1;                
                
                uint8_t B:      1;            
                uint8_t A:      1;                

            };
            uint8_t reg;
        } controllerData[2];
        
    	uint8_t controller_state[2];

    public:

        static void keyDispatcher(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            controller *controllerContainer = eventEntity->getComponent<controller>();

            controllerContainer->controllerData[0].A        = sendor->win->inputManger->isKeyPressed(LT_KEY_Q);
            controllerContainer->controllerData[0].B        = sendor->win->inputManger->isKeyPressed(LT_KEY_E);
            
            controllerContainer->controllerData[0].select   = sendor->win->inputManger->isKeyPressed(LT_KEY_Z);
            controllerContainer->controllerData[0].start    = sendor->win->inputManger->isKeyPressed(LT_KEY_X);

            controllerContainer->controllerData[0].up       = sendor->win->inputManger->isKeyPressed(LT_KEY_W);
            controllerContainer->controllerData[0].down     = sendor->win->inputManger->isKeyPressed(LT_KEY_S);
            controllerContainer->controllerData[0].left     = sendor->win->inputManger->isKeyPressed(LT_KEY_A);
            controllerContainer->controllerData[0].right    = sendor->win->inputManger->isKeyPressed(LT_KEY_D);



            controllerContainer->controllerData[1].A        = sendor->win->inputManger->isKeyPressed(LT_KEY_1);
            controllerContainer->controllerData[1].B        = sendor->win->inputManger->isKeyPressed(LT_KEY_2);
            
            controllerContainer->controllerData[1].select   = sendor->win->inputManger->isKeyPressed(LT_KEY_BACKSLASH);
            controllerContainer->controllerData[1].start    = sendor->win->inputManger->isKeyPressed(LT_KEY_ENTER);

            controllerContainer->controllerData[1].up       = sendor->win->inputManger->isKeyPressed(LT_KEY_UP);
            controllerContainer->controllerData[1].down     = sendor->win->inputManger->isKeyPressed(LT_KEY_DOWN);
            controllerContainer->controllerData[1].left     = sendor->win->inputManger->isKeyPressed(LT_KEY_LEFT);
            controllerContainer->controllerData[1].right    = sendor->win->inputManger->isKeyPressed(LT_KEY_RIGHT);
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

            return data;
        }

        virtual void write(uint16_t addr, uint8_t data) override
        {
    		controller_state[addr & 0x0001] = controllerData[addr & 0x0001].reg;
        }

};