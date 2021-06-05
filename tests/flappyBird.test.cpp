#include "LaughTaleEngine.h"
#include <gtest/gtest.h>
#include <vector>
#include <string.h>
#include "imgui.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <algorithm>
#include "drum.h"
#include "bell.h"


#include <cstdlib>
#include <ctime>


class pilar: public LaughTaleEngine::IEntity
{
    private:
        float pilarPostions[8] = 
        {
             0.1f, -1.0f,
            -0.1f, -1.0f,
            -0.1f,  1.0f,
             0.1f,  1.0f
        };


        unsigned int birdIndices[6] = 
        {
            0, 1, 2,
            0, 3, 2,
        };
        
        LaughTaleEngine::windowPieceId  gameWindowId;
        LaughTaleEngine::vertexBufferId VBId;
        LaughTaleEngine::indexBufferId IBId;
        LaughTaleEngine::vertexArrayId VAId;
        LaughTaleEngine::shaderId      ShaderId;
    public:
        LaughTaleEngine::eventLaughId onRenderId;
        float pilarHight = 0.0f;
        float pilarX = 1.7f;
        bool endOfScreen = false;

        pilar(LaughTaleEngine::windowPieceId  gameWindowId): gameWindowId(gameWindowId)
        {
            std::srand(std::time(nullptr));
            pilarHight = (rand() / (RAND_MAX + 1.) - 1) * 0.8;
            LaughTaleEngine::shader *shader = new LaughTaleEngine::openGLShader("res/shaders/Basic.shader");
            ShaderId = LaughTaleEngine::windowManger::add(gameWindowId, shader);

            LaughTaleEngine::VertexBuffer *vertexBuffer = new LaughTaleEngine::OpenGLVertexBuffer(pilarPostions, 2 * 4 * sizeof(float));
            LaughTaleEngine::indexBuffer *indexBuffer = new LaughTaleEngine::openGLIndexBuffer(birdIndices, 6);

            VBId = LaughTaleEngine::windowManger::add(gameWindowId, vertexBuffer);
            IBId = LaughTaleEngine::windowManger::add(gameWindowId, indexBuffer);

            LaughTaleEngine::windowManger::pushElement(gameWindowId, VBId, {LT_FLOAT, 2, false, 4});

            LaughTaleEngine::vertexArray *vertexArray = new LaughTaleEngine::openGLVertexArray();
            VAId = LaughTaleEngine::windowManger::add(gameWindowId, vertexArray);
            
            LaughTaleEngine::windowManger::addBuffer(gameWindowId, VAId, VBId);

            LaughTaleEngine::entityTaleId pilarEntityId = LaughTaleEngine::entityManger::addEntity(this);

            onRenderId = LaughTaleEngine::eventManger::addEvent(LaughTaleEngine::events::AppRender, onRender, pilarEntityId, gameWindowId);
        }

        static void renderPilar(pilar *pilar)
        {
            LaughTaleEngine::renderApi *api = LaughTaleEngine::windowManger::getRenderApi(pilar->gameWindowId);
            LaughTaleEngine::renderer *r = LaughTaleEngine::windowManger::getRenderer(pilar->gameWindowId);
             

            LaughTaleEngine::windowManger::bindVA(pilar->gameWindowId, pilar->VAId);
            LaughTaleEngine::windowManger::bindS(pilar->gameWindowId, pilar->ShaderId);
            LaughTaleEngine::windowManger::bindIB(pilar->gameWindowId, pilar->IBId);
            
            LaughTaleEngine::shaderManger *windowShaderManger = LaughTaleEngine::windowManger::getShaderManger(pilar->gameWindowId);
            LaughTaleEngine::shader *s = windowShaderManger->getShader(pilar->ShaderId);
            s->setUniform4f("colorOffset", 1.0f, 1.0f, 0.0f, 1.0f);
            
            r->Submit(s, 6, glm::translate(glm::mat4(1.0f), { pilar->pilarX, 1 + pilar->pilarHight + 0.4, 0.0f}));
            s->setUniform4f("colorOffset", 0.0f, 1.0f, 1.0f, 1.0f);

            r->Submit(s, 6, glm::translate(glm::mat4(1.0f), { pilar->pilarX, -1 + pilar->pilarHight - 0.4, 0.0f}));

        }

