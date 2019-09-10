#include "HyperLogLog.hpp"

auto main() -> int {

  auto hll = GotStd::HyperLogLog<uint8_t > {};

  hll.log(10);
  hll.log(8);

  return static_cast<int8_t >(hll.Estar());

}