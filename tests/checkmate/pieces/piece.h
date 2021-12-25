#include "LaughTaleEngine.h"

class piece: public LTE::component
{
    protected:
        int dirction;
        std::string currentTileName;
        glm::vec3 postion;
        
    public:
        bool isWhite;
        piece(bool isWhite = true): isWhite(isWhite), dirction(isWhite * 2 - 1)
        {

        }

        virtual void init(LTE::gameObject *parent) override
        {
            postion = parent->getTransform()->getPostion();
        }

        virtual void move(glm::vec2 newPostion)
        {
            LTE::gameObject *pieceEntity = LTE::entityManger::getEntityById(parentId);
            this->postion = {newPostion, 0.1f};
            pieceEntity->getTransform()->setPostion({newPostion, 0.1f});
        }

        virtual void end() override
        {

        }

        virtual std::vector<glm::vec2> getPossibleMovesList() = 0;


        bool isMouseInBond(glm::vec2 mousePostion)
        {
            mousePostion = {(mousePostion.x - 400) / 400, (mousePostion.y - 400) / -400};

            LTE::gameObject *pieceData = LTE::entityManger::getEntityById(parentId);
            glm::vec3 piecePostiom = pieceData->getTransform()->getPostion();
            glm::vec3 pieceScale = pieceData->getTransform()->getScale();

            return (
                (mousePostion.x > (piecePostiom.x - (pieceScale.x / 2)))  &&
                (mousePostion.x < (piecePostiom.x + (pieceScale.x / 2)))  &&
                (mousePostion.y > (piecePostiom.y - (pieceScale.y / 2)))  &&
                (mousePostion.y < (piecePostiom.y + (pieceScale.y / 2)))
            );

        }

        void startFollowingMouse()
        {
            LTE::gameObject *pieceEntity = LTE::entityManger::getEntityById(parentId);
            postion = pieceEntity->getTransform()->getPostion();
            pieceEntity->getTransform()->setZPostion(1.0f);
            LTE::eventManger::startBuildingEvent()->
                setEventRoute("Mouse moved/" + pieceEntity->getName())->
                setEntityID(parentId)->
                setWindowId(winId)->
                setEventCallback(followMouse)->
                add();               
        }
        
        void stopFollowingMouse()
        {
            LTE::gameObject *pieceEntity = LTE::entityManger::getEntityById(parentId);
            LTE::eventManger::removeEvent("Mouse moved/" + pieceEntity->getName());
        }

        void revertPostion()
        {
            LTE::gameObject *pieceEntity = LTE::entityManger::getEntityById(parentId);
            pieceEntity->getTransform()->setPostion(postion);

        }

        virtual glm::vec3 getPostion(){ return postion; }

    private:
        static void followMouse(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            LTE::mouseMoveData *eventData = static_cast< LTE::mouseMoveData *>(sendor);
            eventEntity->getTransform()->setXPostion((eventData->xPos - 400) / 400);
            eventEntity->getTransform()->setYPostion(-(eventData->yPos - 400) / 400);
        }

};
