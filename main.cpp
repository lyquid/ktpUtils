#include "src/colors.hpp"
#include "src/object_pool.hpp"
#include "src/timer.hpp"
#include <iostream>
#include <thread>

int main(int argv, char* args[]) {
  std::cout.sync_with_stdio(false);
  std::cout << std::fixed;

  ktp::Timer clock {true};

  std::this_thread::sleep_for(std::chrono::seconds(1));

  clock.pause();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "ms clock count: " << ktp::Timer::toMilliseconds(clock.elapsed()) << "\n";
  clock.resume();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "ms clock count after resume: " << ktp::Timer::toMilliseconds(clock.elapsed()) << "\n";
  clock.stop();
  std::cout << "ms clock count after stop: " << ktp::Timer::toMilliseconds(clock.elapsed()) << "\n";

  ktp::Color color {123, 255, 1, 65};
  ktp::IndexedObjectPool<ktp::Timer> indexed_pool {600000};
  ktp::ObjectPool<ktp::Timer> pool {1000000};

  std::cout << "program time: " << ktp::Timer::elapsedFromInit().count() << "ns.\n";
  std::cout << "program time: " << ktp::Timer::toMicroseconds(ktp::Timer::elapsedFromInit()) << "mls.\n";
  std::cout << "program time: " << ktp::Timer::toMilliseconds(ktp::Timer::elapsedFromInit()) << "ms.\n";
  std::cout << "program time: " << ktp::Timer::toSeconds(ktp::Timer::elapsedFromInit()) << "s.\n";

  return 0;
}
