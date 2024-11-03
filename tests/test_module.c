#include "minunit.h"
#include "module.h"

void test_setup(void) {
	/* Nothing */
}

void test_teardown(void) {
	/* Nothing */
}

MU_TEST(test_five) {
	mu_assert_int_eq(25, carre(5));
}

MU_TEST(test_six) {
	mu_assert_int_eq(36, carre(6));
}

MU_TEST_SUITE(test_suite) {
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	MU_RUN_TEST(test_five);
	MU_RUN_TEST(test_six);
}

int main(void) {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}