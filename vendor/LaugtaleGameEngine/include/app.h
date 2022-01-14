#pragma once
#include "core.h"
#include "OSAbstractFactory.h"
#include "osAPI.h"

namespace LTE
{
    class app
    {
        private:
            static inline OSAbstractFactory *osFactory;
            static inline osAPI *os;

        public:
            static void init();
            static void close();
            static void run();
            static uint64_t getTime();

            static bool keepRunning;
            static inline bool isRuning;
            static OSAbstractFactory *getOSAbstractFactory(){ return osFactory; }
            static osAPI *getOsAPI(){ return os; }
    };
}; 