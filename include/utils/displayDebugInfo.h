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

        static std::string toHexString(uint32_t n, uint8_t d)
        {
            std::string s(d, '0');
            for (int i = d - 1; i >= 0; i--, n >>= 4)
                s[i] = "0123456789ABCDEF"[n & 0xF];
            return s;
        }
        
        static void ImGuiRender(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
            
            cpu6502 *c = eventEntity->getComponent<cpu6502>();
            bus<uint8_t, uint16_t> *sysBus = eventEntity->getComponent<bus<uint8_t, uint16_t>>();

            std::array<uint8_t, 0xFF> pageData;
            for(uint16_t i = 0; i <= 0xFF; i++)
                pageData[i] = sysBus->read((page << 8) | i);

            uint16_t pc = c->pc;

            uint16_t codeStart = pc;
            for(int i = 0; i < 11; i++)
            {
                codeStart -= c->lookup[sysBus->read(codeStart)].size + 1;
            }


            if (!ImGui::Begin("debug info"))
            {
                // Early out if the window is collapsed, as an optimization.
                ImGui::End();
                return;
            }

            ImGui::TextColored(ImVec4(1,1,0,1), "flags");
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


            ImGui::TextColored(ImVec4(1,1,0,1), "disassemble code");
            ImGui::BeginChild("##scrolling", ImVec2(0, 150));
            for(int i = 0; i < 10; i++)
            {
                std::string temp = " ";
                if(c->lookup[sysBus->read(codeStart)].size > 0)
                    temp += toHexString(sysBus->read(codeStart + 1), 2) + " ";
                if(c->lookup[sysBus->read(codeStart)].size > 1)
                    temp += toHexString(sysBus->read(codeStart + 2), 2);

                ImGui::Text((
                    toHexString(codeStart, 4) + ":\t" + 
                    c->lookup[sysBus->read(codeStart)].name + temp
                    ).c_str()
                );
                codeStart += c->lookup[sysBus->read(codeStart)].size + 1;
            }


            std::string temp = " ";
            if(c->lookup[sysBus->read(codeStart)].size > 0)
                temp += toHexString(sysBus->read(codeStart + 1), 2) + " ";
            if(c->lookup[sysBus->read(codeStart)].size > 1)
                temp += toHexString(sysBus->read(codeStart + 2), 2);

            ImGui::TextColored(ImVec4(0.5,0.5,0.9,1), (
                toHexString(codeStart, 4) + ":\t" + 
                c->lookup[sysBus->read(codeStart)].name + temp
                ).c_str()
            );

            codeStart += c->lookup[sysBus->read(codeStart)].size + 1;

            for(int i = 0; i < 10; i++)
            {
                std::string temp = " ";
                if(c->lookup[sysBus->read(codeStart)].size > 0)
                    temp += toHexString(sysBus->read(codeStart + 1), 2) + " ";
                if(c->lookup[sysBus->read(codeStart)].size > 1)
                    temp += toHexString(sysBus->read(codeStart + 2), 2);

                ImGui::Text((
                    toHexString(codeStart, 4) + ":\t" + 
                    c->lookup[sysBus->read(codeStart)].name + temp
                    ).c_str()
                );
                codeStart += c->lookup[sysBus->read(codeStart)].size + 1;
            }

            ImGui::EndChild();


            ImGui::TextColored(ImVec4(1,1,0,1), "ram data");
            ImGui::BeginChild("#scrolling", ImVec2(0, 150));
            for(int i = 0; i <= 0xFF; i += 16)
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
                    toHexString(pageData[i + 15], 2) + " "
                    ).c_str()
                );
            }
            ImGui::EndChild();



            ImGui::SliderInt("page num", &page, 0, 0x1F);
            ImGui::End();
        }

    public:
        static bool isInfoShown(){ return showDebugInfo; }
        
        static void init(LTE::windowPieceId winId, LTE::entityTaleId id){
            displayDebugInfo::winId = winId;
            displayDebugInfo::id = id;
            

            LTE::eventManger::startBuildingEvent()->
                setEventRoute("Key pressed/toggleDebugInfo")->
                setEventCallback(toggleDebugInfoOnKey)->
                add();
        }

        static void toggleDebugInfo()
        {
            if(showDebugInfo)
                LTE::eventManger::removeEvent("ImGui render/print debug info");
            else
                LTE::eventManger::startBuildingEvent()->
                    setEventRoute("ImGui render/print debug info")->
                    setEventCallback(ImGuiRender)->
                    setEntityID(id)->
                    setWindowId(winId)->add();
            showDebugInfo =! showDebugInfo;
        }

        static void toggleDebugInfoOnKey(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            LTE::KeyData *eventData = static_cast<LTE::KeyData *>(sendor);

            if(eventData->key == LT_KEY_F1)
                toggleDebugInfo();

        }

};

