#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <core/string/ustring.h>

#define ZYLANN_TEST_ASSERT(m_cond)                                                                                     \
	if ((m_cond) == false) {                                                                                           \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "FATAL: Condition \"" #m_cond "\" is false.");              \
		GENERATE_TRAP();                                                                                               \
	}

#define ZYLANN_TEST_ASSERT_V(m_cond, m_retval)                                                                         \
	if ((m_cond) == false) {                                                                                           \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "FATAL: Condition \"" #m_cond "\" is false.");              \
		GENERATE_TRAP();                                                                                               \
	}

#define ZYLANN_TEST_ASSERT_MSG(m_cond, m_msg)                                                                          \
	if ((m_cond) == false) {                                                                                           \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "FATAL: Condition \"" #m_cond "\" is false. " #m_msg);      \
		GENERATE_TRAP();                                                                                               \
	}

namespace zylann::testing {

// Utilities for testing

class TestDirectory {
public:
	TestDirectory();
	~TestDirectory();

	bool is_valid() const {
		return _valid;
	}

	String get_path() const;

private:
	bool _valid = false;
};

} // namespace zylann::testing

#endif // TEST_UTIL_H
