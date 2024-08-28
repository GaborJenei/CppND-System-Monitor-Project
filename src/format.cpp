#include "format.h"

#include <string>

using std::string;

// Format Long int measuring seconds to HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;
  int secs = seconds % 60;

  string time = Format::DoubleDigitString(hours) + ":" +
                Format::DoubleDigitString(minutes) + ":" +
                Format::DoubleDigitString(secs);
  return time;
};

// Helper function to convert int to string with 2 digits
string Format::DoubleDigitString(int num) {
  std::string double_digit;

  if (num < 10) {
    double_digit = "0" + std::to_string(num);
  } else {
    double_digit = std::to_string(num);
  }
  return double_digit;
};