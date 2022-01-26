#include "LaughTaleEngine.h"



class cpu
{
    private:
  
    public:
        static void init();
        static void close();

        static void clock();
        static void reset();
        static void irq();
        static void nmi();
};
