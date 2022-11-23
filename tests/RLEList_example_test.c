#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RLEList.h"
#include "test_utilities.h"

typedef bool (*testFunc)(void);

bool basicTest();

#define TESTS_NAMES             \
    X(basicTest)               \

testFunc tests[] = {
#define X(name) name,
    TESTS_NAMES
#undef X
};

const char *tests_names[] = {
#define X(name) #name,
    TESTS_NAMES
#undef X
};

static int number_of_tests = sizeof(tests) / sizeof(tests[0]);

int main(int argc, char **argv)
{
	/*RLEList list = RLEListCreate();
	RLEListAppend(list, 'w');
	RLEListAppend(list, 'c');
	RLEListResult  result;
	RLEListExportToString(list, &result);*/
    if (argc == 1)
    {
        for (int test_idx = 0; test_idx < number_of_tests; test_idx++)
        {
            RUN_TEST(tests[test_idx], tests_names[test_idx]);
        }
        return 0;
    }
    if (argc != 2)
    {
        fprintf(stdout, "Usage: tests <test index>\n");
        return 0;
    }

    int test_idx = strtol(argv[1], NULL, 10);
    if (test_idx < 1 || test_idx > number_of_tests)
    {
        fprintf(stderr, "Invalid test index %d\n", test_idx);
        return 0;
    }

    RUN_TEST(tests[test_idx - 1], tests_names[test_idx - 1]);
    return 0;
}

bool basicTest(){
    RLEList list = RLEListCreate();
    bool result=true;
    ASSERT_TEST(list != NULL, destroy);

    //adding elements to the list
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);    // a
    ASSERT_TEST(RLEListAppend(list, 'c') == RLE_LIST_SUCCESS, destroy);    // ac
    ASSERT_TEST(RLEListAppend(list, 'b') == RLE_LIST_SUCCESS, destroy);    // acb
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);    // acba
    ASSERT_TEST(RLEListAppend(list, 'b') == RLE_LIST_SUCCESS, destroy);    // acbab
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);    // acbaba
    ASSERT_TEST(RLEListAppend(list, 'b') == RLE_LIST_SUCCESS, destroy);    // acbabab
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);    // acbababa
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);    // acbababaa
    ASSERT_TEST(RLEListAppend(list, 'a') == RLE_LIST_SUCCESS, destroy);    // acbababaaa

    ASSERT_TEST(RLEListRemove(list, 1) == RLE_LIST_SUCCESS, destroy); // abababaaa

    // check if the represented string is "abababaaa"
    const char *s = "abababaaa";
    char it;
    for(int i=0; i<RLEListSize(list); i++)
    {
        it=RLEListGet(list, i, NULL);
        ASSERT_TEST(it == s[i++], destroy);
    }
    //check if the length's are equal
    ASSERT_TEST(RLEListSize(list)==strlen(s), destroy);
    
    destroy:
    RLEListDestroy(list);
    return result;
}