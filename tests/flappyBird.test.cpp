#include "LaughTaleEngine.h"
#include "openGLTexture.h"


#include <gtest/gtest.h>
#include <vector>
#include <string.h>
#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <thread>
#include <chrono>
#include <algorithm>

#include <cstdlib>
#include <ctime>

#define PILAR_HOLE_WIDTH 0.15f
#define PILAR_HOLE_HIGHT 0.5f

#define PILAR_HOLE_HALF_WIDTH (PILAR_HOLE_WIDTH / 2.0f)
#define PILAR_HOLE_HALF_HIGHT (PILAR_HOLE_HIGHT / 2.0f)

#define PILAR_SPAWN_RATE 0.5f

class pilar: public LTE::component
{
    private:
        bool endOfScreen = false;
    public:


        pilar()
        {
        }

        virtual void init(LTE::gameObject *pilar) override
        {
            LTE::eventManger::addCoustemEventsRoute("App update/move pilar/");

            LTE::eventManger::startBuildingEvent()->
                setEventRoute("App update/move pilar/" + pilar->getName() + " " + std::to_string(parentId))->
                setEventCallback(onUpdate)->
                setEntityID(parentId)->
                setWindowId(winId)->add();
        }

        virtual void end() 
        {
        }

        static void onUpdate(LTE::gameObject *pilarEntity, LTE::coreEventData *sendor)
        {
            LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
            pilar *pilarControle = pilarEntity->getComponent<pilar>();
            LTE::transform *pilarTransform = pilarEntity->getTransform();

            pilarTransform->changeXPostion(- 0.5f * ((float)eventData->DeltaTime) / 1000);
            if(pilarTransform->getPostion().x < -1.7f)
                pilarControle->endOfScreen = true;
        }        
};

class bird: public LTE::component
{
    private:
        LTE::windowPieceId debugInfoWindowId;
        
        LTE::texture *logoTex;
        LTE::texture *starTex;

        float speed  = 0.0f;
        bool failed = false;
        bool star = false;

    public:
        bird(LTE::windowPieceId debugInfoWindowId): debugInfoWindowId(debugInfoWindowId){}

        virtual void init(LTE::gameObject *player) override
        {
            LTE::eventManger::startBuildingEvent()->
                setEventRoute("App update/move bird")->
                setEventCallback(bird::onUpdate)->
                setWindowId(winId)->
                setEntityID(parentId)->add();

            LTE::eventManger::startBuildingEvent()->
                setEventRoute("collider/" + player->getName() + "/handle walls")->
                setEventCallback(bird::onCollide)->
                setWindowId(winId)->
                setEntityID(parentId)->add();

            LTE::eventManger::startBuildingEvent()->
                setEventRoute("Key pressed/set bird speed")->
                setEventCallback(bird::onKey)->
                setEntityID(parentId)->add();

            LTE::eventManger::startBuildingEvent()->
                setEventRoute("ImGui render/print bird data")->
                setEventCallback(onImGui)->
                setEntityID(parentId)->
                setWindowId(debugInfoWindowId)->add();

                logoTex = new LTE::openGLTexture("res/textures/Logo.png");
                starTex = new LTE::openGLTexture("res/textures/5_star copy.png");
        }

        virtual void end() override
        {

        }

        static void movePlayer(LTE::gameObject *player, short DeltaTime)
        {
            bird *playerControl = player->getComponent<bird>();
            playerControl->speed -= ((float)DeltaTime)/1000.0f * 2;
            
            player->getTransform()->changeYPostion(((float)DeltaTime)/1000.0f * playerControl->speed * !playerControl->failed);
            player->getTransform()->setZRotation(glm::radians((playerControl->speed - 1.5f) * 25.0f));
        }
        
        static void onImGui(LTE::gameObject *player, LTE::coreEventData *sendor)
        {
            LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
            bird *playerControl = player->getComponent<bird>();

            ImGui::Text("player speed: %f", playerControl->speed);
            ImGui::Text("player hight: %f", player->getTransform()->getPostion().y);
            ImGui::Text("fps %.1f, %.1f", 1000.0f/(float)eventData->DeltaTime, double(ImGui::GetIO().Framerate));
            if(playerControl->failed)
                ImGui::Text("GAME OVER");
        }

        static void onUpdate(LTE::gameObject *player, LTE::coreEventData *sendor)
        {
            LTE::onUpdateData *eventData = static_cast<LTE::onUpdateData *>(sendor);
              
            movePlayer(player, eventData->DeltaTime);
            checkForScreenBonds(player);
        }

