#include "HyperLogLog.hpp"

auto main() -> int {

  auto hll = GotStd::HyperLogLog<uint8_t > {};

  hll.log(10);
  hll.log(8);

  auto hll2 = GotStd::HyperLogLog<uint8_t > {};

  hll2.log(17);
  hll2.log(108);

  hll.merge(hll2);

  return static_cast<int8_t >(hll.Estar());

}