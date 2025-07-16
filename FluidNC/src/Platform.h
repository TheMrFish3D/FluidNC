#pragma once

// This contains definitions of "very platform specific defines", that cannot be dealth with some other way.

#ifdef ESP32

#    define WEAK_LINK __attribute__((weak))

#elif defined(STM32)

#    define WEAK_LINK __attribute__((weak))
#    define IRAM_ATTR // STM32 doesn't use IRAM attribute

#else

#    define WEAK_LINK
#    define IRAM_ATTR

#endif
