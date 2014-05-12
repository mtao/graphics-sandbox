CMAKE_MINIMUM_REQUIRED( VERSION 2.8.9 )


ADD_EXECUTABLE(kdopTest src/test_kdop.cpp include/kdop.h)


ADD_TEST(kdopTest ${CMAKE_CURRENT_BINARY_DIR}/kdopTest)







add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS kdopTest)
