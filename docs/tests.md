# Tests & Asserts

## Assertions

### nt_assert

Include: [nt/assert.h](../include/nt/assert.h)

Parameters:

* condition: A condition that is checked to be `!= 0`.
* message (`char const*`, optional): An optional message that is displayed on the screen.
* args (optional): Var args holding arguments for formatting `message`.

Ensures that a given condition is met. Otherwise a panic is raised with the source location, the condition and an
optional custom message.

Asserts can be disabled by defining the `NT_DISABLE_ASSERTS` macro.


## Test Helpers

### nt_test

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* func (`void(*)()`): A function containing the logic of the test. The name of the function is also displayed on the
  screen.

Executes the given test function and prints an according message on the screen.


### nt_assert_equal

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.
* comparator (`nt_comparison_result_t(*)(type, type)`): A function that can be used to compare the values. This is only
  necessary for non primitive types.

Checks if the expected and the actual value are equal. If not, a panic is raised.


### nt_assert_unequal

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.
* comparator (`nt_comparison_result_t(*)(type, type)`): A function that can be used to compare the values. This is only
  necessary for non primitive types.

Checks if the expected and the actual value are unequal. If not, a panic is raised.


### nt_assert_less

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.

Checks if the actual value is less than the expected one. If not, a panic is raised.


### nt_assert_less_equal

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.

Checks if the actual value is less than or equal the expected one. If not, a panic is raised.


### nt_assert_greater

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.

Checks if the actual value is greater than the expected one. If not, a panic is raised.


### nt_assert_greater_equal

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* expected: The expected value.
* actual: The actual value.

Checks if the actual value is greater than or equal the expected one. If not, a panic is raised.


### nt_assert_true

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* actual: The actual value.

Checks if the actual value evaluates to `true`. If not, a panic is raised.


### nt_assert_false

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* actual: The actual value.

Checks if the actual value evaluates to `false`. If not, a panic is raised.


### nt_assert_null

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* actual: The actual value.

Checks if the actual value evaluates to `NULL`. If not, a panic is raised.


### nt_assert_not_null

Include: [nt/test.h](../include/nt/test.h)

Parameters:

* actual: The actual value.

Checks if the actual value evaluates to a value unequal to `NULL`. If not, a panic is raised.


## Measurement

### nt_measure

Include: [nt/measurement.h](../include/nt/measurement.h)

Parameters:

* code: Code that should be measured.
* result_var: A variable of type `int64_t` where the result should be stored.

Measures the elapsed time a piece of code takes to execute in µs.


### nt_print_measure

Include: [nt/measurement.h](../include/nt/measurement.h)

Parameters:

* time (`int64_t`): The elapsed time to print in µs.

Pretty prints the passed time to the screen.
