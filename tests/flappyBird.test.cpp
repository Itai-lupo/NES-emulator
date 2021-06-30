#include "LaughTaleEngine.h"
#include <gtest/gtest.h>
#include <vector>
#include <string.h>
#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <thread>
#include <chrono>
#include <algorithm>
#include "drum.h"
#include "bell.h"

#include <cstdlib>
#include <ctime>

#define PILAR_HOLE_WIDTH 0.15f
#define PILAR_HOLE_HIGHT 0.5f

#define PILAR_HOLE_HALF_WIDTH (PILAR_HOLE_WIDTH / 2.0f)
#define PILAR_HOLE_HALF_HIGHT (PILAR_HOLE_HIGHT / 2.0f)

#define PILAR_SPAWN_RATE 0.5f

class pilar: public LTE::IEntity
{
    private:
        float pilarPostions[20] = 
        {
            -PILAR_HOLE_HALF_WIDTH,  1.0f, 0.0f,  0.0f,  1.0f,
            -PILAR_HOLE_HALF_WIDTH, -1.0f, 0.0f,  0.0f,  0.0f,
             PILAR_HOLE_HALF_WIDTH, -1.0f, 0.0f,  1.0f,  0.0f, 
             PILAR_HOLE_HALF_WIDTH,  1.0f, 0.0f,  1.0f,  1.0f
        };


        unsigned int birdIndices[6] = 
        {
            0, 1, 2,
            0, 3, 2,
        };
        

    public:
        LTE::mesh *pilarBottomMesh;
        LTE::mesh *pilarTopMesh;
        LTE::eventLaughId onRenderId1;
        LTE::eventLaughId onRenderId2;
        LTE::eventLaughId onUpdateId;

        float pilarHight = 0.0f;
        float pilarX = 1.7f;
        bool endOfScreen = false;

        pilar(LTE::windowPieceId  gameWindowId)
        {
            std::srand(std::time(nullptr));
            
            pilarHight = (rand() / (RAND_MAX + 1.0f) - 0.5f) * 0.7;

            pilarTopMesh = new LTE::mesh(gameWindowId);
            pilarBottomMesh = new LTE::mesh(gameWindowId);


            pilarTopMesh->setShader("res/flappyBird/Basic.shader");
            pilarBottomMesh->setShader("res/flappyBird/Basic.shader");

            pilarTopMesh->setVertexBuffer(pilarPostions, 20 * sizeof(float));
            pilarBottomMesh->setVertexBuffer(pilarPostions, 20 * sizeof(float));

            pilarTopMesh->setIndexBuffer(birdIndices, 6);
            pilarBottomMesh->setIndexBuffer(birdIndices, 6);
            
            pilarTopMesh->getVertexBuffer()->pushElement({LT_FLOAT, 3, false, 4});
            pilarTopMesh->getVertexBuffer()->pushElement({LT_FLOAT, 2, false, 4});
            
            pilarBottomMesh->getVertexBuffer()->pushElement({LT_FLOAT, 3, false, 4});
            pilarBottomMesh->getVertexBuffer()->pushElement({LT_FLOAT, 2, false, 4});

            pilarTopMesh->setVertexArray();
            pilarBottomMesh->setVertexArray();

            pilarTopMesh->setMaterial("res/textures/5_star.png", {1.0f, 1.0f, 0.0f, 1.0f});
            pilarBottomMesh->setMaterial("res/textures/5_star.png", {0.0f, 1.0f, 1.0f, 1.0f});

            LTE::renderLoop::addMesh(pilarTopMesh);
            LTE::renderLoop::active(pilarTopMesh->getId());
            
            LTE::renderLoop::addMesh(pilarBottomMesh);
            LTE::renderLoop::active(pilarBottomMesh->getId());
            
            
            LTE::entityTaleId pilarEntityId = LTE::entityManger::addEntity(dynamic_cast<IEntity*>(this));
            LTE::event *onUpdateEvent = LTE::event::eventBuilder::startBuilding()->
                            setEventType(LTE::events::AppRender)->
                            setEventCallback(onUpdate)->setEntityID(pilarEntityId)->setWindowId(gameWindowId)->build();
           
            onUpdateId = LTE::eventManger::addEvent(onUpdateEvent);
        }
        

        static void onUpdate(LTE::IEntity *eventEntity, LTE::coreEventData *sendor)
        {
            pilar *p = static_cast<pilar *>(eventEntity);
            LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);

            p->pilarX -= 0.5f * ((float)eventData->DeltaTime) / 1000;
            if(p->pilarX < -1.7f)
                p->endOfScreen = true;

