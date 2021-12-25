#pragma once
#include "LaughTaleEngine.h"
#include "pieces/pawn.h"

class board : public LTE::component
{
private:
    LTE::entityTaleId whitePawnsId[8];

    LTE::entityTaleId blackPawnsId[8];

    LTE::entityTaleId TilesId[8][8];

    LTE::component *black = new LTE::material(glm::vec4({0.1f, 0.1f, 0.1f, 1}));
    LTE::component *white = new LTE::material(glm::vec4({0.9, 0.9, 0.9, 1}));

    LTE::windowPieceId windowId;

    std::vector<piece *> pieces;
    std::vector<LTE::entityTaleId> blueTilesIds;

    float tilePostions[20] =
        {
            -0.5, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5, 0.5f, 0.0f, 1.0f, 1.0f};

    unsigned int tileIndices[6] =
        {
            0,
            1,
            2,
            0,
            3,
            2,
        };

    bool isWhiteTurn = true;
    piece *activePiece = nullptr;

public:
    board(LTE::windowPieceId windowId) : windowId(windowId)
    {

        genTiles();
        genWhitePawns();
        genBlackPawns();
    }

    void genTiles()
    {
        std::string boardStr = "\n";

        for (size_t y = 0; y < 8; y++)
        {
            boardStr += "\n\t" + std::to_string(7 - y);
            for (size_t x = 0; x < 8; x++)
            {
                TilesId[y][x] = LTE::entityManger::addEntity(
                    [=](LTE::gameObject::gameObjectBuilder *tileBuild)
                    {
                        tileBuild->
                            setObjectName("tile " + std::to_string(x) + ", " + std::to_string(y))->setObjectTransform({{(x - 4.0f) * 0.25f + 0.125f, (y - 4.0f) * 0.25f + 0.125f, 0.0f}, {0, 0, 0}, {0.25f, 0.25f, 0.0f}})->
                            setWindowId(windowId)->
                            addComponent(LTE::mesh::build([&](LTE::mesh::meshBuilder *builder)
                                { 
                                    builder->
                                        setIndexBuffer(tileIndices, 6)->
                                        setShaderName("res/checkmate/shaders/piece.glsl")->
                                        setVertexBuffer(tilePostions, 20 * sizeof(float)); }))->
                            addComponent((x + y % 2) % 2 == 1 ? white : black);
                    });
                boardStr += std::string((x + y % 2) % 2 == 1 ? "B" : "W");
            }
        }
        LAUGHTALE_ENGINR_LOG_INFO(boardStr << "\n\t 01234567");
    }

    void genWhitePawns()
    {
        for (size_t x = 0; x < 8; x++)
        {
            piece *nextWhitePawn = new pawn();
            pieces.push_back(nextWhitePawn);
            whitePawnsId[x] = LTE::entityManger::addEntity(
                [=](LTE::gameObject::gameObjectBuilder *tileBuild)
                { 
                    tileBuild->
                        setObjectName("white pawn " + std::to_string(x + 1))->
                        setObjectTransform({{(x - 4.0f) * 0.25f + 0.125f, -3.0f * 0.25f + 0.125f, 0.1f}, {0, 0, 0}, {0.25f, 0.25f, 0.0f}})->
                        setWindowId(windowId)->
                        addComponent(LTE::mesh::build([&](LTE::mesh::meshBuilder *builder)
                            {
                                builder->
                                    setIndexBuffer(tileIndices, 6)->
                                    setShaderName("res/checkmate/shaders/piece.glsl")->
                                    setVertexBuffer(tilePostions, 20 * sizeof(float)); }))->
                            addComponent(new LTE::material("res/checkmate/textures/whitePawn.png"))->
                            addComponent(nextWhitePawn); 
                });
        }
    }

