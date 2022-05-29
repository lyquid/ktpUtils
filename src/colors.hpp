/**
 * @file colors.hpp
 * @author Alejandro Castillo Blanco (alex@tinet.org)
 * @brief Color class and useful color functions.
 * @version 0.1
 * @date 2022-05-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef KTP_UTILS_COLORS_HPP_
#define KTP_UTILS_COLORS_HPP_

namespace ktp {

/**
 * @brief A RGBA color representation with range [0,1] for channel.
 */
class Color {
 public:

  constexpr Color(float red = 0.f, float green = 0.f, float blue = 0.f, float alpha = 1.f):
    r(red), g(green), b(blue), a(alpha) {}

  constexpr Color(int red, int green, int blue, int alpha = 255):
    r((float)red * kInv), g((float)green * kInv), b((float)blue * kInv), a((float)alpha * kInv) {}

  constexpr Color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha = 255u):
    r((float)red * kInv), g((float)green * kInv), b((float)blue * kInv), a((float)alpha * kInv) {}

  float r, g, b, a;

  /**
   * @brief Gives you the inverse of 255.
   * @return Something like 0.003921568859f.
   */
  static constexpr auto inv255() { return kInv; }

 private:

  static constexpr float kInv {1.f / 255.f};
};

} // end namespace ktp

#endif // KTP_UTILS_COLORS_HPP_