            p->pilarTopMesh->setTransform(glm::translate(glm::mat4(1.0f), { p->pilarX,  1 + p->pilarHight + PILAR_HOLE_HALF_HIGHT, 0.0f}));
            p->pilarBottomMesh->setTransform(glm::translate(glm::mat4(1.0f), { p->pilarX, -1 + p->pilarHight - PILAR_HOLE_HALF_HIGHT, 0.0f}));
            LAUGHTALE_ENGINR_CONDTION_LOG_ERROR((uint64_t)p->pilarTopMesh, (uint64_t)p->pilarTopMesh == 0xd);

        }

        
};

class bird: public LTE::IEntity
{
    private:
        float birdPostions[6 * 5] = 
        {
             0.00f,     0.10f,  0.0f,   0.5f,   1.0f,
             0.085f,    0.05f,  0.0f,   1.0f,   0.75f,
             0.085f,   -0.05f,  0.0f,   1.0f,   0.25f,
             0.00f,    -0.10f,  0.0f,   0.5f,   0.0f,
            -0.085f,   -0.05f,  0.0f,   0.0f,   0.25f,
            -0.085f,    0.05f,  0.0f,   0.0f,   0.75f
        };


        unsigned int birdIndices[12] = 
        {
            0, 1, 5,
            1, 2, 4,
            2, 3, 4,
            1, 4, 5
        };

        LTE::mesh *birdMesh;
        static inline LTE::materialId starMat;
        static inline LTE::materialId logoMat;
    public:
        LTE::eventLaughId updateId;
        LTE::eventLaughId onKeyId;

        LTE::entityTaleId playerEntityId;
        float birdHight;
        float speed = 0;
        bool failed = false;
        bool star = false;

        bird(LTE::windowPieceId  gameWindowId)
        {
            birdMesh = new LTE::mesh(gameWindowId);

            starMat = LTE::materialsManger::addMatrial(new LTE::material("res/textures/5_star.png"));
            logoMat = LTE::materialsManger::addMatrial(new LTE::material("res/textures/Logo.png"));

            birdMesh->setShader("res/flappyBird/bird.shader");

            birdMesh->setVertexBuffer(birdPostions, 6 * 5 * sizeof(float));
            birdMesh->setIndexBuffer(birdIndices, 12);

            birdMesh->getVertexBuffer()->pushElement({LT_FLOAT, 3, false, 4});
            birdMesh->getVertexBuffer()->pushElement({LT_FLOAT, 2, false, 4});

            birdMesh->setVertexArray();

            birdMesh->setMaterial(logoMat);
            LTE::renderLoop::addMesh(birdMesh);
            LTE::renderLoop::active(birdMesh->getId());

            
            playerEntityId = LTE::entityManger::addEntity(this);
            
            LTE::event *onUpdateEvent = LTE::event::eventBuilder::startBuilding()->
                            setEventType(LTE::events::AppUpdate)->
                            setEventCallback(bird::onUpdate)->setEntityID(playerEntityId)->build();
            
            LTE::event *onKeyPressedEvent = LTE::event::eventBuilder::startBuilding()->
                            setEventType(LTE::events::KeyPressed)->
                            setEventCallback(bird::onKey)->setEntityID(playerEntityId)->setWindowId(gameWindowId)->build();

            updateId = LTE::eventManger::addEvent(onUpdateEvent);
            onKeyId = LTE::eventManger::addEvent(onKeyPressedEvent);
            LAUGHTALE_ENGINR_LOG_INFO(updateId)
        }

        static void movePlayer(bird *player, short DeltaTime)
        {
            player->speed -= ((float)DeltaTime)/1000.0f * 2;
            player->birdHight = 
                (player->birdHight + ((float)DeltaTime)/1000.0f * player->speed) * !player->failed;
            
            if(player->birdHight < -1.0f)
                player->failed = true;
            if(player->birdHight > 1.0f)
                player->birdHight = 1.0f;  
            
            player->birdMesh->setTransform(glm::translate(glm::mat4(1.0f), { -0.9, player->birdHight, 1.0f}) * 
                                glm::rotate(glm::mat4(1.0f), glm::radians((player->speed - 1.5f) * 25.0f ), glm::vec3(0, 0, 1)));
            
        }

        static void onUpdate(LTE::IEntity *eventEntity, LTE::coreEventData *sendor)
        {
            bird *player = static_cast<bird *>(eventEntity);
            LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
              
            movePlayer(player, eventData->DeltaTime);
        }

