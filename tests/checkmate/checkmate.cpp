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

class board : public ::testing::Test
{
    private:
        LTE::windowPieceId gameWindowId;
        LTE::windowPieceId debugInfoWindowId;


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

    private:
        float tilePostions[20] = 
        {
            -0.5,  0.5f, 0.0f, 0.0f, 1.0f,
            -0.5, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5, -0.5f, 0.0f, 1.0f, 0.0f, 
             0.5,  0.5f, 0.0f, 1.0f, 1.0f
        };


        unsigned int tileIndices[6] = 
        {
            0, 1, 2,
            0, 3, 2,
        };
        

    public:        
        void SetUp()
        { 
            LTE::app::init();
            initWindows();

            LTE::component *black = new LTE::material(glm::vec4({0.1f, 0.1f, 0.1f, 1}));
            LTE::component *white = new LTE::material(glm::vec4({0.9, 0.9, 0.9, 1}));

            LTE::entityTaleId tiles[8][8];
            LTE::entityTaleId whitePawns[8];
            LTE::entityTaleId blackPawns[8];

            for (size_t y = 0; y < 8; y++)
            {
                for (size_t x = 0; x < 8; x++)
                {
                    tiles[y][x] = LTE::entityManger::addEntity([=](LTE::gameObject::gameObjectBuilder *tileBuild)
                    {
                        tileBuild->
                            setObjectName("tile " + std::to_string(x) + ", " + std::to_string(y))->
                            setObjectTransform({{(x - 4.0f) * 0.25f + 0.125f, (y - 4.0f) * 0.25f + 0.125f, 0.0f}, {0, 0, 0}, {0.25f, 0.25f, 0.0f}})->
                            setWindowId(gameWindowId)->
                            addComponent( LTE::mesh::build([&](LTE::mesh::meshBuilder *builder){
                                        builder->
                                            setIndexBuffer(tileIndices, 6)->
                                            setShaderName("res/checkmate/shaders/piece.glsl")->
                                            setVertexBuffer(tilePostions, 20 * sizeof(float));
                                    }))->
                            addComponent( (x + y % 2) % 2 == 1 ? white: black);
                    });
                }
                
            }
    
            for (size_t x = 0; x < 8; x++)
            {
                whitePawns[x] = LTE::entityManger::addEntity([=](LTE::gameObject::gameObjectBuilder *tileBuild)
                    {
                        tileBuild->
                            setObjectName("white pawn " + std::to_string(x + 1))->
                            setObjectTransform({{(x - 4.0f) * 0.25f + 0.125f, -3.0f * 0.25f + 0.125f, 0.1f}, {0, 0, 0}, {0.25f, 0.25f, 0.0f}})->
                            setWindowId(gameWindowId)->
                            addComponent( LTE::mesh::build([&](LTE::mesh::meshBuilder *builder){
                                        builder->
                                            setIndexBuffer(tileIndices, 6)->
                                            setShaderName("res/checkmate/shaders/piece.glsl")->
                                            setVertexBuffer(tilePostions, 20 * sizeof(float));
                                    }))->
                            addComponent( new LTE::material("res/checkmate/textures/whitePawn.png"))->
                            addComponent(new pawn());
                    });
            }
    
            for (size_t x = 0; x < 8; x++)
            {
                blackPawns[x] = LTE::entityManger::addEntity([=](LTE::gameObject::gameObjectBuilder *tileBuild)
                    {
                        tileBuild->
                            setObjectName("black pawn " + std::to_string(x + 1))->
                            setObjectTransform({{(x - 4.0f) * 0.25f + 0.125f, 2.0f * 0.25f + 0.125f, 0.1f}, {0, 0, 0}, {0.25f, 0.25f, 0.0f}})->
                            setWindowId(gameWindowId)->
                            addComponent( LTE::mesh::build([&](LTE::mesh::meshBuilder *builder){
                                        builder->
                                            setIndexBuffer(tileIndices, 6)->
                                            setShaderName("res/checkmate/shaders/piece.glsl")->
                                            setVertexBuffer(tilePostions, 20 * sizeof(float));
                                    }))->
                            addComponent( new LTE::material("res/checkmate/textures/blackPawn.png"))->
                            addComponent(new pawn());
                    });
            }
        }

        void TearDown() 
        { 
            LTE::app::close();
        }
};



TEST_F(board, testGames)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    LTE::app::run();
}
