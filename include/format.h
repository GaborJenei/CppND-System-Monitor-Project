#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
std::string DoubleDigitString(int num); // Helper function to convert int to string with 2 digits
};                                    // namespace Format

#endif