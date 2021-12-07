#include "piece.h"

class pawn: public piece
{
    private:
        bool hasMoved = false;
        LTE::entityTaleId blueTile1;
        LTE::entityTaleId blueTile2;
    public:
        void move(LTE::gameObject *eventEntity)
        {
            if(moveingPieceName == "")
            {
                moveingPieceName = eventEntity->getName();
                eventEntity->getTransform()->setZPostion(1.0f);
                LTE::eventManger::startBuildingEvent()->
                    setEventRoute("Mouse moved/" + eventEntity->getName())->
                    setEntityID(parentId)->
                    setWindowId(winId)->
                    setEventCallback(followMouse)->
                    add();
                    
                blueTile1 = LTE::entityManger::addEntity([=](LTE::gameObject::gameObjectBuilder *build)
                {
                    LTE::transform temp = *eventEntity->getTransform();
                    temp.changeYPostion(0.25f);
                    temp.changeZPostion(-0.25f);
                    build->setObjectName("blueTile1")->setObjectTransform({temp.getPostion(), temp.getRotation(), temp.getScale()})->
                            setWindowId(winId)->
                            addComponent( LTE::mesh::build([&](LTE::mesh::meshBuilder *builder){
                                        builder->
                                            setIndexBuffer(tileIndices, 6)->
                                            setShaderName("res/checkmate/shaders/piece.glsl")->
                                            setVertexBuffer(tilePostions, 20 * sizeof(float));
                                    }))->
                            addComponent( new LTE::material(glm::vec4({0.0f, 0.0f, 1.0f, 0.2f})));   
                });

                if(!hasMoved)
                {
                    blueTile2 = LTE::entityManger::addEntity([=](LTE::gameObject::gameObjectBuilder *build)
                    {
                        LTE::transform temp = *eventEntity->getTransform();
                        temp.changeYPostion(0.5f);
                        temp.setZPostion(0.05f);
                        build->setObjectName("blueTile1")->setObjectTransform({temp.getPostion(), temp.getRotation(), temp.getScale()})->
                                setWindowId(winId)->
                                addComponent( LTE::mesh::build([&](LTE::mesh::meshBuilder *builder){
                                            builder->
                                                setIndexBuffer(tileIndices, 6)->
                                                setShaderName("res/checkmate/shaders/piece.glsl")->
                                                setVertexBuffer(tilePostions, 20 * sizeof(float));
                                        }))->
                                addComponent( new LTE::material(glm::vec4({0.0f, 0.0f, 1.0f, 0.3f})));   
                    });
                }
            }
            else
            {
                if(mouseInBond(LTE::entityManger::getEntityById(blueTile1), LTE::windowManger::getWindow(winId)->inputManger))
                {
                    eventEntity->getTransform()->setPostion(LTE::entityManger::getEntityById(blueTile1)->getTransform()->getPostion());
                    moveingPieceName = "";
                    if(!hasMoved)
                        LTE::entityManger::removeEntityById(blueTile2);
                    LTE::entityManger::removeEntityById(blueTile1);
                    hasMoved = true;
                    LTE::eventManger::removeEvent("Mouse moved/" + eventEntity->getName());
                }
                else if(!hasMoved && mouseInBond(LTE::entityManger::getEntityById(blueTile2), LTE::windowManger::getWindow(winId)->inputManger))
                {
                    eventEntity->getTransform()->setPostion(LTE::entityManger::getEntityById(blueTile2)->getTransform()->getPostion());
                    moveingPieceName = "";
                    hasMoved = true;
                    LTE::entityManger::removeEntityById(blueTile2);
                    LTE::entityManger::removeEntityById(blueTile1);
                    LTE::eventManger::removeEvent("Mouse moved/" + eventEntity->getName());
                }
            }
        }

};