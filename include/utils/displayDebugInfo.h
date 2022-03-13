#pragma once
#include "LaughTaleEngine.h"
#include "6502.h"
#include "cpu.h"
#include "bus.h"
#include "imgui.h"

class displayDebugInfo
{
private:
    static inline int page;
    static inline bool showDebugInfo = false;
    static inline LTE::windowPieceId winId;
    static inline LTE::entityTaleId id;
    static inline LTE::entityTaleId colorPlateid;
    static inline LTE::entityTaleId patternMemoryId;
    static inline int pallateNumber = 0;

    static std::string toHexString(uint32_t n, uint8_t d)
    {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    }

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

    static void ImGuiRender(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
    {
        LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);

        cpu6502 *c = eventEntity->getComponent<cpu6502>();
        ppu2c02 *p = eventEntity->getComponent<ppu2c02>();
        bus<uint8_t, uint16_t> *sysBus = eventEntity->getComponent<bus<uint8_t, uint16_t>>();

        std::array<uint8_t, 0xFF> pageData;
        for (uint16_t i = 0; i <= 0xFF; i++)
            pageData[i] = sysBus->read((page << 8) | i);

        uint16_t pc = c->pc;

        uint16_t codeStart = pc;
        for (int i = 0; i < 11; i++)
        {
            codeStart -= c->lookup[sysBus->read(codeStart)].size + 1;
        }

        if (!ImGui::Begin("debug info"))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }


        ImGui::TextColored(ImVec4(1, 1, 0, 1), "flags");
        ImGui::BeginChild("###scrolling", ImVec2(0, 220));

        ImGui::Text("STATUS:");
        ImGui::TextColored(ImVec4(1 - c->status.N, c->status.N, 0, 1), "N");
        ImGui::TextColored(ImVec4(1 - c->status.V, c->status.V, 0, 1), "V");
        ImGui::TextColored(ImVec4(1 - c->status.U, c->status.U, 0, 1), "U");
        ImGui::TextColored(ImVec4(1 - c->status.B, c->status.B, 0, 1), "B");
        ImGui::TextColored(ImVec4(1 - c->status.D, c->status.D, 0, 1), "D");
        ImGui::TextColored(ImVec4(1 - c->status.I, c->status.I, 0, 1), "I");
        ImGui::TextColored(ImVec4(1 - c->status.Z, c->status.Z, 0, 1), "Z");
        ImGui::TextColored(ImVec4(1 - c->status.C, c->status.C, 0, 1), "C");

        ImGui::Text("A: $%x [%d]", c->a, c->a);
        ImGui::Text("X: $%x [%d]", c->x, c->x);
        ImGui::Text("Y: $%x [%d]", c->y, c->y);
        ImGui::Text("Stack P: $00%x", c->stkp);

        ImGui::EndChild();

        ImGui::TextColored(ImVec4(1, 1, 0, 1), "disassemble code");
        ImGui::BeginChild("##scrolling", ImVec2(0, 150));
        std::string temp = " ";
        if (c->lookup[sysBus->read(pc)].size > 0)
            temp += toHexString(sysBus->read(pc + 1), 2) + " ";
        if (c->lookup[sysBus->read(pc)].size > 1)
            temp += toHexString(sysBus->read(pc + 2), 2);
            
        ImGui::TextColored(ImVec4(0.5, 0.5, 0.9, 1), (
                                                         toHexString(pc, 4) + ":\t" +
                                                         c->lookup[sysBus->read(pc)].name + temp)
                                                         .c_str());
        for (int i = 0; i < 10; i++)
        {
            temp = " ";
            if (c->lookup[sysBus->read(codeStart)].size > 0)
                temp += toHexString(sysBus->read(codeStart + 1), 2) + " ";
            if (c->lookup[sysBus->read(codeStart)].size > 1)
                temp += toHexString(sysBus->read(codeStart + 2), 2);

            ImGui::Text((
                            toHexString(codeStart, 4) + ":\t" +
                            c->lookup[sysBus->read(codeStart)].name + temp)
                            .c_str());
            codeStart += c->lookup[sysBus->read(codeStart)].size + 1;
        }

        temp = " ";
        if (c->lookup[sysBus->read(pc)].size > 0)
            temp += toHexString(sysBus->read(pc + 1), 2) + " ";
        if (c->lookup[sysBus->read(pc)].size > 1)
            temp += toHexString(sysBus->read(pc + 2), 2);

        ImGui::TextColored(ImVec4(0.5, 0.5, 0.9, 1), (
                                                         toHexString(pc, 4) + ":\t" +
                                                         c->lookup[sysBus->read(pc)].name + temp)
                                                         .c_str());

