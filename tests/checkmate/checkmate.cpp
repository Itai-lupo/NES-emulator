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

#include "pieces/pawn.h"
#include "board.h"

class checkmate : public ::testing::Test
{
    private:
        LTE::windowPieceId gameWindowId;
        LTE::windowPieceId debugInfoWindowId;

        LTE::entityTaleId boardId;

        void initWindows()
        {

            gameWindowId =  LTE::windowManger::addWindow([=](LTE::windowBuilder *build)
            {
                build->setHeight(800)->setWidth(800)->setTitle("check mate");
            });

            debugInfoWindowId =  LTE::windowManger::addWindow([=](LTE::windowBuilder *build)
            {
                build->setHeight(600)->setWidth(600)->setTitle("debug Info")->useImGui();
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

            boardId = LTE::entityManger::addEntity([=](LTE::gameObject::gameObjectBuilder *boardBuild)
                    {
                        boardBuild->
                            setObjectName("board")->
                            setWindowId(gameWindowId)->
                            addComponent(new board(gameWindowId));
                    });
        }

        void TearDown() 
        { 
            LTE::app::close();
        }
};



TEST_F(checkmate, testGames)
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
