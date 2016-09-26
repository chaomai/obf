#ifndef COMMON_H_
#define COMMON_H_

#include <chrono>
#include <iostream>
#include <string>

class Timer {
 public:
  Timer()
      : start_time(std::chrono::high_resolution_clock::now()),
        last_time(start_time) {}
  ~Timer() {
    std::cout << "total: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::high_resolution_clock::now() - start_time)
                     .count()
              << " ms" << std::endl;
  }

  void print_passed(const std::string& msg = "") {
    if (msg.size() > 0) {
      std::cout << msg << ", ";
    }

    auto now = std::chrono::high_resolution_clock::now();
    std::cout << "passed: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     now - last_time)
                     .count()
              << " ms" << std::endl;
    last_time = now;
  }

 private:
  std::chrono::high_resolution_clock::time_point start_time;
  std::chrono::high_resolution_clock::time_point last_time;
};

#endif  // COMMON_H_
