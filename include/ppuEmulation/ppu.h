#pragma once
#include "LaughTaleEngine.h"

#include "2c02.h"
#include "bus.h"

#include <thread>

class ppu
{
    private:

        static inline uint8_t cycles = 0X00;

    public:

        static inline const uint8_t colorPalate[0x40][3] =
        {
            { 84,   84,     84  },
            { 0,    30,     116 },
            { 8,    16,     144 },
            { 48,   0,      136 },
            { 68,   0,      100 },
            { 92,   0,      48  },
            { 84,   4,      0   },
            { 60,   24,     0   },
            { 32,   42,     0   },
            { 8,    58,     0   },
            { 0,    64,     0   },
            { 0,    60,     0   },
            { 0,    50,     60  },
            { 0,    0,      0   },
            { 0,    0,      0   },
            { 0,    0,      0   },


            { 152,  150,    152 },
            { 8,    76,     196 },
            { 48,   50,     236 },
            { 92,   30,     228 },
            { 136,  20,     176 },
            { 160,  20,     100 },
            { 152,  34,     32  },
            { 120,  60,     0   },
            { 84,   90,     0   },
            { 40,   114,    0   },
            { 8,    124,    0   },
            { 0,    118,    40  },
            { 0,    102,    120 },
            { 0,    0,      0   },
            { 0,    0,      0   },
            { 0,    0,      0   },

            { 236,  238,    236 },
            { 76,   154,    236 },
            { 120,  124,    236 },
            { 176,  98,     236 },
            { 228,  84,     236 },
            { 236,  88,     180 },
            { 236,  106,    100 },
            { 212,  136,    32  },
            { 160,  170,    0   },
            { 116,  196,    0   },
            { 76,   208,    32  },
            { 56,   204,    108 },
            { 56,   180,    204 },
            { 60,   60,     60  },
            { 0,    0,      0   },
            { 0,    0,      0   },

            { 236,  238,    236 },
            { 168,  204,    236 },
            { 188,  188,    236 },
            { 212,  178,    236 },
            { 236,  174,    236 },
            { 236,  174,    212 },
            { 236,  180,    176 },
            { 228,  196,    144 },
            { 204,  210,    120 },
            { 180,  222,    120 },
            { 168,  226,    144 },
            { 152,  226,    180 },
            { 160,  214,    228 },
            { 160,  162,    160 },
            { 0,    0,      0   },
            { 0,    0,      0   }
        };

        static inline bool frameComplete = false;
        static inline bool nmi = false;
        static bool isCpuComplete(){ return cycles == 0; }

        static void init(LTE::entityTaleId SystemDataId)
        {
            
        }

        static void close()
        {

        }



        static uint8_t GetColourFromPaletteRam(ppu2c02 *ppuData, uint8_t palette, uint8_t pixel)
        {
            return ppuData->ppuBus.read(0x3F00 + (palette << 2) + pixel) & 0x3F;
        }

        
        static void updateShifters(ppu2c02 *ppuData)
        {
            ppuData->bg_shifter_pattern_lo <<= 1;
            ppuData->bg_shifter_pattern_hi <<= 1;

            ppuData->bg_shifter_attrib_lo <<= 1;
            ppuData->bg_shifter_attrib_hi <<= 1;
        }

