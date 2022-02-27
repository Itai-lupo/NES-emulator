#include "LaughTaleEngine.h"
#include "6502.h"
#include "cpu.h"
#include "ram.h"
#include "bus.h"
#include "cpuBusCartridge.h"
#include "ppuBusCartridge.h"
#include "imgui.h"

std::string toHexString(uint32_t n, uint8_t d)
{
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4)
        s[i] = "0123456789ABCDEF"[n & 0xF];
    return s;
}

void keyDispatcher(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
{
    LTE::KeyData *eventData = static_cast<LTE::KeyData *>(sendor);
    
    if(eventData->key == LT_KEY_SPACE)
        sendor->route = "cpu cmd/cpu clock/";

    if(eventData->key == LT_KEY_R)        
        sendor->route = "cpu cmd/cpu reset/";
            
    if(eventData->key == LT_KEY_I)
        sendor->route = "cpu cmd/cpu irq/";
        
    if(eventData->key == LT_KEY_N)
        sendor->route = "cpu cmd/cpu nmi/";

    if(sendor->route != "Key pressed/cpu keyDispatcher")
        LTE::eventManger::trigerEvent(sendor);
        // for(unsigned long i = 0; i < 49110; i++){
        // }
}


int page;
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
    for(int i = 0; i < 10; i++)
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
        toHexString(pc, 4) + ":\t" + 
        c->lookup[sysBus->read(pc)].name + temp
        ).c_str()
    );

    codeStart = pc + c->lookup[sysBus->read(pc)].size + 1;

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

int main() 
{
    LTE::app::init();

    ram *r = new ram();
    cpu6502 *c = new cpu6502();
    bus<uint8_t, uint16_t> *sysBus = (new bus<uint8_t, uint16_t>())->pushDevice(r);

    cartridge *cart = new cartridge();
    cart->load("testRes/roms/nestest.nes");
    sysBus->pushDevice(new cpuBusCartridge(cart));

    LTE::entityTaleId id =  LTE::entityManger::addEntity(
        [&](LTE::gameObject::gameObjectBuilder *b)
        {
            b->setObjectName("system data")->
                addComponent(sysBus)->
                addComponent(c);
        });

    cpu<uint8_t, uint16_t>::init(id);

    LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu reset/"));
    for (size_t i = 0; i < 8; i++)
        LTE::eventManger::trigerEvent(new LTE::coreEventData("cpu cmd/cpu clock/"));


    LTE::windowPieceId winId =  LTE::windowManger::addWindow([](LTE::windowBuilder *b){ b->setTitle("nes emulator")->useImGui();});

    LTE::eventManger::startBuildingEvent()->
        setEntityID(id)->
        setEventRoute("Key pressed/cpu keyDispatcher")->
        setEventCallback(keyDispatcher)->
        add();

    LTE::eventManger::startBuildingEvent()->
        setEventRoute("ImGui render/print debug info")->
        setEventCallback(ImGuiRender)->
        setEntityID(id)->
        setWindowId(winId)->add();


    LTE::app::run();
    cpu<uint8_t, uint16_t>::close();
    LTE::app::close();
    return 0;
}