    void genBlackPawns()
    {
        for (size_t x = 0; x < 8; x++)
        {
            piece *nextBlackPawn = new pawn(false);
            pieces.push_back(nextBlackPawn);
            blackPawnsId[x] = LTE::entityManger::addEntity([=](LTE::gameObject::gameObjectBuilder *tileBuild)
                                                           { tileBuild->setObjectName("black pawn " + std::to_string(x + 1))->setObjectTransform({{(x - 4.0f) * 0.25f + 0.125f, 2.0f * 0.25f + 0.125f, 0.1f}, {0, 0, 0}, {0.25f, 0.25f, 0.0f}})->setWindowId(windowId)->addComponent(LTE::mesh::build([&](LTE::mesh::meshBuilder *builder)
                                                                                                                                                                                                                                                                                                      { builder->setIndexBuffer(tileIndices, 6)->setShaderName("res/checkmate/shaders/piece.glsl")->setVertexBuffer(tilePostions, 20 * sizeof(float)); }))
                                                                 ->addComponent(new LTE::material("res/checkmate/textures/blackPawn.png"))
                                                                 ->addComponent(nextBlackPawn); });
        }
    }

    virtual void init(LTE::gameObject *parent) override
    {
        LTE::eventManger::startBuildingEvent()->setEventRoute("Mouse button pressed/boardHandlePress")->setEntityID(parentId)->setWindowId(winId)->setEventCallback(handleMouseClick)->add();
    }

    virtual void end() override
    {
    }

