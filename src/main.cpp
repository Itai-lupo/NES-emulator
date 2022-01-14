#include "LaughTaleEngine.h"

int main() 
{
    LTE::app::init();
    //game systems init code go here;

    LTE::app::run();
    //game systems close code go here;

    LTE::app::close();
    return 0;
}
