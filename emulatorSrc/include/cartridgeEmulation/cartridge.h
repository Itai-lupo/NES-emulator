#pragma once
#include "busDevice.h"

#include <cstdint>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

#include "NESError.h"
#include "mapper.h"

struct NESHeader
{
    char name[4];
    uint8_t prg_rom_chunks;
    uint8_t chr_rom_chunks;
    uint8_t mapper1;
    uint8_t mapper2;
    uint8_t prg_ram_size;
    uint8_t tv_system1;
    uint8_t tv_system2;
    char unused[5];
};


class cartridge
{
    private:
        mapper *cartMapper = nullptr;

        NESHeader header;
        uint8_t mapperId;        
        uint8_t PRGBanks = 0;
        uint8_t CHRBanks = 0;
    
        bool thereIsALoadedCart = false;

        std::vector<uint8_t> PRGMemory;
        std::vector<uint8_t> CHRMemory;

        MIRROR mirror = HORIZONTAL;

    public:
        cartridge(){}
        virtual ~cartridge() = default;

        void load(const std::string& romPath);
        void unload();

        uint8_t readPRGMemory(uint32_t addr);
        uint8_t readCHRMemory(uint32_t addr);

        void writePRGMemory(uint32_t addr, uint8_t data);
        void writeCHRMemory(uint32_t addr, uint8_t data);

        MIRROR getMirror();

    private:
        std::ifstream ifs;
        uint8_t fileType;

        void readHeader();

        void handleFileTypes();
        void handleFileType0();
        void handleFileType1();
        void handleFileType2();

        void initMapper();

};
