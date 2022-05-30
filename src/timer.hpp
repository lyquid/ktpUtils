/**
 * @file timer.hpp
 * @author Alejandro Castillo Blanco (alex@tinet.org)
 * @brief Timer class and related functions.
 * @version 0.1
 * @date 2022-05-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef KTP_UTILS_TIMER_HPP_
#define KTP_UTILS_TIMER_HPP_

#include <chrono>

namespace ktp {

/**
 * @brief A simple timer class useful for games.
 */
class Timer {

  using Clock     = std::chrono::steady_clock;
  using Duration  = Clock::duration;
  using TimePoint = Clock::time_point;

 public:

  /**
   * @brief Construct a new Timer object
   * @param start True if you want the clock to start right aay.
   */
  constexpr Timer(bool start = false) noexcept {
    if (start) Timer::start();
  }

  /**
   * @brief Time passed from program start.
   * @return Duration in nanoseconds from program start.
   */
  static Duration elapsedFromInit() { return now() - s_initialization_time; }

  /**
   * @brief Convert a duration to hours.
   * @param duration The duration to convert.
   * @return An int64_t representing how many hours passed in the given duration.
   */
  static auto toHours(const Duration& duration) {
    return std::chrono::duration_cast<std::chrono::hours>(duration).count();
  }

  /**
   * @brief Convert a duration to microseconds, 1/1.000.000 seconds.
   * @param duration The duration to convert.
   * @return An int64_t representing how many microseconds passed in the given duration.
   */
  static auto toMicroseconds(const Duration& duration) {
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
  }

  /**
   * @brief Convert a duration to milliseconds, 1/1000 seconds.
   * @param duration The duration to convert.
   * @return An int64_t representing how many milliseconds passed in the given duration.
   */
  static auto toMilliseconds(const Duration& duration) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  }

  /**
   * @brief Convert a duration to minutes.
   * @param duration The duration to convert.
   * @return An int64_t representing how many minutes passed in the given duration.
   */
  static auto toMinutes(const Duration& duration) {
    return std::chrono::duration_cast<std::chrono::minutes>(duration).count();
  }

  /**
   * @brief Convert a duration to seconds.
   * @param duration The duration to convert.
   * @return An int64_t representing how many seconds passed in the given duration.
   */
  static auto toSeconds(const Duration& duration) {
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
  }

  /**
   * @return Duration of the elapsed time in nanosecons (1/1.000.000.000 seconds).
   */
  constexpr Duration elapsed() const {
    if (m_started) return m_paused ? m_paused_time - m_started_time : now() - m_started_time;
    return {};
  }

  /**
   * @brief Pauses the timer.
   */
  constexpr void pause() {
    if (m_started && !m_paused) {
      m_paused_time = now();
      m_paused = true;
    }
  }

  /**
   * @return True if the timer is paused.
   */
  constexpr bool paused() const { return m_paused; }

  /**
   * @brief Restarts the timer, resetting the elapsed duration.
   * @return Duration of the elapsed time in nanosecons (1/1.000.000.000 seconds).
   */
  const Duration restart() {
    const auto time {elapsed()};
    m_started_time = now();
    m_paused_time = {};
    m_started = true;
    m_stopped = false;
    m_paused = false;
    return time;
  }

  /**
   * @brief Resumes counting the time.
   */
  constexpr void resume() {
    if (m_started && m_paused) {
      // we substract the total duration of the pause
      m_started_time = now() - (m_paused_time - m_started_time);
      m_paused = false;
    }
  }

  /**
   * @brief Starts counting the time.
   */
  void start() {
    m_started_time = now();
    m_paused_time = {};
    m_started = true;
    m_stopped = false;
    m_paused = false;
  }

  /**
   * @return True it the timer has been started.
   */
  constexpr bool started() const { return m_started; }

  /**
   * @brief Stops the timer resetting elapsed time.
   */
  constexpr void stop() {
    m_started = false;
    m_paused = false;
    m_stopped = true;
    m_started_time = {};
    m_paused_time = {};
  }

  /**
   * @return True if the timer is stopped.
   */
  constexpr bool stopped() const { return m_stopped; }

 private:

  static const TimePoint now() { return Clock::now(); }

  static inline const TimePoint s_initialization_time {now()};
  TimePoint m_paused_time {};
  TimePoint m_started_time {};
  bool m_paused {false};
  bool m_started {false};
  bool m_stopped {true};
};

} // end namespace ktp

#endif // KTP_UTILS_TIMER_HPP_
