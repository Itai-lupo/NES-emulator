#pragma once
#include "LaughTaleEngine.h"
#include "busDevice.h"
#include <cstdint>

#include <array>

#include "bus.h"
#include "ppuBusCartridge.h"
#include "nameTable.h"
#include "palettesMemory.h"

class ppu2c02 : public busDevice<uint8_t, uint16_t>, public LTE::component
{
    public:
        std::array<uint8_t, 0x1FFF> ppuData;
        
        // pattern(0x0000 - 0x1FFF), name table(0x2000 - 0x2FFF), PALETTES( 0x3F00 - 0x3FFF)
        bus<uint8_t, uint16_t> ppuBus; 

        union
        {
            struct
            {
                uint8_t y;
                uint8_t tileId;
                union
                {
                    struct
                    {
                        uint8_t palette: 2;  
                        uint8_t U: 3;  
                        uint8_t priority: 1;  
                        uint8_t flipHorizontally: 1;  
                        uint8_t flipVertically: 1;  
                    };
                    uint8_t attribute;  
                };
                uint8_t x;
                
            } OAM[64];
            uint8_t pOAM[256];
        };

        uint8_t oamAddr = 0x00;
        

        union
        {
            struct
            {
                uint8_t unused : 5;
                uint8_t sprite_overflow : 1;
                uint8_t sprite_zero_hit : 1;
                uint8_t vertical_blank : 1;
            };

            uint8_t reg;
        } status;

        union
        {
            struct
            {
                uint8_t grayscale : 1;
                uint8_t render_background_left : 1;
                uint8_t render_sprites_left : 1;
                uint8_t render_background : 1;
                uint8_t render_sprites : 1;
                uint8_t enhance_red : 1;
                uint8_t enhance_green : 1;
                uint8_t enhance_blue : 1;
            };

            uint8_t reg;
        } mask;

        union PPUCTRL
        {
            struct
            {
                uint8_t nametable_x : 1;
                uint8_t nametable_y : 1;
                uint8_t increment_mode : 1;
                uint8_t pattern_sprite : 1;
                uint8_t pattern_background : 1;
                uint8_t sprite_size : 1;
                uint8_t slave_mode : 1; // unused
                uint8_t enable_nmi : 1;
            };

            uint8_t reg;
        } control;

        // Credit to Loopy for working this out :D
        union loopy_register
        {
            struct
            {
                uint16_t coarse_x : 5;
                uint16_t coarse_y : 5;
                uint16_t nametable_x : 1;
                uint16_t nametable_y : 1;
                uint16_t fine_y : 3;
                uint16_t unused : 1;
            };

            uint16_t reg = 0x0000;
        };

        loopy_register vram_addr;
        loopy_register tram_addr;

        uint8_t fine_x = 0x00;

        int16_t scanline = 0;
        int16_t cycle = 0;

        uint8_t address_latch = 0x00;
        uint8_t ppu_data_buffer = 0x00;

        uint8_t bg_next_tile_id = 0x00;
        uint8_t bg_next_tile_attrib = 0x00;
        uint8_t bg_next_tile_lsb = 0x00;
        uint8_t bg_next_tile_msb = 0x00;
        uint16_t bg_shifter_pattern_lo = 0x0000;
        uint16_t bg_shifter_pattern_hi = 0x0000;
        uint16_t bg_shifter_attrib_lo = 0x0000;
        uint16_t bg_shifter_attrib_hi = 0x0000;

        nameTable *nt;
        palettesMemory *pm;
        ppuBusCartridge *cartridge;

        std::string datatoHexString(uint32_t n, uint8_t d)
        {
            std::string s(d, '0');
            for (int i = d - 1; i >= 0; i--, n >>= 4)
                s[i] = "0123456789ABCDEF"[n & 0xF];
            return s;
        }

