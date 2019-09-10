#pragma once

#include <array>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <random>
#include <limits>

#include <iostream>

#include "MurmurHash3.h"

namespace GotStd {
template <typename T_, size_t B_ = 4> class HyperLogLog {
  static_assert(B_ >= 4 && B_ < 16);
  static constexpr size_t m() { return 1ULL << B_; }
  static constexpr float alpha() {
    switch (m()) {
    case 16:
      return 0.673f;
    case 32:
      return 0.697f;
    case 64:
      return 0.709f;
    default:
      return 0.7213f / (1.0f + 1.097f / m());
    }
  }
  std::array<uint32_t, m()> M_ = {0};

public:
  [[nodiscard]] inline float E() const noexcept {
    constexpr auto m2a = m() * m() * alpha();
    return m2a * Z();
  }
  [[nodiscard]] inline float Estar() const noexcept {
    auto En = E();
    if (2 * En < 5 * m()) {
      // small E optimize
      auto V = std::count(M_.cbegin(), M_.cend(), 0);
      return V > 0 ? m() * std::log(1.0f * m() / V) : En;
    } else if (En > std::pow(2.f, 32) / 30) {
      // large E optimize
      return std::pow(-2.f, 32) * std::log(1.f - En / std::pow(2.f, 32));
    }
    return En;
  }
  [[nodiscard]] inline float Z() const noexcept {
    auto a = std::accumulate(
        M_.cbegin(), M_.cend(), 0.0,
        [](const double &a, double b) { return a + std::pow(2, -b); });
    return 1.0 / a;
  }
  inline void log(const T_ &v) noexcept {
    uint32_t h;
    MurmurHash3_x86_32(&v, sizeof(v), 0xbeef, &h);
    auto s = sizeof(h) * 8 - B_;
    auto b = h << B_ == 0 ? s : __builtin_clz(h << B_) + 1;
    b = b > s ? s : b;
    auto &a = M_[h >> s];
    if (b > a) {
      a = b;
    }
  }
  [[nodiscard]] auto buckets() const noexcept -> const auto & { return M_; }

  void merge(const HyperLogLog<T_, B_> &with) noexcept {
    auto wb = with.buckets();

    std::transform(M_.begin(), M_.end(), with.buckets().cbegin(), M_.begin(),
                   [](auto a, auto b) { return std::max(a, b); });
  }
};
} // GotStd