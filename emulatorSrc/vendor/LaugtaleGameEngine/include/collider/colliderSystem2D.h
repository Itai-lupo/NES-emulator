#pragma once
#include "squreCollider.h"

#include <vector>

namespace LTE
{
    class colliderSystem2D
    {
        private:
            std::vector<gameObject *> squreColliders;
        public:
            void init();
            void close();

            void addSqureCollider(gameObject *collider){ squreColliders.push_back(collider); }
            void removeSqureCollider(entityTaleId);

            void checkCollision();

    };
} 
