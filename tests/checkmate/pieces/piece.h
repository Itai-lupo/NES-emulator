#include "LaughTaleEngine.h"

class piece: public LTE::component
{
    protected:
        static inline std::string moveingPieceName = "";

        static inline float tilePostions[20] = 
        {
            -0.5,  0.5f, 0.0f, 0.0f, 1.0f,
            -0.5, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5, -0.5f, 0.0f, 1.0f, 0.0f, 
            0.5,  0.5f, 0.0f, 1.0f, 1.0f
        };


        static inline unsigned int tileIndices[6] = 
        {
            0, 1, 2,
            0, 3, 2,
        };
        
    public:
        piece()
        {

        }

        static void followMouse(LTE::gameObject *eventEntity, LTE::coreEventData *sendor)
        {
            LTE::mouseMoveData *eventData = static_cast< LTE::mouseMoveData *>(sendor);
            eventEntity->getTransform()->setXPostion((eventData->xPos - 400) / 400);
            eventEntity->getTransform()->setYPostion(-(eventData->yPos - 400) / 400);
        }

        bool mouseInBond(LTE::gameObject *eventEntity, LTE::coreInput *windowInput)
        {
            glm::vec2 mosePostion = windowInput->GetMousePosition();
            mosePostion = { (mosePostion.x - 400) / 400, -(mosePostion.y - 400) / 400};
            glm::vec3 piecePostiom = eventEntity->getTransform()->getPostion();
            glm::vec3 pieceScale = eventEntity->getTransform()->getScale();

            return (
                (mosePostion.x > (piecePostiom.x - (pieceScale.x / 2)))  &&
                (mosePostion.x < (piecePostiom.x + (pieceScale.x / 2)))  &&
                (mosePostion.y > (piecePostiom.y - (pieceScale.y / 2)))  &&
                (mosePostion.y < (piecePostiom.y + (pieceScale.y / 2)))
            );

        }

        virtual void init(LTE::gameObject *parent) override
        {
            LTE::eventManger::addCoustemEventsRoute("Mouse button pressed/move piece/");
            LTE::eventManger::startBuildingEvent()->
                setEventRoute("Mouse button pressed/move piece/" + parent->getName())->
                setEntityID(parentId)->
                setWindowId(winId)->
                setEventCallback(
                    [=, this](LTE::gameObject *eventEntity, LTE::coreEventData *sendor){ 
                        if((mouseInBond(eventEntity, sendor->win->inputManger) && this->moveingPieceName == "") || parent->getName() == this->moveingPieceName)
                            this->move(eventEntity); 
                    })->add();
        }

        virtual void end() override
        {

        }

        virtual void move(LTE::gameObject *eventEntity) = 0;

};