        codeStart += c->lookup[sysBus->read(pc)].size + 1;

        for (int i = 0; i < 10; i++)
        {
            std::string temp = " ";
            if (c->lookup[sysBus->read(codeStart)].size > 0)
                temp += toHexString(sysBus->read(codeStart + 1), 2) + " ";
            if (c->lookup[sysBus->read(codeStart)].size > 1)
                temp += toHexString(sysBus->read(codeStart + 2), 2);

            ImGui::Text((
                            toHexString(codeStart, 4) + ":\t" +
                            c->lookup[sysBus->read(codeStart)].name + temp)
                            .c_str());
            codeStart += c->lookup[sysBus->read(codeStart)].size + 1;
        }

        ImGui::EndChild();

        ImGui::TextColored(ImVec4(1, 1, 0, 1), "ram data");
        ImGui::BeginChild("#scrolling", ImVec2(0, 150));
        for (int i = 0; i <= 0xFF; i += 16)
        {
            ImGui::Text((
                            toHexString((page << 8) | i, 4) + ":\t" +
                            toHexString(pageData[i + 0], 2) + " " +
                            toHexString(pageData[i + 1], 2) + " " +
                            toHexString(pageData[i + 2], 2) + " " +
                            toHexString(pageData[i + 3], 2) + " " +
                            toHexString(pageData[i + 4], 2) + " " +
                            toHexString(pageData[i + 5], 2) + " " +
                            toHexString(pageData[i + 6], 2) + " " +
                            toHexString(pageData[i + 7], 2) + "\t" +
                            toHexString(pageData[i + 8], 2) + " " +
                            toHexString(pageData[i + 9], 2) + " " +
                            toHexString(pageData[i + 10], 2) + " " +
                            toHexString(pageData[i + 11], 2) + " " +
                            toHexString(pageData[i + 12], 2) + " " +
                            toHexString(pageData[i + 13], 2) + " " +
                            toHexString(pageData[i + 14], 2) + " " +
                            toHexString(pageData[i + 15], 2) + " ")
                            .c_str());
        }
        ImGui::EndChild();

        ImGui::SliderInt("page num", &page, 0, 0x1F);