        static void loadBackgroundShifters(ppu2c02 *ppuData)
        {
            ppuData->bg_shifter_pattern_lo = (ppuData->bg_shifter_pattern_lo & 0xFF00) | ppuData->bg_next_tile_lsb;
            ppuData->bg_shifter_pattern_hi = (ppuData->bg_shifter_pattern_hi & 0xFF00) | ppuData->bg_next_tile_msb;


            ppuData->bg_shifter_attrib_lo  = (ppuData->bg_shifter_attrib_lo & 0xFF00) | ((ppuData->bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
            ppuData->bg_shifter_attrib_hi  = (ppuData->bg_shifter_attrib_hi & 0xFF00) | ((ppuData->bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
        }

        static void incrementScrollX(ppu2c02 *ppuData)
        {
            if (ppuData->mask.render_background || ppuData->mask.render_sprites)
            {
                if (ppuData->vram_addr.coarse_x == 31)
                {
                    ppuData->vram_addr.coarse_x = 0;
                    ppuData->vram_addr.nametable_x = ~ppuData->vram_addr.nametable_x;
                }
                else
                {
                    ppuData->vram_addr.coarse_x++;
                }
            }
        }

        static void incrementScrollY(ppu2c02 *ppuData)
        {
            if (ppuData->mask.render_background || ppuData->mask.render_sprites)
            {
                if (ppuData->vram_addr.fine_y < 7)
                {
                    ppuData->vram_addr.fine_y++;
                }
                else
                {
                    ppuData->vram_addr.fine_y = 0;

                    if (ppuData->vram_addr.coarse_y == 29)
                    {
                        ppuData->vram_addr.coarse_y = 0;
                        ppuData->vram_addr.nametable_y = ~ppuData->vram_addr.nametable_y;
                    }
                    else if (ppuData->vram_addr.coarse_y == 31)
                    {
                        ppuData->vram_addr.coarse_y = 0;
                    }
                    else
                    {
                        ppuData->vram_addr.coarse_y++;
                    }
                }
            }
        }

        static void transferAddressX(ppu2c02 *ppuData)
        {
            if (ppuData->mask.render_background || ppuData->mask.render_sprites)
            {
                ppuData->vram_addr.nametable_x = ppuData->tram_addr.nametable_x;
                ppuData->vram_addr.coarse_x    = ppuData->tram_addr.coarse_x;
            }
        }

        static void transferAddressY(ppu2c02 *ppuData)
        {
            if (ppuData->mask.render_background || ppuData->mask.render_sprites)
            {
                ppuData->vram_addr.nametable_y = ppuData->tram_addr.nametable_y;
                ppuData->vram_addr.coarse_y    = ppuData->tram_addr.coarse_y;
                ppuData->vram_addr.fine_y    = ppuData->tram_addr.fine_y;
            }
        }

        static void loadNametableByte(ppu2c02 *ppuData)
        {
            ppuData->bg_next_tile_id = ppuData->ppuBus.read(0x2000 | (ppuData->vram_addr.reg & 0x0FFF));
        }

        static void loadAtributeTableByte(ppu2c02 *ppuData)
        {
            ppuData->bg_next_tile_attrib = ppuData->ppuBus.read(0x23C0 | 
                                                            (ppuData->vram_addr.nametable_y << 11) |
                                                            (ppuData->vram_addr.nametable_x << 10) |
                                                            ((ppuData->vram_addr.coarse_y >> 2 ) << 3) |
                                                            (ppuData->vram_addr.coarse_x >> 2)
                                                            );		

            if (ppuData->vram_addr.coarse_y & 0x02) ppuData->bg_next_tile_attrib >>= 4;
            if (ppuData->vram_addr.coarse_x & 0x02) ppuData->bg_next_tile_attrib >>= 2;
            ppuData->bg_next_tile_attrib &= 0x03;
        }

        static void patternTableTileLow(ppu2c02 *ppuData)
        {
            ppuData->bg_next_tile_lsb = ppuData->ppuBus.read(
                                    (ppuData->control.pattern_background << 12) + 
                                    ((uint16_t)ppuData->bg_next_tile_id << 4) + 
                                    ppuData->vram_addr.fine_y + 0);

        }

        static void patternTableTileHigh(ppu2c02 *ppuData)
        {
            ppuData->bg_next_tile_msb = ppuData->ppuBus.read(
                                    (ppuData->control.pattern_background << 12) + 
                                    ((uint16_t)ppuData->bg_next_tile_id << 4) + 
                                    ppuData->vram_addr.fine_y + 8);

        }

        static void visableAreaRender(ppu2c02 *ppuData)
        {
            updateShifters(ppuData);
            switch ((ppuData->cycle - 1) % 8)
            {
                case 0:
                    loadBackgroundShifters(ppuData);
                    loadNametableByte(ppuData);
                    break;

                case 2:
                    loadAtributeTableByte(ppuData);
                    break;
                case 4:
                    patternTableTileLow(ppuData);
                    break;
                case 6:
                    patternTableTileHigh(ppuData);
                    break;
                case 7:
                    incrementScrollX(ppuData);
                    break;
                
            }
        }

        static uint8_t getBGPixelPaletteOffset(ppu2c02 *ppuData)
        {
                    
            uint16_t bit_mux = 0x8000 >> ppuData->fine_x;

            uint8_t p0_pixel = (ppuData->bg_shifter_pattern_lo & bit_mux) > 0;
            uint8_t p1_pixel = (ppuData->bg_shifter_pattern_hi & bit_mux) > 0;

            return (p1_pixel << 1) | p0_pixel;
        }

        static uint8_t getBGPixelPalette(ppu2c02 *ppuData)
        {
            uint16_t bit_mux = 0x8000 >> ppuData->fine_x;

            uint8_t bg_pal0 = (ppuData->bg_shifter_attrib_lo & bit_mux) > 0;
            uint8_t bg_pal1 = (ppuData->bg_shifter_attrib_hi & bit_mux) > 0;
            return (bg_pal1 << 1) | bg_pal0;
        }

        static void increaseCycle(ppu2c02 *ppuData)
        {
            ppuData->cycle++;
            if (ppuData->cycle >= 341)
            {
                ppuData->cycle = 0;
                ppuData->scanline++;

                if (ppuData->scanline > 261)
                {
                    ppuData->scanline = 0;
                    frameComplete = true;
                }
            }
        }

        static void clock(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {

            
            bus<uint8_t, uint16_t> *busData = eventEntity->getComponent<bus<uint8_t, uint16_t>>();
            ppu2c02 *ppuData = eventEntity->getComponent<ppu2c02>();
            LTE::texture *t =  eventEntity->getComponent<LTE::material>()->getTexture();
        
        
            for (size_t i = 0; i < 3; i++)
            {

                if (ppuData->scanline == 0 && ppuData->cycle == 0)
                        ppuData->cycle = 1;

                else if(ppuData->scanline == 261 && ppuData->cycle == 1)
                    ppuData->status.vertical_blank = 0;


                if (ppuData->scanline < 240 || ppuData->scanline == 261 )
                {
                    if((ppuData->cycle >= 2 && ppuData->cycle < 258) || (ppuData->cycle >= 321 && ppuData->cycle < 338))
                    {
                        visableAreaRender(ppuData);
                    }
                    
                    if(ppuData->cycle == 256)
                    {	
                        incrementScrollY(ppuData);
                    }
                    else if(ppuData->cycle == 257)
                    {
                        loadBackgroundShifters(ppuData);
                        transferAddressX(ppuData);
                    }
                    else if (ppuData->cycle == 338 || ppuData->cycle == 340)
                    {
                        ppuData->bg_next_tile_id = ppuData->ppuBus.read(0x2000 | (ppuData->vram_addr.reg & 0x0FFF));
                    }
                    else if(ppuData->scanline == 261 && ppuData->cycle >= 280 && ppuData->cycle < 305)
                    {
                        transferAddressY(ppuData);
                    }
                }
                
                if(ppuData->scanline == 241 && ppuData->cycle == 1)
                {
                    ppuData->status.vertical_blank = 1;
                    if(ppuData->control.enable_nmi){
                        nmi = true;
                    }
                }

                uint8_t bg_pixel = 0x00;  
                uint8_t bg_palette = 0x00;

                if (ppuData->mask.render_background)
                {
                    bg_pixel = getBGPixelPaletteOffset(ppuData);
                    bg_palette = getBGPixelPalette(ppuData);
                }
                
                if(ppuData->scanline < 240 && ppuData->cycle < 256)
                {    
                    t->setRGBValue(
                        {ppuData->cycle - 1, ppuData->scanline}, 
                        colorPalate[GetColourFromPaletteRam(ppuData, bg_palette, bg_pixel)]);
                }
                
                increaseCycle(ppuData);

            }            
        }
        
};
