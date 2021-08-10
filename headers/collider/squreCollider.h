#pragma once
#include "component.h"
#include "core.h"
#include "entity.h"
#include <vector>

namespace LTE
{
    class squreCollider: public component
    {
        private:
            std::vector<squreCollider *> InCollisionWith;
        public:
            squreCollider();
            ~squreCollider();

            virtual void init(gameObject *parent) override;
            virtual void end() override;
            transform *getBonds() const;

            bool operator == (squreCollider& s);

    };
} 