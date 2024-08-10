#include "fuchslib/stack.hpp"

namespace Fuchs {
	m_range<int8> INT8_RANGE{ INT8_MAX, INT8_MIN };
	m_range<uint8> UINT8_RANGE{ UINT8_MAX, 0 };
	m_range<int16> INT16_RANGE{ INT16_MAX, INT16_MIN };
	m_range<uint16> UINT16_RANGE{ UINT16_MAX, 0 };
	m_range<int32> INT32_RANGE{ INT32_MAX, INT32_MIN };
	m_range<uint32> UINT32_RANGE{ UINT32_MAX, 0 };
	m_range<intmax> INTMAX_RANGE{ INTMAX_MAX, INTMAX_MIN };
	m_range<uintmax> UINTMAX_RANGE{ UINTMAX_MAX, 0 };
}