        static void onRender(LaughTaleEngine::IEntity *eventEntity, LaughTaleEngine::IEventData *sendor)
        {
            pilar *p = static_cast<pilar *>(eventEntity);
            LaughTaleEngine::onUpdateData *eventData = static_cast<LaughTaleEngine::onUpdateData *>(sendor);
            p->pilarX -= 0.5f * ((float)eventData->DeltaTime) / 1000;
            if(p->pilarX < -1.7f)
                p->endOfScreen = true;
            renderPilar(p);
        }

        
};

class bird: public LaughTaleEngine::IEntity
{
    private:
        float birdPostions[12] = 
        {
             0.0f ,    0.2f,
             0.17f,    0.1f,
             0.17f,   -0.1f,
             0.0f ,   -0.2f,
            -0.17f,   -0.1f,
            -0.17f,    0.1f
        };


        unsigned int birdIndices[12] = 
        {
            0, 1, 5,
            1, 2, 4,
            2, 3, 4,
            1, 4, 5
        };
        
        LaughTaleEngine::windowPieceId  gameWindowId;
        LaughTaleEngine::vertexBufferId birdVBId;
        LaughTaleEngine::indexBufferId birdIBId;
        LaughTaleEngine::vertexArrayId birdVAId;
        LaughTaleEngine::shaderId      birdShaderId;
    public:
        LaughTaleEngine::entityTaleId playerEntityId;
        float birdHight;
        float speed = 0;
        bool failed = false;

        bird(LaughTaleEngine::windowPieceId  gameWindowId): gameWindowId(gameWindowId)
        {
            LaughTaleEngine::shader *shader = new LaughTaleEngine::openGLShader("res/shaders/Basic.shader");
            birdShaderId = LaughTaleEngine::windowManger::add(gameWindowId, shader);

            LaughTaleEngine::VertexBuffer *birdVB = new LaughTaleEngine::OpenGLVertexBuffer(birdPostions, 2 * 6 * sizeof(float));
            LaughTaleEngine::indexBuffer *birdIB = new LaughTaleEngine::openGLIndexBuffer(birdIndices, 12);

            birdVBId = LaughTaleEngine::windowManger::add(gameWindowId, birdVB);
            birdIBId = LaughTaleEngine::windowManger::add(gameWindowId, birdIB);

            LaughTaleEngine::windowManger::pushElement(gameWindowId, birdVBId, {LT_FLOAT, 2, false, 4});

            LaughTaleEngine::vertexArray *birdVA = new LaughTaleEngine::openGLVertexArray();
            birdVAId = LaughTaleEngine::windowManger::add(gameWindowId, birdVA);
            
            LaughTaleEngine::windowManger::addBuffer(gameWindowId, birdVAId, birdVBId);

            playerEntityId = LaughTaleEngine::entityManger::addEntity(this);

            LaughTaleEngine::eventManger::addEvent(LaughTaleEngine::events::AppRender, bird::onRender, playerEntityId, gameWindowId);
            LaughTaleEngine::eventManger::addEvent(LaughTaleEngine::events::KeyPressed, bird::onKey, playerEntityId, gameWindowId);
        }

        static void movePlayer(bird *player, short DeltaTime)
        {
            player->speed -= ((float)DeltaTime)/1000.0f * 3;
            player->birdHight = 
                (player->birdHight + ((float)DeltaTime)/1000.0f * player->speed) * !player->failed;
            
            if(player->birdHight < -1.0f)
                player->failed = true;
            if(player->birdHight > 1.0f)
                player->birdHight = 1.0f;  
        }

        static void renderBird(bird *player)
        {
            LaughTaleEngine::renderApi *api = LaughTaleEngine::windowManger::getRenderApi(player->gameWindowId);
            LaughTaleEngine::renderer *r = LaughTaleEngine::windowManger::getRenderer(player->gameWindowId);

            LaughTaleEngine::windowManger::bindVA(player->gameWindowId, player->birdVAId);
            LaughTaleEngine::windowManger::bindS(player->gameWindowId, player->birdShaderId);
            LaughTaleEngine::windowManger::bindIB(player->gameWindowId, player->birdIBId);
            
            LaughTaleEngine::shaderManger *windowShaderManger = LaughTaleEngine::windowManger::getShaderManger(player->gameWindowId);
            LaughTaleEngine::shader *s = windowShaderManger->getShader(player->birdShaderId);
            s->setUniform4f("colorOffset", 0.0f, 1.0f, 0.0f, 1.0f);
            
            LaughTaleEngine::coreCamera *camera = new LaughTaleEngine::orthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
            LaughTaleEngine::windowManger::setCamera(player->gameWindowId, camera);

            r->Submit(s, 12, glm::translate(glm::mat4(1.0f), { -0.9, player->birdHight, 1.0f}));
        }