    public:
        ppu2c02(ppuBusCartridge *cartridge)
        {
            for (auto &i : ppuData)
                i = 0x00;
            nt = new nameTable();
            pm = new palettesMemory();
            this->cartridge = cartridge;

            nt->cart = cartridge;

            ppuBus.pushDevice(cartridge)->pushDevice(nt)->pushDevice(pm);
            reset();
        }

        void reset()
        {
            fine_x = 0x00;
            address_latch = 0x00;
            ppu_data_buffer = 0x00;
            scanline = 0;
            cycle = 0;
            bg_next_tile_id = 0x00;
            bg_next_tile_attrib = 0x00;
            bg_next_tile_lsb = 0x00;
            bg_next_tile_msb = 0x00;
            bg_shifter_pattern_lo = 0x0000;
            bg_shifter_pattern_hi = 0x0000;
            bg_shifter_attrib_lo = 0x0000;
            bg_shifter_attrib_hi = 0x0000;
            status.reg = 0x00;
            mask.reg = 0x00;
            control.reg = 0x00;
            vram_addr.reg = 0x0000;
            tram_addr.reg = 0x0000;
            oamAddr = 0x00;
        }

        ~ppu2c02()
        {
        }

        virtual void init(LTE::gameObject *parent) override {}

        virtual void end() override {}

        virtual bool isInRange(uint16_t addr) override
        {
            return addr >= 0x2000 && addr <= 0x3FFF;
        }

        virtual uint8_t read(uint16_t addr, bool bReadOnly = false) override
        {
            addr &= 0x0007;
            uint8_t data = 0x00;

            switch (addr)
            {
                // Control - Not readable
                case 0x0000:
                    break;

                    // Mask - Not Readable
                case 0x0001:
                    break;

                    // Status
                case 0x0002:
                    data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
                    status.vertical_blank = 0;
                    address_latch = 0;
                    break;

                    // OAM Address
                case 0x0003:
                    data = oamAddr;
                    break;

                    // OAM Data
                case 0x0004:
                    data = pOAM[oamAddr];
                    break;

                    // Scroll - Not Readable
                case 0x0005:
                    break;

                    // PPU Address - Not Readable
                case 0x0006:
                    break;

                    // PPU Data
                case 0x0007:
                    data = ppu_data_buffer;
                    ppu_data_buffer = ppuBus.read(vram_addr.reg);
                    if (vram_addr.reg >= 0x3F00)
                        data = ppu_data_buffer;
                    vram_addr.reg += (control.increment_mode ? 32 : 1);
                    break;
            }
            return data;
        }

        virtual void write(uint16_t addr, uint8_t data) override
        {
            addr = addr & 0x0007;
        
            switch (addr)
            {
                case 0x0000: // Control
                    control.reg = data;
                    tram_addr.nametable_x = control.nametable_x;
                    tram_addr.nametable_y = control.nametable_y;
                    break;
                case 0x0001: // Mask
                    mask.reg = data;
                    break;
                case 0x0002: // Status
                    break;
                case 0x0003: // OAM Address
                    oamAddr = data;
                    break;
                case 0x0004: // OAM Data
                    pOAM[oamAddr] = data; 
                    break;
                case 0x0005: // Scroll
                    if (address_latch == 0)
                    {
                        fine_x = data & 0x07;
                        tram_addr.coarse_x = data >> 3;
                        address_latch = 1;
                    }
                    else
                    {
                        tram_addr.fine_y = data & 0x07;
                        tram_addr.coarse_y = data >> 3;
                        address_latch = 0;
                    }
                    break;
                case 0x0006: // PPU Address
                    if (address_latch == 0)
                    {
                        tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
                        address_latch = 1;
                    }
                    else
                    {
                        tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
                        vram_addr = tram_addr;
                        address_latch = 0;
                    }
                    break;
                case 0x0007: // PPU Data
                    ppuBus.write(vram_addr.reg, data);
                    vram_addr.reg += (control.increment_mode ? 32 : 1);
                    break;
            }
        }
};