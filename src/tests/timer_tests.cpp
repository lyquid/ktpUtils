#include "../timer.hpp"
#include <gtest/gtest.h>
#include <thread>

auto isStartedFlag(const ktp::Timer& timer) {
  return timer.started() ? testing::AssertionSuccess() : testing::AssertionFailure() << "Started flag is false.";
}

auto isStoppedFlag(const ktp::Timer& timer) {
  return timer.stopped() ? testing::AssertionSuccess() : testing::AssertionFailure() << "Stopped flag is false.";
}

auto isPausedFlag(const ktp::Timer& timer) {
  return timer.paused() ? testing::AssertionSuccess() : testing::AssertionFailure() << "Paused flag is false.";
}

// Constructors tests
TEST(TimerTests, Constructors) {
  constexpr std::chrono::nanoseconds sleep_time {1000};

  ktp::Timer clock_stopped {};
  ASSERT_TRUE(isStoppedFlag(clock_stopped));

  ktp::Timer clock_started {true};
  ASSERT_TRUE(isStartedFlag(clock_started));

  std::this_thread::sleep_for(sleep_time);
  ASSERT_EQ(clock_stopped.elapsed().count(), 0) << "Constructor without starting should not advance time counting.";
  ASSERT_GE(clock_started.elapsed().count(), sleep_time.count()) << "Constructor with start should advance time counting.";
}

// Elapsed tests
TEST(TimerTests, Elapsed) {
  constexpr std::chrono::nanoseconds sleep_time {1000};
  ktp::Timer clock {true};
  std::this_thread::sleep_for(sleep_time);
  const auto duration {clock.elapsed()};

  ASSERT_GE(duration.count(), sleep_time.count()) << "Elapsed should return the nanoseconds passed from the start of the timer.";
}

// Pause
TEST(TimerTests, Pause) {
  constexpr std::chrono::nanoseconds sleep_time {1000};
  ktp::Timer clock1 {true};
  ktp::Timer clock2 {true};

  std::this_thread::sleep_for(sleep_time);
  clock1.pause();
  ASSERT_TRUE(isPausedFlag(clock1));

  std::this_thread::sleep_for(sleep_time);
  clock2.pause();

  ASSERT_TRUE(isPausedFlag(clock2));
  ASSERT_GT(clock2.elapsed().count(), clock1.elapsed().count()) << "The timer paused last must have the biggest elapsed time.";
}

// Restart
TEST(TimerTests, Restart) {
  constexpr std::chrono::nanoseconds sleep_time {1000};
  ktp::Timer clock {true};

  std::this_thread::sleep_for(sleep_time);
  const auto time1 {clock.restart()};
  const auto time2 {clock.elapsed()};

  ASSERT_GT(time1.count(), time2.count()) << "The elapsed time should be restarted.";
}

// Resume
TEST(TimerTests, Resume) {
  constexpr std::chrono::nanoseconds sleep_time {100000};
  ktp::Timer clock {true};

  std::this_thread::sleep_for(sleep_time);
  const auto time1 {clock.elapsed()};
  clock.pause();
  clock.resume();
  const auto time2 {clock.elapsed()};

  ASSERT_GT(time2.count(), time1.count()) << "The timer should keep previous time when resumed.";
}

// Start
TEST(TimerTests, Start) {
  ktp::Timer clock {};
  const auto time1 {clock.elapsed()};
  clock.start();
  EXPECT_TRUE(isStartedFlag(clock));
  const auto time2 {clock.elapsed()};

  ASSERT_GT(time2.count(), time1.count()) << "The timer should start counting time on start.";
}

// Stop
TEST(TimerTests, Stop) {
  ktp::Timer clock {true};
  clock.stop();
  EXPECT_TRUE(isStoppedFlag(clock));
  const auto time {clock.elapsed()};

  ASSERT_EQ(time.count(), 0) << "The timer should stop.";
}

// STATIC MEMBERS TEST

// elapsedFromInit
TEST(TimerStaticTest, ElapsedFromInit) {
  const auto time {ktp::Timer::elapsedFromInit()};
  ASSERT_GT(time.count(), 0) << "The global timer should starting counting on app init.";
}

// toHours
TEST(TimerStaticTest, ToHours) {
  auto hours {ktp::Timer::toHours(std::chrono::nanoseconds {3600000000000})};
  EXPECT_EQ(hours, 1) << "3600000000000 nanoseconds should be 1 hour.";
  hours = ktp::Timer::toHours(std::chrono::microseconds {3600000000});
  EXPECT_EQ(hours, 1) << "3600000000 microseconds should be 1 hour.";
  hours = ktp::Timer::toHours(std::chrono::milliseconds {3600000});
  EXPECT_EQ(hours, 1) << "3600000 milliseconds should be 1 hour.";
  hours = ktp::Timer::toHours(std::chrono::seconds {7200});
  EXPECT_EQ(hours, 2) << "7200 seconds should be 2 hours.";
  hours = ktp::Timer::toHours(std::chrono::minutes {120});
  EXPECT_EQ(hours, 2) << "120 minutes should be 2 hours.";
  hours = ktp::Timer::toHours(std::chrono::hours {3});
  EXPECT_EQ(hours, 3) << "3 hours should be 3 hours.";
}

