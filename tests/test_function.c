#include "minunit.h"
#include "function.h"

void test_setup(void) {
	/* Nothing */
}

void test_teardown(void) {
	/* Nothing */
}

MU_TEST(test_distance) {
	mu_assert_int_eq(7, distance(5, 5, 10, 10));
	mu_assert_int_eq(40, distance(46, 45, 65, 81));
	mu_assert_int_eq(89, distance(18, 75, 96, 32));
	mu_assert_int_eq(64, distance(15, 89, 52, 36));
	mu_assert_int_eq(21, distance(18, 82, 39, 76));
	mu_assert_int_eq(50, distance(14, 52, 36, 97));
	mu_assert_int_eq(68, distance(19, 73, 82, 46));
	mu_assert_int_eq(20, distance(59, 48, 73, 63));
	mu_assert_int_eq(36, distance(35, 57, 68, 42));
}

MU_TEST_SUITE(test_suite) {
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	MU_RUN_TEST(test_distance);
}

int main(void) {
	MU_RUN_SUITE(test_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}