        static void checkForScreenBonds(LTE::gameObject *player)
        {
            if(player->getTransform()->getPostion().y < -1.0f)
                player->getComponent<bird>()->failed = true;
            if(player->getTransform()->getPostion().y > 1.0f)
                player->getTransform()->setYPostion(1.0f);  
        }

        static void onCollide(LTE::gameObject *player, LTE::coreEventData *sendor)
        {
            player->getComponent<bird>()->failed = true;
        }

        static void onKey(LTE::gameObject *player, LTE::coreEventData *sendor)
        {
            LTE::KeyData *eventData = dynamic_cast<LTE::KeyData *>(sendor);

            player->getComponent<bird>()->speed = 1.25f;
            player->getComponent<LTE::mesh>()->setShader(( player->getComponent<bird>()->star? "res/flappyBird/bird.shader": "res/flappyBird/Basic.shader"));
            player->getComponent<LTE::material>()->setTexture(( player->getComponent<bird>()->star ? player->getComponent<bird>()->logoTex : player->getComponent<bird>()->starTex));
            player->getComponent<bird>()->star = !player->getComponent<bird>()->star;
        }
};

class pilarSummener: public LTE::component
{
    private:
        float pilarPostions[20] = 
        {
            -0.5,   0.5f, 0.0f,  0.0f,  1.0f,
            -0.5,  -0.5f, 0.0f,  0.0f,  0.0f,
             0.5,  -0.5f, 0.0f,  1.0f,  0.0f, 
             0.5,   0.5f, 0.0f,  1.0f,  1.0f
        };


        unsigned int pilarIndices[6] = 
        {
            0, 1, 2,
            0, 3, 2,
        };
        

        LTE::mesh *initPilarTopMesh()
        {
            LTE::mesh *pilarTopMesh = new LTE::mesh(winId);

            LTE::app::getOsAPI()->makeContextCurrent(winId);

            pilarTopMesh->setShader("res/flappyBird/Basic.shader");

            pilarTopMesh->setVertexBuffer(pilarPostions, 20 * sizeof(float));
            pilarTopMesh->setIndexBuffer(pilarIndices, 6);

            pilarTopMesh->getVertexBuffer()->pushElement({LT_FLOAT, 3, false, 4});
            pilarTopMesh->getVertexBuffer()->pushElement({LT_FLOAT, 2, false, 4});

            pilarTopMesh->setVertexArray();

            return pilarTopMesh;
        }

        LTE::mesh *initPilarBottomMesh()
        {
            LTE::mesh *pilarBottomMesh = new LTE::mesh(winId);

            LTE::app::getOsAPI()->makeContextCurrent(winId);

            pilarBottomMesh->setShader("res/flappyBird/Basic.shader");

            pilarBottomMesh->setVertexBuffer(pilarPostions, 20 * sizeof(float));
            pilarBottomMesh->setIndexBuffer(pilarIndices, 6);

            pilarBottomMesh->getVertexBuffer()->pushElement({LT_FLOAT, 3, false, 4});
            pilarBottomMesh->getVertexBuffer()->pushElement({LT_FLOAT, 2, false, 4});

            pilarBottomMesh->setVertexArray();
            

            return pilarBottomMesh;
        }

    public:
        pilarSummener(){}

        virtual void init(LTE::gameObject *summener) override
        {
            LTE::eventManger::startBuildingEvent()->
                setEventRoute("collider/" + summener->getName() + "/summen pilars")->
                setEventCallback(onCollide)->
                setWindowId(winId)->
                setEntityID(parentId)->add();
        }

        virtual void end() override
        {

        }

        static void onCollide(LTE::gameObject *summener, LTE::coreEventData *sendor)
        {
            
            std::srand(std::time(nullptr));
            float pilarHight = (rand() / (RAND_MAX + 1.0f) - 0.5f) * 0.7;

            LTE::entityManger::addEntity([=](LTE::gameObject::gameObjectBuilder *builder){ 
                builder->
                    setObjectName("pilar top")->
                    setObjectTransform({{ 1.7f, pilarHight + 1.25f, 0.0f}, {0.0f, 0.0f, 0.0f}, { PILAR_HOLE_WIDTH, 2.0f, 1.0f}})->
                    setWindowId(sendor->windowId)->
                    addComponent(summener->getComponent<pilarSummener>()->initPilarTopMesh())->
                    addComponent(new LTE::material("res/textures/5_star.png", {1.0f, 1.0f, 0.0f, 1.0f}))->
                    addComponent(new LTE::meshRenderer())->
                    addComponent(new pilar())->
                    addComponent(new LTE::squreCollider());
                });

            LTE::entityManger::addEntity([=](LTE::gameObject::gameObjectBuilder *builder){ 
                builder->
                    setObjectName("pilar bottom")->
                    setObjectTransform({{ 1.7f, pilarHight - 1.25f, 0.0f}, {0.0f, 0.0f, 0.0f}, { PILAR_HOLE_WIDTH, 2.0f, 1.0f}})->
                    setWindowId(sendor->windowId)->
                    addComponent(summener->getComponent<pilarSummener>()->initPilarBottomMesh())->
                    addComponent(new LTE::material("res/textures/5_star.png", {1.0f, 1.0f, 0.0f, 1.0f}))->
                    addComponent(new LTE::meshRenderer())->
                    addComponent(new pilar())->
                    addComponent(new LTE::squreCollider());
                });
        }
};

