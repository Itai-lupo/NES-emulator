#pragma once
#include "LaughTaleEngine.h"

#include "2c02.h"
#include "bus.h"

#include <thread>

class ppu
{
    private:
        static inline LTE::entityTaleId SystemData;

        static inline uint8_t cycles = 0X00;
    public:
        static inline bool frameComplete = false;
        static bool isCpuComplete(){ return cycles == 0; }

        static void init(LTE::entityTaleId SystemDataId)
        {
            SystemData = SystemDataId;

            LTE::eventManger::startBuildingEvent()->
                setEntityID(SystemData)->
                setEventRoute("cpu cmd/cpu clock/2c02")->
                setEventCallback(ppu::clock)->add();

        }

        static void close()
        {

        }


        static void clock(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            bus<uint8_t, uint16_t> *busData = LTE::entityManger::getEntityById(SystemData)->getComponent<bus<uint8_t, uint16_t>>();
            ppu2c02 *ppuData = LTE::entityManger::getEntityById(SystemData)->getComponent<ppu2c02>();
        
            for (size_t i = 0; i < 3; i++)
            {
                if(ppuData->scanline == -1 && ppuData->cycle == 1)
                    ppuData->status.vertical_blank = 0;


                if(ppuData->scanline == 241 && ppuData->cycle == 1)
                {
                    ppuData->status.vertical_blank = 1;
                    if(ppuData->control.enable_nmi){
                        sendor->route = "cpu cmd/cpu nmi/";
                        LTE::eventManger::trigerEvent(sendor);
                    }
                }
                
                ppuData->cycle++;
                if (ppuData->cycle >= 341)
                {
                    ppuData->cycle = 0;
                    ppuData->scanline++;
                    if (ppuData->scanline >= 261)
                    {
                        ppuData->scanline = -1;
                        frameComplete = true;
                    }
                }
            }            
        }
        
};
