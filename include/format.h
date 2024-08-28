#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // Helper function to convert seconds to HH:MM:SS
std::string DoubleDigitString(int num); // Helper function to convert int to string with 2 digits
};

#endif