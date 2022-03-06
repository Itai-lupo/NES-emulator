#include "cartridge.h"
#include "mappersFacory.h"

void cartridge::load(const std::string& romPath)
{
    ifs.open(romPath, std::ifstream::binary);
    
    if(!ifs.is_open())
        throw new romFileNotFoundException(romPath);
        
    readHeader();
    handleFileTypes();
    initMapper();

    ifs.close();
    thereIsALoadedCart = true;
}

uint8_t cartridge::readPRGMemory(uint16_t addr)
{
    if(thereIsALoadedCart && cartMapper->readPRGMemory(addr))
        return PRGMemory[addr];
    return 0;
}

uint8_t cartridge::readCHRMemory(uint16_t addr)
{
    if(thereIsALoadedCart && cartMapper->readCHRMemory(addr))
        return CHRMemory[addr];
    return 0;
}

void cartridge::writePRGMemory(uint16_t addr, uint8_t data)
{
    if(thereIsALoadedCart && cartMapper->writePRGMemory(addr))
        PRGMemory[addr] = data;
}

void cartridge::writeCHRMemory(uint16_t addr, uint8_t data)
{
    if(thereIsALoadedCart && cartMapper->writeCHRMemory(addr))
        CHRMemory[addr] = data;
}

void cartridge::readHeader()
{
    ifs.read((char*)&header, sizeof(NESHeader));

    char nameValid[5] = {'N', 'E', 'S', 0x1A};
    if(strncmp(header.name, nameValid, 4) != 0)
        throw new badRomHeaderException("rom not start with NES error");

    
    if (header.mapper1 & 0x04)
        ifs.seekg(512, std::ios_base::cur);
    
    mapperId = (header.mapper2 & 0xF0) | (header.mapper1 >> 4);
    mirror = (header.mapper1 & 0x01) ? VERTICAL: HORIZONTAL;

    fileType = 1;
}

void cartridge::handleFileTypes()
{
    if (fileType == 0)
        handleFileType0();

    else if (fileType == 1)
        handleFileType1();

    else if (fileType == 2)
        handleFileType2();
}

void cartridge::handleFileType0()
{

}

void cartridge::handleFileType1()
{
    PRGBanks = header.prg_rom_chunks;
    PRGMemory.resize(PRGBanks * 0x4000);
    ifs.read((char*)PRGMemory.data(), PRGMemory.size());

    CHRBanks = header.chr_rom_chunks;
    CHRMemory.resize(CHRBanks * 0x2000);
    ifs.read((char*)CHRMemory.data(), CHRMemory.size());

}

void cartridge::handleFileType2()
{

}

void cartridge::initMapper()
{
    cartMapper = mappersFacory::createMapper({mapperId, PRGBanks, CHRBanks});
}