    static void handleMouseClick(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
    {
        board *gameBoard = eventEntity->getComponent<board>();
        LTE::mouseClickData *eventData = dynamic_cast<LTE::mouseClickData *>(sendor);
        if (gameBoard->activePiece)
        {
            if (eventData->button == LT_MOUSE_BUTTON_RIGHT)
            {
                gameBoard->activePiece->stopFollowingMouse();
                gameBoard->activePiece->revertPostion();
                gameBoard->activePiece = nullptr;
                gameBoard->unmarkTilesBlue();
            }
            else if (gameBoard->canMoveToNewPostion())
            {
                gameBoard->activePiece->stopFollowingMouse();
                gameBoard->removePieceAtLoction(gameBoard->getSelectedTilePostion());
                gameBoard->activePiece->move(gameBoard->getSelectedTilePostion());
                gameBoard->unmarkTilesBlue();
                gameBoard->isWhiteTurn = !gameBoard->isWhiteTurn;
                gameBoard->activePiece = nullptr;
            }
        }
        else
        {
            for (auto &gamePiece : gameBoard->pieces)
            {
                if (gamePiece->isMouseInBond(eventData->win->inputManger->GetMousePosition()) && gamePiece->isWhite == gameBoard->isWhiteTurn)
                {
                    gamePiece->startFollowingMouse();
                    gameBoard->markTilesBlue(gamePiece->getPossibleMovesList());
                    gameBoard->activePiece = gamePiece;
                    return;
                }
            }
        }
    }

    glm::vec2 getNewPiecePostion()
    {
        glm::vec2 newPostion = LTE::windowManger::getWindow(winId)->inputManger->GetMousePosition();
        newPostion = {(newPostion.x - 400) / 400, (newPostion.y - 400) / 400};
        newPostion = {(int)((newPostion.x + 0.875f) * 4), (int)((newPostion.y + 0.875f) * 4)};
        LAUGHTALE_ENGINR_LOG_INFO(newPostion.x << ", " << newPostion.y);
        newPostion = {(newPostion.x - 4.0f) * 0.25f + 0.125f, (newPostion.y - 4.0f) * 0.25f + 0.125f};
        LAUGHTALE_ENGINR_LOG_INFO(newPostion.x << ", " << newPostion.y);
        return newPostion;
    }

    void markTilesBlue(const std::vector<glm::vec2> &tilesPostions)
    {
        for (auto &tilePostion : tilesPostions)
        {
            markTileBlue(tilePostion);
        }
    }

    void markTileBlue(const glm::vec2 &tilePostion)
    {
        blueTilesIds.push_back(LTE::entityManger::addEntity(
            [&, this](LTE::gameObject::gameObjectBuilder *build)
            { build->setObjectName("blueTile " + std::to_string(blueTilesIds.size()))->
                    setObjectTransform({{tilePostion, 0.05f}, {0, 0, 0}, {0.25f, 0.25f, 0.0f}})->
                    setWindowId(winId)->
                    addComponent(
                        LTE::mesh::build([&](LTE::mesh::meshBuilder *builder){ 
                            builder->setIndexBuffer(tileIndices, 6)->setShaderName("res/checkmate/shaders/piece.glsl")->setVertexBuffer(tilePostions, 20 * sizeof(float)); 
                            }))->
                    addComponent(new LTE::material(glm::vec4({0.0f, 0.0f, 1.0f, 0.3f}))); }));
    }

    void unmarkTilesBlue()
    {
        for (auto &tileId : blueTilesIds)
        {
            unmarkTileBlue(tileId);
        }
        blueTilesIds.clear();
    }

    void unmarkTileBlue(LTE::entityTaleId TileId)
    {
        LTE::entityManger::removeEntityById(TileId);
    }

    bool canMoveToNewPostion()
    {

        glm::vec2 MousePostion = LTE::windowManger::getWindow(winId)->inputManger->GetMousePosition();
        for (auto tileId : blueTilesIds)
        {
            piece *pieceAtTile = getPieceAtTile(tileId);
            if (isMouseInTileBonds(tileId, MousePostion) && (pieceAtTile == nullptr || (pieceAtTile != nullptr && pieceAtTile->isWhite != activePiece->isWhite)))
                return true;
        }
        return false;
    }

    piece *getPieceAtTile(LTE::entityTaleId tileId)
    {
        glm::vec3 tilePostion = LTE::entityManger::getEntityById(tileId)->getTransform()->getPostion();
        return getPieceAtLocation({tilePostion.x, tilePostion.y});
    }

    void removePieceAtLoction(glm::vec2 tilePostion)
    {
        piece *pieceToRemove = getPieceAtLocation(tilePostion);
        if(pieceToRemove == nullptr)
            return;
        
        LTE::entityTaleId pieceToRemoveId = pieceToRemove->getParentId();
        pieces.erase(std::remove_if(pieces.begin(), pieces.end(),
            [=](piece *piece) -> bool
            {
                return piece->getParentId() == pieceToRemoveId;
            }));
        LTE::entityManger::removeEntityById(pieceToRemoveId);
    }

    piece *getPieceAtLocation(glm::vec2 postion)
    {
        for(auto gamePiece: pieces)
        {
            if(gamePiece->getPostion().x == postion.x && gamePiece->getPostion().y == postion.y)
                return gamePiece;
        }
        return nullptr;
    }
    
    glm::vec2 getSelectedTilePostion()
    {
        glm::vec2 MousePostion = LTE::windowManger::getWindow(winId)->inputManger->GetMousePosition();
        for (auto tileId : blueTilesIds)
        {
            if (isMouseInTileBonds(tileId, MousePostion))
            {
                LTE::gameObject *pieceData = LTE::entityManger::getEntityById(tileId);
                glm::vec3 piecePostion = pieceData->getTransform()->getPostion();
                return {piecePostion.x, piecePostion.y};
            }
        }
        return {0.0f, 0.0f};
    }

    bool isMouseInTileBonds(LTE::entityTaleId tileId, glm::vec2 mousePostion)
    {
        mousePostion = {(mousePostion.x - 400) / 400, (mousePostion.y - 400) / -400};

        LTE::gameObject *pieceData = LTE::entityManger::getEntityById(tileId);
        glm::vec3 piecePostion = pieceData->getTransform()->getPostion();
        glm::vec3 pieceScale = pieceData->getTransform()->getScale();

        return (
            (mousePostion.x > (piecePostion.x - (pieceScale.x / 2))) &&
            (mousePostion.x < (piecePostion.x + (pieceScale.x / 2))) &&
            (mousePostion.y > (piecePostion.y - (pieceScale.y / 2))) &&
            (mousePostion.y < (piecePostion.y + (pieceScale.y / 2))));
    }
};
