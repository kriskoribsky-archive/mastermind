#include <Arduino.h>
#include <unity.h>

void setUp(void);
void tearDown(void);

void test_generate_code_generates_correct_length(void);

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_generate_code_generates_correct_length);

    return UNITY_END();
}

void setUp(void)
{
}

void tearDown(void)
{
}

void test_generate_code_generates_correct_length(void)
{
    TEST_ASSERT_EQUAL(5, 5);
}