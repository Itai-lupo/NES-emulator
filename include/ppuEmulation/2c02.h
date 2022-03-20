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
    bus<uint8_t, uint16_t> ppuBus; // pattern(0x0000 - 0x1FFF), name table(0x2000 - 0x2FFF), PALETTES( 0x3F00 - 0x3FFF)

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

    union loopy_register
    {
        // Credit to Loopy for working this out :D
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

    loopy_register vram_addr; // Active "pointer" address into nametable to extract background tile info
    loopy_register tram_addr; // Temporary store of information to be "transferred" into "pointer" at various times

    uint8_t fine_x = 0x00;

    // Pixel "dot" position information
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
            break;

            // OAM Data
        case 0x0004:
            break;

            // Scroll - Not Readable
        case 0x0005:
            break;

            // PPU Address - Not Readable
        case 0x0006:
            break;

            // PPU Data
        case 0x0007:
            // Reads from the NameTable ram get delayed one cycle,
            // so output buffer which contains the data from the
            // previous read request
            data = ppu_data_buffer;
            // then update the buffer for next time
            ppu_data_buffer = ppuBus.read(vram_addr.reg);
            // However, if the address was in the palette range, the
            // data is not delayed, so it returns immediately
            if (vram_addr.reg >= 0x3F00)
                data = ppu_data_buffer;
            // All reads from PPU data automatically increment the nametable
            // address depending upon the mode set in the control register.
            // If set to vertical mode, the increment is 32, so it skips
            // one whole nametable row; in horizontal mode it just increments
            // by 1, moving to the next column
            vram_addr.reg += (control.increment_mode ? 32 : 1);
            break;
        }
        return data;
    }

    virtual void write(uint16_t addr, uint8_t data) override
    {
        addr &= 0x0007;
        // LAUGHTALE_ENGINR_LOG_INFO((int)addr << ", " << (int)data)
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
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            if (address_latch == 0)
            {
                // First write to scroll register contains X offset in pixel space
                // which we split into coarse and fine x values
                fine_x = data & 0x07;
                tram_addr.coarse_x = data >> 3;
                address_latch = 1;
            }
            else
            {
                // First write to scroll register contains Y offset in pixel space
                // which we split into coarse and fine Y values
                tram_addr.fine_y = data & 0x07;
                tram_addr.coarse_y = data >> 3;
                address_latch = 0;
            }
            break;
        case 0x0006: // PPU Address
            if (address_latch == 0)
            {
                // PPU address bus can be accessed by CPU via the ADDR and DATA
                // registers. The fisrt write to this register latches the high byte
                // of the address, the second is the low byte. Note the writes
                // are stored in the tram register...
                tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
                address_latch = 1;
            }
            else
            {
                // ...when a whole address has been written, the internal vram address
                // buffer is updated. Writing to the PPU is unwise during rendering
                // as the PPU will maintam the vram address automatically whilst
                // rendering the scanline position.
                tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
                vram_addr = tram_addr;
                address_latch = 0;
            }
            break;
        case 0x0007: // PPU Data
            ppuBus.write(vram_addr.reg, data);
            // All writes from PPU data automatically increment the nametable
            // address depending upon the mode set in the control register.
            // If set to vertical mode, the increment is 32, so it skips
            // one whole nametable row; in horizontal mode it just increments
            // by 1, moving to the next column
            vram_addr.reg += (control.increment_mode ? 32 : 1);
            break;
        }
    }
};