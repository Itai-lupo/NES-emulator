#pragma once
#include <functional>
#include <vector>
#include "core.h"

namespace LTE
{
    class gameObject;
    class component
    {
        protected:
            windowPieceId winId;
            entityTaleId parentId;
        public:
            virtual ~component() = default;
            virtual void init(gameObject *parent) = 0;
            virtual void end() = 0;

            void setParent(entityTaleId parentId, windowPieceId winId)
            {
                this->parentId = parentId;
                this->winId = winId;
            }
    };

}