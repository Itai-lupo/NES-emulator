#include "LaughTaleEngine.h"
#include "dataStructers/router.h"
#include <gtest/gtest.h>


TEST(router, routerCRDTest)
{
    LTE::router<int> testRouter;
    testRouter.addRoute("route/to/value/");
    testRouter.addValue("route/to/value/name", 3);
    LAUGHTALE_ENGINR_LOG_INFO(testRouter["route/to1/value /name"]);
    testRouter.removeValue("route/to/value/name");
    LAUGHTALE_ENGINR_LOG_INFO(testRouter.getValue("route/to/value/name"));
}

struct testClass
{
    std::string name;
    int id;
    testClass(std::string name, int id): name(name), id(id){}

    friend std::ostream& operator << (std::ostream& os, testClass *msg)
    {
        if(!msg) return os << "NULL ";
        return os << msg->name << ", " << msg->id << ", ";
    }
    
};


TEST(router, routerCRDObjectTest)
{
    LTE::router<testClass*> testRouter;
    testClass *a = new testClass("a", 1);
    testClass *b = new testClass("b", 2);
    testClass *c = new testClass("c", 3);
    testClass *d = new testClass("d", 4);

    testRouter.addRoute("test/route/");
    testRouter.addRoute("test/route/to/");
    testRouter.addRoute("test1/route/");

    testRouter.addValue("test1/route/c", c);
    testRouter.addValue("test1/route/d", b);
    testRouter.addValue("test/route/a", a);
    testRouter.addValue("test/route/to/b", b);
    testRouter.addValue("d", d);
    testRouter.deleteValue("d");
    testRouter.removeValue("test/route/to/b");

    LAUGHTALE_ENGINR_LOG_INFO(testRouter["test/route/a"]  << testRouter["test/route/to/b"] << testRouter["test1/route/c"] << testRouter["d"]);

    testRouter.itrateFrom([](testClass *t){
        LAUGHTALE_ENGINR_LOG_INFO(t);
    });

    
    
}