class pilarDestroyer: public LTE::component
{
    private:
        
    public:
        pilarDestroyer(){}

        virtual void init(LTE::gameObject *destroyer) override
        {
            LTE::eventManger::startBuildingEvent()->
                setEventRoute("collider/" + destroyer->getName() + "/destroy pilars")->
                setEventCallback(onCollide)->
                setWindowId(winId)->
                setEntityID(parentId)->add();
        }

        virtual void end() override
        {
        }

        static void onCollide(LTE::gameObject *summener, LTE::coreEventData *sendor)
        {
            LTE::colliderEventData *sendorData = static_cast<LTE::colliderEventData*>(sendor);
            LTE::entityManger::removeEntityById(sendorData->target->getId());
        }
};



class flappyBird : public ::testing::Test
{
    private:
        LTE::windowPieceId gameWindowId;
        LTE::windowPieceId debugInfoWindowId;



        void initWindows()
        {
            gameWindowId =  LTE::windowManger::addWindow([=](LTE::windowBuilder *build)
            {
                build->setTitle("flappyBird");
            });
            
            debugInfoWindowId = LTE::windowManger::addWindow([=](LTE::windowBuilder *build)
            {
                build->setHeight(600)->setWidth(600)->setTitle("debug Info Window")->useImGui();
            });
        }

        static void WindowClose(__attribute__((unused)) LTE::gameObject *eventEntity, __attribute__((unused)) LTE::coreEventData *sendor)
        {
            LTE::app::keepRunning = false;
        }

    private:
        float birdPostions[6 * 5] = 
        {
             0.0f,    0.5f ,  0.0f,   0.5f,   1.0f ,
             0.5f,    0.25f,  0.0f,   1.0f,   0.75f,
             0.5f,   -0.25f,  0.0f,   1.0f,   0.25f,
             0.0f,   -0.5f ,  0.0f,   0.5f,   0.0f ,
            -0.5f,   -0.25f,  0.0f,   0.0f,   0.25f,
            -0.5f,   0.25f,   0.0f,   0.0f,   0.75f 
        };


        unsigned int birdIndices[12] = 
        {
            0, 1, 5,
            1, 2, 4,
            2, 3, 4,
            1, 4, 5
        };

        float pilarPostions[20] = 
        {
            -0.5,   0.5f, 0.0f,  0.0f,  1.0f,
            -0.5,  -0.5f, 0.0f,  0.0f,  0.0f,
             0.5,  -0.5f, 0.0f,  1.0f,  0.0f, 
             0.5,   0.5f, 0.0f,  1.0f,  1.0f
        };


        unsigned int pilarIndices[6] = 
        {
            0, 1, 2,
            0, 3, 2,
        };
        

        LTE::mesh *initPlayerMesh()
        {
            LTE::mesh *playerMesh = new LTE::mesh(gameWindowId);
            LTE::app::getOsAPI()->makeContextCurrent(gameWindowId);

            playerMesh->setShader("res/flappyBird/bird.shader");

            playerMesh->setVertexBuffer(birdPostions, 6 * 5 * sizeof(float));
            playerMesh->setIndexBuffer(birdIndices, 12);

            playerMesh->getVertexBuffer()->pushElement({LT_FLOAT, 3, false, 4});
            playerMesh->getVertexBuffer()->pushElement({LT_FLOAT, 2, false, 4});

            playerMesh->setVertexArray();

            return playerMesh;
        }