// toMicroseconds
TEST(TimerStaticTest, ToMicroseconds) {
  auto micro {ktp::Timer::toMicroseconds(std::chrono::nanoseconds {1000})};
  EXPECT_EQ(micro, 1) << "1000 nanoseconds should be 1 microsecond.";
  micro = ktp::Timer::toMicroseconds(std::chrono::microseconds {10});
  EXPECT_EQ(micro, 10) << "10 microseconds should be 10 microseconds.";
  micro = ktp::Timer::toMicroseconds(std::chrono::milliseconds {1});
  EXPECT_EQ(micro, 1000) << "1 millisecond should be 1000 microseconds.";
  micro = ktp::Timer::toMicroseconds(std::chrono::seconds {1});
  EXPECT_EQ(micro, 1000000) << "1 second should be 1000000 microseconds.";
  micro = ktp::Timer::toMicroseconds(std::chrono::minutes {1});
  EXPECT_EQ(micro, 60000000) << "1 minute should be 60000000 microseconds.";
  micro = ktp::Timer::toMicroseconds(std::chrono::hours {1});
  EXPECT_EQ(micro, 3600000000) << "1 hour should be 3600000000 microseconds.";
}

// toMilliseconds
TEST(TimerStaticTest, ToMilliseconds) {
  auto milli {ktp::Timer::toMilliseconds(std::chrono::nanoseconds {1000000})};
  EXPECT_EQ(milli, 1) << "1000000 nanoseconds should be 1 millisecond.";
  milli = ktp::Timer::toMilliseconds(std::chrono::microseconds {1000});
  EXPECT_EQ(milli, 1) << "1000 microseconds should be 1 millisecond.";
  milli = ktp::Timer::toMilliseconds(std::chrono::milliseconds {10});
  EXPECT_EQ(milli, 10) << "10 milliseconds should be 10 milliseconds.";
  milli = ktp::Timer::toMilliseconds(std::chrono::seconds {1});
  EXPECT_EQ(milli, 1000) << "1 second should be 1000 milliseconds.";
  milli = ktp::Timer::toMilliseconds(std::chrono::minutes {1});
  EXPECT_EQ(milli, 60000) << "1 minute should be 60000 milliseconds.";
  milli = ktp::Timer::toMilliseconds(std::chrono::hours {1});
  EXPECT_EQ(milli, 3600000) << "1 hour should be 3600000 milliseconds.";
}

// toMinutes
TEST(TimerStaticTest, ToMinutes) {
  auto minutes {ktp::Timer::toMinutes(std::chrono::nanoseconds {60000000000})};
  EXPECT_EQ(minutes, 1) << "60000000000 nanoseconds should be 1 minute.";
  minutes = ktp::Timer::toMinutes(std::chrono::microseconds {60000000});
  EXPECT_EQ(minutes, 1) << "60000000 microseconds should be 1 minute.";
  minutes = ktp::Timer::toMinutes(std::chrono::milliseconds {60000});
  EXPECT_EQ(minutes, 1) << "60000 milliseconds should be 1 minute.";
  minutes = ktp::Timer::toMinutes(std::chrono::seconds {60});
  EXPECT_EQ(minutes, 1) << "60 seconds should be 1 minute.";
  minutes = ktp::Timer::toMinutes(std::chrono::minutes {15});
  EXPECT_EQ(minutes, 15) << "15 minutes should be 15 minutes.";
  minutes = ktp::Timer::toMinutes(std::chrono::hours {1});
  EXPECT_EQ(minutes, 60) << "1 hour should be 60 minutes.";
}

// toSeconds
TEST(TimerStaticTest, ToSeconds) {
  auto seconds {ktp::Timer::toSeconds(std::chrono::nanoseconds {1000000000})};
  EXPECT_EQ(seconds, 1) << "1000000000 nanoseconds should be 1 second.";
  seconds = ktp::Timer::toSeconds(std::chrono::microseconds {1000000});
  EXPECT_EQ(seconds, 1) << "1000000 microseconds should be 1 second.";
  seconds = ktp::Timer::toSeconds(std::chrono::milliseconds {1000});
  EXPECT_EQ(seconds, 1) << "1000 milliseconds should be 1 second.";
  seconds = ktp::Timer::toSeconds(std::chrono::seconds {42});
  EXPECT_EQ(seconds, 42) << "42 seconds should be 42 seconds.";
  seconds = ktp::Timer::toSeconds(std::chrono::minutes {2});
  EXPECT_EQ(seconds, 120) << "2 minutes should be 120 seconds.";
  seconds = ktp::Timer::toSeconds(std::chrono::hours {1});
  EXPECT_EQ(seconds, 3600) << "1 hour should be 3600 seconds.";
}