        ImGui::End();
    }

    static void updateColorPalate(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
    {
        LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
        ppu2c02 *p = eventEntity->getComponent<ppu2c02>();
        LTE::texture *t =  LTE::entityManger::getEntityById(colorPlateid)->getComponent<LTE::material>()->getTexture();


        int x = 0, y = 0;
        for (uint64_t i = 0x3F00; i < 0x3F20; i++)
        {
            uint8_t j = p->ppuBus.read(i);
            t->setRGBValue({y, x}, colorPalate[j]);
            x++;
            if(x == 4)
            {
                x = 0;
                y++;
            }
        }
    }
    
    static void updatePatternMemory(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
    {
        LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
        ppu2c02 *p = eventEntity->getComponent<ppu2c02>();
        LTE::texture *t =  LTE::entityManger::getEntityById(patternMemoryId)->getComponent<LTE::material>()->getTexture();


        int x = 0, y = 0;
        for (uint64_t i = 0x0000; i < 0x1000; i+=16)
        {
            for(int k = 0; k < 8; k++)
            {
                uint8_t LSB = p->ppuBus.read(i + 0 + k);
                uint8_t MSB = p->ppuBus.read(i + 8 + k);
                
                uint8_t c;
                for (int n = 7; n >= 0; n--)
                { 
                    c = ((MSB & 1) << 1) | (LSB & 1);
                    MSB >>= 1;
                    LSB >>= 1;

                    uint8_t j = p->ppuBus.read(0x3F00 + c + pallateNumber * 3);
                    t->setRGBValue({x + n, y + k}, colorPalate[j]);

                }
            }
            
            x+=8;
            if(x >=  16 * 8)
            {
                x = 0;
                y+=8;
            }
        }

        x = 16 * 8, y = 0;
        for (uint64_t i = 0x1000; i < 0x2000 ; i+=16)
        {
            for(int k = 0; k < 8; k++)
            {
                uint8_t LSB = p->ppuBus.read(i + 0 + k);
                uint8_t MSB = p->ppuBus.read(i + 8 + k);
                
                uint8_t c;
                for (int n = 7; n >= 0; n--)
                { 
                    c = ((MSB & 1) << 1) | (LSB & 1);
                    MSB >>= 1;
                    LSB >>= 1;

                    uint8_t j = p->ppuBus.read(0x3F00 + c + pallateNumber * 3);
                    t->setRGBValue({x + n, y + k}, colorPalate[j]);

                }
            }
            
            x+=8;
            if(x >=  16 * 8 * 2)
            {
                x = 16 * 8;
                y+=8;
            }
        }
    }

    static inline float tilePostions[12] =
    {
        0.0, 1.0f, 0.0f,
        0.0, 0.0f, 0.0f,
        1.0, 0.0f, 0.0f,
        1.0, 1.0f, 0.0f
    };

    static inline unsigned int tileIndices[6] =
    {
        0,
        1,
        2,
        0,
        3,
        2,
    };

public:

    
    static void changePallate()
    {
        LTE::texture *t =  LTE::entityManger::getEntityById(colorPlateid)->getComponent<LTE::material>()->getTexture();

        t->setRGBValue({pallateNumber, 4}, new uint8_t[3]{0, 0, 0});
        pallateNumber = (pallateNumber + 1) % 8;
        t->setRGBValue({pallateNumber, 4}, new uint8_t[3]{255, 255, 255});

    }
    static bool isInfoShown() { return showDebugInfo; }

    static void init(LTE::windowPieceId winId, LTE::entityTaleId id)
    {
        displayDebugInfo::winId = winId;
        displayDebugInfo::id = id;

        LTE::eventManger::startBuildingEvent()->setEventRoute("Key pressed/toggleDebugInfo")->setEventCallback(toggleDebugInfoOnKey)->add();

        LTE::texture *colorPalateT = LTE::windowManger::getWindow(winId)->context->getMeshFactory()->createCustemTexture({8, 6});
        LTE::texture *patternMemoryT = LTE::windowManger::getWindow(winId)->context->getMeshFactory()->createCustemTexture({256, 128});

        LTE::windowManger::getWindow(winId)->assetLibrary->saveAsset(colorPalateT, "res/colorPalateTexture");
        LTE::windowManger::getWindow(winId)->assetLibrary->saveAsset(patternMemoryT, "res/patternMemoryTexture");


        colorPlateid =  LTE::entityManger::addEntity(
            [&](LTE::gameObject::gameObjectBuilder *b)
            {
                b->setObjectName("color palate display")->
                    setWindowId(winId)->
                    setObjectTransform({{0, -0.3f, 0}, {0, 0, 0}, {1.77778f, 0.2f, 0}})->
                    addComponent(LTE::mesh::build([&](LTE::mesh::meshBuilder *builder)
                        { 
                            builder->
                                setIndexBuffer(tileIndices, 6)->
                                setShaderName("res/shaders/Basic.glsl")->
                                setVertices(tilePostions, 12); 
                        }))->
                    addComponent(new LTE::material("res/colorPalateTexture", glm::vec4(0, 0, 0, 1.0f)));
            });


        patternMemoryId =  LTE::entityManger::addEntity(
            [&](LTE::gameObject::gameObjectBuilder *b)
            {
                b->setObjectName("pattern memory display")->
                    setWindowId(winId)->
                    setObjectTransform({{0, -1, 0}, {0, 0, 0}, {1.77778f, 0.7f, 0}})->
                    addComponent(LTE::mesh::build([&](LTE::mesh::meshBuilder *builder)
                        { 
                            builder->
                                setIndexBuffer(tileIndices, 6)->
                                setShaderName("res/shaders/Basic.glsl")->
                                setVertices(tilePostions, 12); 
                        }))->
                    addComponent(new LTE::material("res/patternMemoryTexture", glm::vec4(0, 0, 0, 1.0f)));
            });
        toggleDebugInfo();
    }

    static void toggleDebugInfo()
    {
        if (showDebugInfo)
        {
            LTE::eventManger::removeEvent("ImGui render/print debug info");
            LTE::eventManger::removeEvent("window render/nes emulator/show color palate");
            LTE::eventManger::removeEvent("window render/nes emulator/show name table");
        }
        else
        {
            LTE::eventManger::startBuildingEvent()->setEventRoute("ImGui render/print debug info")->setEventCallback(ImGuiRender)->setEntityID(id)->setWindowId(winId)->add();
            LTE::eventManger::startBuildingEvent()->setEventRoute("window render/nes emulator/show color palate")->setEventCallback(updateColorPalate)->setEntityID(id)->setWindowId(winId)->add();
            LTE::eventManger::startBuildingEvent()->setEventRoute("window render/nes emulator/show name table")->setEventCallback(updatePatternMemory)->setEntityID(id)->setWindowId(winId)->add();
        }
        showDebugInfo = !showDebugInfo;
    }

    static void toggleDebugInfoOnKey(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
    {
        LTE::KeyData *eventData = static_cast<LTE::KeyData *>(sendor);

        if (eventData->key == LT_KEY_F1)
            toggleDebugInfo();
    }
};
