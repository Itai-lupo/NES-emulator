#pragma once
#include "piece.h"

class pawn: public piece
{
    private:
        bool hasMoved = false;
        LTE::entityTaleId blueTile1;
        LTE::entityTaleId blueTile2;
    public:
        pawn(bool isWhite = true): piece(isWhite)
        {

        }

        virtual std::vector<glm::vec2> getPossibleMovesList()
        {
            LTE::gameObject *pieceEntity = LTE::entityManger::getEntityById(parentId);
            LTE::transform temp = *pieceEntity->getTransform();
            std::vector<glm::vec2> res;
            res.push_back({temp.getPostion().x, temp.getPostion().y + dirction * 0.25f});
            if(!hasMoved)
                res.push_back({temp.getPostion().x, temp.getPostion().y + dirction * 0.5f});

            return res;
        }


        virtual void move(glm::vec2 newPostion)
        {
            LTE::gameObject *pieceEntity = LTE::entityManger::getEntityById(parentId);
            this->postion = {newPostion, 0.1f};
            pieceEntity->getTransform()->setPostion({newPostion, 0.1f});
            hasMoved = true;
        }
};