/**
 * @file libppm.hpp
 * @author Alejandro Castillo Blanco (alexcastilloblanco@gmail.com)
 * @brief A library for creating portable pixmap format (PPM) image files.
 * @version 0.1
 * @date 2022-10-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef KTP_LIBPPM_HPP_
#define KTP_LIBPPM_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace ppm {

// **** forward declarations ****
struct Color;
using Pixels = std::vector<Color>;
void writePixel(std::ostream& out, const Color& color);
// **** forward declarations ****

/**
 * @brief Clamps a value between 2 numbers.
 * @param x The value to check.
 * @param min The minimun number.
 * @param max The maximun number.
 * @return double A number that is always between min and max.
 */
inline double clamp(double x, double min, double max) {
  if (x < min) return min;
  if (x > max) return max;
  return x;
}

/**
 * @brief A RGB color [0, 1].
 */
struct Color {
  Color(double red, double green, double blue): r(red), g(green), b(blue) {}
  double r{}, g{}, b{};
};

inline Color operator+(const Color& col1, const Color& col2) {
  return Color(col1.r + col2.r, col1.g + col2.g, col1.b + col2.b);
}

inline Color operator-(const Color& col1, const Color& col2) {
  return Color(col1.r - col2.r, col1.g - col2.g, col1.b - col2.b);
}

inline Color operator*(double t, const Color& color) {
  return Color(t * color.r, t * color.g, t * color.b);
}

inline Color operator*(const Color& color, double t) {
  return t * color;
}

inline Color operator/(Color color, double t) {
  return (1 / t) * color;
}

/**
 * @brief Struct containing the info needed to generate a ppm file.
 */
struct PPMFileData {
  const int   m_channels_per_color {3};
  int         m_height {};
  std::string m_name {};
  Pixels      m_pixels {};
  int         m_width {};
};

/**
 * @brief Generates a test image to see if everything is working.
 * @param data The info of the file to be generated.
 */
inline void generateTestImage(PPMFileData& data) {
  for (auto row = data.m_height - 1; row >= 0; --row) {
    std::cout << "\rScanlines remaining: " << row << ' ' << std::flush;
    for (auto col = 0; col < data.m_width; ++col) {
      const Color color {
        static_cast<double>(col) / (data.m_width  - 1),
        static_cast<double>(row) / (data.m_height - 1),
        0.25
      };
      data.m_pixels.push_back(color);
    }
  }
  std::cout << "\rScanlines processing finished.\n";
}

/**
 * @brief Generates a ppm file.
 * @param data The data of the ppm file.
 */
inline void makePPMFile(const PPMFileData& data) {
  std::ofstream image_file {data.m_name};
  // header for ppm file
  image_file << "P3\n" << data.m_width << ' ' << data.m_height << "\n255\n";
  std::size_t percent {0u};
  // write pixels to file. 1 RGB triplet per row
  for (std::size_t i = 0u; i < data.m_pixels.size(); ++i) {
    writePixel(image_file, data.m_pixels[i]);
    // if percentage changes, print it
    if (percent != static_cast<std::size_t>((i * 100u) / data.m_pixels.size())) {
      percent = static_cast<std::size_t>((i * 100u) / data.m_pixels.size());
      std::cout << "\rGenerating ppm file: " << percent << "% " << std::flush;
    }
  }
  image_file.close();
  std::cout << "\rPPM file generated successfully!\n";
}

/**
 * @brief Writes a pixel (rgb [0, 255] triplet) to the given stream.
 * @param out The output stream. Recommended to be a file.
 * @param color A color in rgb [0, 1] format.
 */
inline void writePixel(std::ostream& out, const Color& color) {
  constexpr auto magic_num {256};
  out << static_cast<int>(magic_num * clamp(color.r, 0.0, 0.999)) << ' '
      << static_cast<int>(magic_num * clamp(color.g, 0.0, 0.999)) << ' '
      << static_cast<int>(magic_num * clamp(color.b, 0.0, 0.999)) << '\n';
}

} // namespace libppm

#endif // LIBPPM_HPP_