        static void onRender(LaughTaleEngine::IEntity *eventEntity, LaughTaleEngine::IEventData *sendor)
        {
            bird *player = static_cast<bird *>(eventEntity);
            LaughTaleEngine::onUpdateData *eventData = static_cast<LaughTaleEngine::onUpdateData *>(sendor);
              
            movePlayer(player, eventData->DeltaTime);
            renderBird(player);
        }

        static void onKey(LaughTaleEngine::IEntity *eventEntity, __attribute__((unused)) LaughTaleEngine::IEventData *sendor)
        {
            bird *player = static_cast<bird *>(eventEntity);
            player->speed = 1.5f;
        }
};

class flappyBird : public ::testing::Test, public LaughTaleEngine::IEntity
{
    private:
        bird *player;
        std::vector<pilar *> pilars;
        LaughTaleEngine::windowPieceId gameWindowId;
        LaughTaleEngine::windowPieceId debugInfoWindowId;
        
        void initWindows()
        {
            gameWindowId =  LaughTaleEngine::windowManger::addWindow("flappyBird");
            debugInfoWindowId = LaughTaleEngine::windowManger::addWindow("debug Info Window", true, 600, 600);

            LaughTaleEngine::windowManger::setCamera(gameWindowId, new LaughTaleEngine::orthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f));
            LaughTaleEngine::windowManger::bindContext(gameWindowId);

        }

        static void WindowClose(__attribute__((unused)) LaughTaleEngine::IEntity *eventEntity, __attribute__((unused)) LaughTaleEngine::IEventData *sendor)
        {
            LaughTaleEngine::app::keepRunning = false;
        }

    public:
        static void onImGui(LaughTaleEngine::IEntity *eventEntity, __attribute__((unused)) LaughTaleEngine::IEventData *sendor)
        {
            LaughTaleEngine::onUpdateData *eventData = static_cast<LaughTaleEngine::onUpdateData *>(sendor);
            bird *player = static_cast<bird *>(eventEntity);
            ImGui::Text("player speed: %d", player->speed);
            ImGui::Text("player hight: %f", player->birdHight);
            ImGui::Text("fps %.1f, %.1f", 1000.0f/(float)eventData->DeltaTime, double(ImGui::GetIO().Framerate));
            if(player->failed)
                ImGui::Text("GAME OVER");
                
        }

        static void onUpdate(LaughTaleEngine::IEntity *eventEntity, __attribute__((unused)) LaughTaleEngine::IEventData *sendor)
        {
            flappyBird *game = static_cast<flappyBird *>(eventEntity);
            LaughTaleEngine::onUpdateData *eventData = static_cast<LaughTaleEngine::onUpdateData *>(sendor);
            
            if(game->pilars.size() > 0 && game->pilars[0]->endOfScreen)
            {
                pilar *temp = game->pilars[0];
                LaughTaleEngine::eventManger::removeEvent(LaughTaleEngine::events::AppRender, temp->onRenderId);

                game->pilars.erase(game->pilars.begin());
                free(temp);
            }

            game->player->failed = 
                game->player->failed ||
                    ((game->pilars[0]->pilarX > -0.95 &&  -0.85 > game->pilars[0]->pilarX) &&
                    !(game->player->birdHight < game->pilars[0]->pilarHight + 0.4f && game->player->birdHight > game->pilars[0]->pilarHight - 0.4f));

            if(game->pilars[game->pilars.size() - 1]->pilarX < 0.0f)
                game->pilars.push_back(new pilar(game->gameWindowId));
        }
        
        void SetUp()
        { 
            LaughTaleEngine::app::init();
            initWindows();
            player = new bird(gameWindowId);
            pilars.push_back(new pilar(gameWindowId));

            LaughTaleEngine::entityTaleId gameEntityId = LaughTaleEngine::entityManger::addEntity(this);

            LaughTaleEngine::eventManger::addEvent(LaughTaleEngine::events::WindowClose, WindowClose);
            LaughTaleEngine::eventManger::addEvent(LaughTaleEngine::events::ImGuiRender, onImGui, player->playerEntityId, debugInfoWindowId);
            LaughTaleEngine::eventManger::addEvent(LaughTaleEngine::events::AppUpdate, onUpdate, gameEntityId, gameWindowId);
        }

        void TearDown() 
        { 
            LaughTaleEngine::app::close();
        }
};




TEST_F(flappyBird, testGames)
{
    LaughTaleEngine::app::run();
}
