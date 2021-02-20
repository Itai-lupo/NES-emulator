#include <gtest/gtest.h>
#include "raftelGraphicEngine.h"
#include <iostream>
#include <stdio.h>


TEST(test, test)
{
    printf("hello world\n");
    std::cout << "hello world" << std::endl; 

}


int main(int argc, char **argv) {
    std::cout << argv[1] << std::endl; 
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}