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

class player: public LTE::component
{
    private:
        static inline int updateCounter = 0;
        static inline int animationCounter = 0;
        static inline const float playerSpeed = 0.6f;
    public:
        virtual void init(LTE::gameObject *parent)
        {
            LTE::eventManger::startBuildingEvent()->
            setEntityID(parentId)->
            setEventCallback(playerMove)->setEventRoute("window render/rpg/playerMove")->setWindowId(winId)->add();
        }

        virtual void end()
        {

        }

        static void playerMove(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            LTE::onUpdateData *eventData = dynamic_cast<LTE::onUpdateData *>(sendor);

            LTE::material *playerMatreial = eventEntity->getComponent<LTE::material>();
            LTE::transform *playerPostion = eventEntity->getTransform();

            bool goLeft = eventData->win->inputManger->isKeyPressed(LT_KEY_LEFT);
            bool goRight = eventData->win->inputManger->isKeyPressed(LT_KEY_RIGHT);
            bool goUp = eventData->win->inputManger->isKeyPressed(LT_KEY_UP);
            bool goDown = eventData->win->inputManger->isKeyPressed(LT_KEY_DOWN);
            
            playerMatreial->setTileIndex(1, 3);
            if(goRight)
            {
                playerMatreial->setTileIndex(animationCounter, 1);
                playerPostion->changeXPostion(playerSpeed * eventData->DeltaTime / 1000.0f);
            } 
            
            if(goLeft)
            {
                playerMatreial->setTileIndex(animationCounter, 2);
                playerPostion->changeXPostion(-playerSpeed  * eventData->DeltaTime / 1000.0f);
            } 
            
            if(goUp)
            {
                playerMatreial->setTileIndex(animationCounter, 0);
                playerPostion->changeYPostion(playerSpeed  * eventData->DeltaTime / 1000.0f);
            } 
            
            if(goDown)
            {
                playerMatreial->setTileIndex(animationCounter, 3);
                playerPostion->changeYPostion(-playerSpeed  * eventData->DeltaTime / 1000.0f);
            } 
            
            if(updateCounter++ % 8 == 0)
                animationCounter += animationCounter++ % 4;
        }

        player(){}
        ~player(){}
};



class topDownGame : public ::testing::Test
{
    private:


        LTE::windowPieceId gameWindowId;

        void initWindows()
        {

            gameWindowId =  LTE::windowManger::addWindow([=](LTE::windowBuilder *build)
            {
                build->setHeight(800)->setWidth(800)->setTitle("rpg");
            });
        }

        static void WindowClose(__attribute__((unused)) LTE::gameObject *eventEntity, __attribute__((unused)) LTE::coreEventData *sendor)
        {
            LTE::app::keepRunning = false;
        }        

    public:        
        void SetUp()
        { 
            LTE::app::init();
            initWindows();

            LTE::windowManger::getWindow(gameWindowId)->
                assetLibrary->
                getAsset<LTE::texture>("res/topDownScene/textures/Meta data assets files/ENVIRONMENT/tilesets/dungeon-tileset.png")->
                setSprtieData(
                    {
                        {{1, 48}, {15, 15}},
                        {{1, 66}, {16, 14}},
                    
                    });
            

            LTE::windowManger::getWindow(gameWindowId)->
                assetLibrary->
                getAsset<LTE::texture>("res/topDownScene/textures/PIPOYA FREE RPG Character Sprites NEKONIN/pipo-nekonin011.png")->
                setSprtieData({32, 32});
            

            LTE::tilemap *map = new LTE::tilemap({800, 800}, {80, 80}, 6, gameWindowId);

            for(int x = 0; x < 10; x++)
                for(int y = 0; y < 10; y++)
                    try
                    {
                        map->addTile(
                                [=](LTE::tilemap::tileBuilder *tileBuild)
                                {
                                    tileBuild->
                                        setTileMatrial(new LTE::material("res/topDownScene/textures/Meta data assets files/ENVIRONMENT/tilesets/dungeon-tileset.png", y == 9))->
                                        setLayer(0)->
                                        setTilePostion({x, y});
                                }
                        );
                    }
                    catch(const std::exception& e)
                    {
                        LAUGHTALE_ENGINR_LOG_ERROR("can't render " << e.what());
                    }
        
            try
            {
                map->addTile(
                        [=](LTE::tilemap::tileBuilder *tileBuild)
                        {
                            tileBuild->
                                setTileMatrial(new LTE::material("res/topDownScene/textures/PIPOYA FREE RPG Character Sprites NEKONIN/pipo-nekonin011.png", 1, 3))->
                                setLayer(1)->
                                setTilePostion({5, 5})->
                                AddCompponnetToTile(new player());
                        }
                );
            }
            catch(const std::exception& e)
            {
                LAUGHTALE_ENGINR_LOG_ERROR("can't render " << e.what());
            }
        }

        

        void TearDown() 
        { 
            LTE::app::close();
        }
};



TEST_F(topDownGame, topDownScene)
{
    try
    {    /* code */
        LTE::app::run();
    }
    catch(const std::exception& e)
    {
        LAUGHTALE_ENGINR_LOG_FATAL(e.what());
    }
    catch(std::exception* e)
    {
        LAUGHTALE_ENGINR_LOG_FATAL(e->what());
    }
}