        LTE::mesh *initPilarTopMesh()
        {
            LTE::mesh *pilarTopMesh = new LTE::mesh(gameWindowId);
            LTE::app::getOsAPI()->makeContextCurrent(gameWindowId);

            pilarTopMesh->setShader("res/flappyBird/Basic.shader");

            pilarTopMesh->setVertexBuffer(pilarPostions, 20 * sizeof(float));
            pilarTopMesh->setIndexBuffer(pilarIndices, 6);

            pilarTopMesh->getVertexBuffer()->pushElement({LT_FLOAT, 3, false, 4});
            pilarTopMesh->getVertexBuffer()->pushElement({LT_FLOAT, 2, false, 4});

            pilarTopMesh->setVertexArray();

            return pilarTopMesh;
        }

        LTE::mesh *initPilarBottomMesh()
        {
            LTE::mesh *pilarBottomMesh = new LTE::mesh(gameWindowId);

            LTE::app::getOsAPI()->makeContextCurrent(gameWindowId);
            
            pilarBottomMesh->setShader("res/flappyBird/Basic.shader");

            pilarBottomMesh->setVertexBuffer(pilarPostions, 20 * sizeof(float));
            pilarBottomMesh->setIndexBuffer(pilarIndices, 6);

            pilarBottomMesh->getVertexBuffer()->pushElement({LT_FLOAT, 3, false, 4});
            pilarBottomMesh->getVertexBuffer()->pushElement({LT_FLOAT, 2, false, 4});

            pilarBottomMesh->setVertexArray();
            

            return pilarBottomMesh;
        }

    public:        
        void SetUp()
        { 
            LTE::app::init();
            initWindows();

            LTE::entityManger::addEntity([=, this](LTE::gameObject::gameObjectBuilder *builder){ 
                builder->
                    setObjectName("player")->
                    setObjectTransform({{ -0.9, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, { 0.2, 0.2f, 0.0f}})->
                    setWindowId(gameWindowId)->
                    addComponent(initPlayerMesh())->
                    addComponent(new LTE::material("res/textures/Logo.png", glm::vec4({0.0f, 0.0f, 0.0f, 1.0f})))->
                    addComponent(new LTE::meshRenderer())->
                    addComponent(new LTE::squreCollider())->
                    addComponent(new bird(debugInfoWindowId));
                });


            std::srand(std::time(nullptr));
            float pilarHight = (rand() / (RAND_MAX + 1.0f) - 0.5f) * 0.7;

            LTE::entityManger::addEntity([=, this](LTE::gameObject::gameObjectBuilder *builder){ 
                builder->
                    setObjectName("pilar top")->
                    setObjectTransform({{ 1.7f, pilarHight + 1.25f, 0.0f}, {0.0f, 0.0f, 0.0f}, { PILAR_HOLE_WIDTH, 2.0f, 1.0f}})->
                    setWindowId(gameWindowId)->
                    addComponent(initPilarTopMesh())->
                    addComponent(new LTE::material("res/textures/5_star.png", {1.0f, 1.0f, 0.0f, 1.0f}))->
                    addComponent(new LTE::meshRenderer())->
                    addComponent(new pilar())->
                    addComponent(new LTE::squreCollider());
                });

            LTE::entityManger::addEntity([=, this](LTE::gameObject::gameObjectBuilder *builder){ 
                builder->
                    setObjectName("pilar bottom")->
                    setObjectTransform({{ 1.7f, pilarHight - 1.25f, 0.0f}, {0.0f, 0.0f, 0.0f}, { PILAR_HOLE_WIDTH, 2.0f, 1.0f}})->
                    setWindowId(gameWindowId)->
                    addComponent(initPilarBottomMesh())->
                    addComponent(new LTE::material("res/textures/5_star.png", {1.0f, 1.0f, 0.0f, 1.0f}))->
                    addComponent(new LTE::meshRenderer())->
                    addComponent(new pilar())->
                    addComponent(new LTE::squreCollider());
                });

            LTE::entityManger::addEntity([=, this](LTE::gameObject::gameObjectBuilder *builder){ 
                builder->
                    setObjectName("pilar summener")->
                    setObjectTransform({{ 0.8f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, { 0.1f, 0.1f, 1.0f}})->
                    setWindowId(gameWindowId)->
                    addComponent(new LTE::squreCollider())->
                    addComponent(new pilarSummener());
                });

            LTE::entityManger::addEntity([=, this](LTE::gameObject::gameObjectBuilder *builder){ 
                builder->
                    setObjectName("pilar destroyer")->
                    setObjectTransform({{ -1.7f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, { 0.1f, 2.0f, 1.0f}})->
                    setWindowId(gameWindowId)->
                    addComponent(new LTE::squreCollider())->
                    addComponent(new pilarDestroyer());
                });


            LTE::eventManger::startBuildingEvent()->
                setEventRoute("Window close/close app")->
                setEventCallback(WindowClose)->add();
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