        static void onKey(LTE::IEntity *eventEntity, __attribute__((unused)) LTE::coreEventData *sendor)
        {
            LTE::KeyData *eventData = dynamic_cast<LTE::KeyData *>(sendor);
            bird *player = static_cast<bird *>(eventEntity);
            player->speed = 1.25f;
            player->birdMesh->setShader(( player->star? "res/flappyBird/bird.shader": "res/flappyBird/Basic.shader"));
            player->birdMesh->setMaterial(( player->star? logoMat: starMat));
            player->star = !player->star;
        }
};

class flappyBird : public ::testing::Test, public LTE::IEntity
{
    private:        
        bird *player;
        std::vector<pilar *> pilars;
        LTE::windowPieceId gameWindowId;
        LTE::windowPieceId debugInfoWindowId;
        
        void initWindows()
        {
            gameWindowId =  LTE::windowManger::addWindow("flappyBird");
            debugInfoWindowId = LTE::windowManger::addWindow("debug Info Window", true, 600, 600);

            LTE::windowManger::setCamera(gameWindowId, new LTE::orthographicCameraControler(1.6f / 0.9f, gameWindowId));
            LTE::windowManger::bindContext(gameWindowId);

        }

        static void WindowClose(__attribute__((unused)) LTE::IEntity *eventEntity, __attribute__((unused)) LTE::coreEventData *sendor)
        {
            LTE::app::keepRunning = false;
        }

    public:
        static void onImGui(LTE::IEntity *eventEntity, __attribute__((unused)) LTE::coreEventData *sendor)
        {
            LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
            bird *player = static_cast<bird *>(eventEntity);
            ImGui::Text("player speed: %d", player->speed);
            ImGui::Text("player hight: %f", player->birdHight);
            ImGui::Text("fps %.1f, %.1f", 1000.0f/(float)eventData->DeltaTime, double(ImGui::GetIO().Framerate));
            if(player->failed)
                ImGui::Text("GAME OVER");
                
        }

        static void onUpdate(LTE::IEntity *eventEntity, __attribute__((unused)) LTE::coreEventData *sendor)
        {
            flappyBird *game = static_cast<flappyBird *>(eventEntity);
            LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
            
            if(game->pilars.size() > 0 && (game->pilars[0]->endOfScreen || game->pilars[0]->pilarX < -1.7f))
            {
                pilar *temp = game->pilars[0];
                LTE::entityManger::removeEntityById(temp->getId());
                LTE::eventManger::removeEvent(LTE::events::AppRender, temp->onUpdateId);
                LTE::renderLoop::remove(temp->pilarTopMesh->getId());
                LTE::renderLoop::remove(temp->pilarBottomMesh->getId());

                game->pilars.erase(game->pilars.begin());
                delete temp;
            }

            game->player->failed = 
                game->player->failed ||
                    ((game->pilars[0]->pilarX + PILAR_HOLE_HALF_WIDTH > -0.95 &&  -0.85 > game->pilars[0]->pilarX - PILAR_HOLE_HALF_WIDTH) &&
                    !(game->player->birdHight + 0.1f < game->pilars[0]->pilarHight + PILAR_HOLE_HALF_HIGHT && game->player->birdHight - 0.1f > game->pilars[0]->pilarHight - PILAR_HOLE_HALF_HIGHT));

            if(game->pilars[game->pilars.size() - 1]->pilarX < PILAR_SPAWN_RATE)
                game->pilars.push_back(new pilar(game->gameWindowId));
        }
        
        void SetUp()
        { 
            LTE::app::init();
            initWindows();

            LTE::entityTaleId gameEntityId = LTE::entityManger::addEntity(this);
            player = new bird(gameWindowId);
            pilars.push_back(new pilar(gameWindowId));

            LTE::event *onAppUpdateEvent = LTE::event::eventBuilder::startBuilding()->
                setEventType(LTE::events::AppUpdate)->
                setEventCallback(onUpdate)->setEntityID(gameEntityId)->build();

            LTE::event *onWindowCloseEvent = LTE::event::eventBuilder::startBuilding()->
                setEventType(LTE::events::WindowClose)->
                setEventCallback(WindowClose)->build();

            LTE::event *onImGuiRenderEvent = LTE::event::eventBuilder::startBuilding()->
                setEventType(LTE::events::ImGuiRender)->
                setEventCallback(onImGui)->setEntityID(player->playerEntityId)->setWindowId(debugInfoWindowId)->build();

            LTE::eventManger::addEvent(onAppUpdateEvent);
            LTE::eventManger::addEvent(onWindowCloseEvent);
            LTE::eventManger::addEvent(onImGuiRenderEvent);
        }

        void TearDown() 
        { 
            LTE::app::close();
        }
};




TEST_F(flappyBird, testGames)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    LTE::app::run();

}
