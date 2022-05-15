#pragma once
#include "LaughTaleEngine.h"

#include "6502.h"
#include "bus.h"
#include "DMA.h"

#include <thread>

template<typename dataSize, typename addrSize>
class cpu
{
    private:
        static inline LTE::entityTaleId SystemData;
        static inline std::thread *clockT;

    public:
    	static std::string hex(uint32_t n, uint8_t d)
        {
            std::string s(d, '0');
            for (int i = d - 1; i >= 0; i--, n >>= 4)
                s[i] = "0123456789ABCDEF"[n & 0xF];
            return s;
        }

        static void init(LTE::entityTaleId SystemDataId)
        {
            SystemData = SystemDataId;


            LTE::eventManger::addCoustemEventsRoute("cpu cmd/");
            LTE::eventManger::addCoustemEventsRoute("cpu cmd/cpu reset/");
            LTE::eventManger::addCoustemEventsRoute("cpu cmd/cpu irq/");
            LTE::eventManger::addCoustemEventsRoute("cpu cmd/cpu nmi/");
            LTE::eventManger::addCoustemEventsRoute("cpu cmd/cpu clock/");
            
            LTE::eventManger::startBuildingEvent()->setEntityID(SystemData)->setEventRoute("cpu cmd/cpu reset/6502")->setEventCallback(cpu<uint8_t, uint16_t>::reset)->add();
            LTE::eventManger::startBuildingEvent()->setEntityID(SystemData)->setEventRoute("cpu cmd/cpu irq/6502")->setEventCallback(cpu<uint8_t, uint16_t>::irq)->add();
            LTE::eventManger::startBuildingEvent()->setEntityID(SystemData)->setEventRoute("cpu cmd/cpu nmi/6502")->setEventCallback(cpu<uint8_t, uint16_t>::nmi)->add();
        }

        static void close()
        {

        }

        static void executeInstruction(bus<dataSize, addrSize> *busData, cpu6502 *cpuData)
        {
            cpuData->opcode = busData->read(cpuData->pc);
            cpuData->pc++;
            cpuData->status.U = 1;

            cpuData->cycles = cpuData->lookup[cpuData->opcode].cycles;

            int additional_cycle1 = (cpuData->*(cpuData->lookup[cpuData->opcode].addrmode))();
            int additional_cycle2 = (cpuData->*(cpuData->lookup[cpuData->opcode].operate))();

            cpuData->cycles += (additional_cycle1 & additional_cycle2);
            cpuData->status.U = 1;
        }


        static void clock(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            bus<dataSize, addrSize> *busData = eventEntity->getComponent<bus<dataSize, addrSize>>();
            cpu6502 *cpuData = eventEntity->getComponent<cpu6502>();
            DMA *dma = eventEntity->getComponent<DMA>();


            if (dma->isOn())
                dma->clock();
            else{
                if((int)cpuData->cycles == 0)
                {
                    executeInstruction(busData, cpuData);
                }
                cpuData->clock++;
                cpuData->cycles--;
            }
        }

        static void reset(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            bus<dataSize, addrSize> *busData = LTE::entityManger::getEntityById(SystemData)->getComponent<bus<dataSize, addrSize>>();
            cpu6502 *cpuData = LTE::entityManger::getEntityById(SystemData)->getComponent<cpu6502>();

            uint16_t addr_abs = 0xFFFC;
            uint16_t lo = busData->read(addr_abs + 0);
            uint16_t hi = busData->read(addr_abs + 1);
            cpuData->pc = (hi << 8) | lo;

            cpuData->statusData = 0;
            cpuData->status.U = 1;
            cpuData->a = 0;
            cpuData->x = 0;
            cpuData->y = 0;
            cpuData->stkp = 0xFD;

	        cpuData->cycles = 8;
        }

        static void irq(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            bus<dataSize, addrSize> *busData = LTE::entityManger::getEntityById(SystemData)->getComponent<bus<dataSize, addrSize>>();
            cpu6502 *cpuData = LTE::entityManger::getEntityById(SystemData)->getComponent<cpu6502>();

            if(cpuData->status.I != 0)
                return;

            busData->write(0x0100 + cpuData->stkp, (cpuData->pc >> 8) & 0x00FF);
            cpuData->stkp--;
            busData->write(0x0100 + cpuData->stkp, cpuData->pc & 0x00FF);
            cpuData->stkp--;

            cpuData->status.B = 0;
            cpuData->status.U = 1;
            cpuData->status.I = 1;

            busData->write(0x0100 + cpuData->stkp, cpuData->statusData);
            cpuData->stkp--;

            uint16_t addr_abs = 0xFFFE;
            uint16_t lo = busData->read(addr_abs + 0);
            uint16_t hi = busData->read(addr_abs + 1);
            cpuData->pc = (hi << 8) | lo;
	        cpuData->cycles = 7;

        }

        static void nmi(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            bus<dataSize, addrSize> *busData = LTE::entityManger::getEntityById(SystemData)->getComponent<bus<dataSize, addrSize>>();
            cpu6502 *cpuData = LTE::entityManger::getEntityById(SystemData)->getComponent<cpu6502>();

            busData->write(0x0100 + cpuData->stkp, (cpuData->pc >> 8) & 0x00FF);
            cpuData->stkp--;
            busData->write(0x0100 + cpuData->stkp, cpuData->pc & 0x00FF);
            cpuData->stkp--;

            cpuData->status.B = 0;
            cpuData->status.U = 1;
            cpuData->status.I = 1;

            busData->write(0x0100 + cpuData->stkp, cpuData->statusData);
            cpuData->stkp--;

            uint16_t addr_abs = 0xFFFA;
            uint16_t lo = busData->read(addr_abs + 0);
            uint16_t hi = busData->read(addr_abs + 1);
            cpuData->pc = (hi << 8) | lo;
	        cpuData->cycles = 8;
        }

};