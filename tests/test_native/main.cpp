#include <catch.hpp>
#include <GraalvmTestNative.h>

namespace dxfeed {
namespace test {
TEST_CASE("Simple check run_main", "[TestNative]") {
  // "run_main" is a renamed Java "main" in native-image.
  int res = run_main(0, nullptr);
  REQUIRE(res == 0);
}

TEST_CASE("Check create IsolateThread from Java", "[TestNative]") {
  auto graalvmThread = native_test_api_create_isolate();
  REQUIRE(graalvmThread != 0);
}

TEST_CASE("Check @CEntryPoint functions", "[TestNative]") {
  auto graalvmThread = native_test_api_create_isolate();
  REQUIRE(native_test_api_add(graalvmThread, 2, 2) == 4);
  REQUIRE(native_test_api_add(graalvmThread, 2, -2) == 0);
}

TEST_CASE("Check pass Java System property", "[TestNative]") {
  auto graalvmThread = native_test_api_create_isolate();
  auto otherGraalvmThread = native_test_api_create_isolate();
  // Requires conversion to char* because the native-image produces a very bad header file.
  native_test_set_system_property(graalvmThread, const_cast<char*>("propA"), const_cast<char*>("5"));
  native_test_set_system_property(otherGraalvmThread, const_cast<char*>("propA"), const_cast<char*>("test"));
  REQUIRE(native_test_check_system_property(graalvmThread, const_cast<char*>("propA"), const_cast<char*>("5")));
  REQUIRE(native_test_check_system_property(otherGraalvmThread, const_cast<char*>("propA"), const_cast<char*>("test")));
  REQUIRE(native_test_check_system_property(graalvmThread, const_cast<char*>("propA"), const_cast<char*>("5")));
  REQUIRE(native_test_check_system_property(otherGraalvmThread, const_cast<char*>("propA"), const_cast<char*>("test")));
}
}
}