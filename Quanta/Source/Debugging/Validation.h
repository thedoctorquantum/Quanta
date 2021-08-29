#pragma once

#ifndef NDEBUG
    #define DEBUG 1
#else   
    #define DEBUG 0
#endif

#if DEBUG
    #include <assert.h>

    #define DEBUG_ASSERT(condition) { assert(condition); }
    #define DEBUG_ASSERT_MESSAGE(condition, message) { assert((condition) && (message)); } 

    #define DEBUG_FAILURE() { assert(false); }
    #define DEBUG_FAILURE_MESSAGE(message) { assert(false && (message)); }
#else
    #define DEBUG_MESSAGE(message) 

    #define DEBUG_ASSERT(condition)
    #define DEBUG_ASSERT_MESSAGE(condition, message)

    #define DEBUG_FAILURE()
    #define DEBUG_FAILURE_MESSAGE(message)
#endif