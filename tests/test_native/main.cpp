#include <catch.hpp>
#include <graalvm_test_native.h>
#include <dxf_types.h>
#include <thread>

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
  graal_isolate_t *isolate = nullptr;
  graal_isolatethread_t *thread = nullptr;
  graal_create_isolate(nullptr, &isolate, nullptr);
  bool t1Result = true;
  std::thread t1([isolate, &t1Result]() {
    graal_isolatethread_t *thread = nullptr;
    graal_attach_thread(isolate, &thread);
    for (int i = 0; i < 100000 && t1Result; ++i) {
      if (t1Result) {
        t1Result &= native_test_api_add(thread, 2, 2) == 4;
        t1Result &= native_test_api_add(thread, 2, -2) == 0;
      }
    }
    graal_detach_thread(thread);
  });
  bool t2Result = true;
  std::thread t2([isolate, &t2Result]() {
    graal_isolatethread_t *thread = nullptr;
    graal_attach_thread(isolate, &thread);
    for (int i = 0; i < 100000 && t2Result; ++i) {
      if (t2Result) {
        t2Result &= native_test_api_add(thread, 2, 2) == 4;
        t2Result &= native_test_api_add(thread, 2, -2) == 0;
      }
    }
    graal_detach_thread(thread);
  });
  t1.join();
  t2.join();
  // REQUIRE macros are not thread safe.
  REQUIRE(t1Result == true);
  REQUIRE(t2Result == true);
}

TEST_CASE("Check pass Java System property", "[TestNative]") {
  graal_isolate_t *isolate;
  graal_isolatethread_t *graalvmThread;
  graal_isolatethread_t *otherGraalvmThread;
  graal_create_isolate(nullptr, &isolate, &graalvmThread);
  graal_create_isolate(nullptr, &isolate, &otherGraalvmThread);

  // Requires conversion to char* because the native-image produces a very bad header file.
  native_test_set_system_property(graalvmThread, const_cast<char *>("propA"), const_cast<char *>("5"));
  native_test_set_system_property(otherGraalvmThread, const_cast<char *>("propA"), const_cast<char *>("test"));
  REQUIRE(native_test_check_system_property(graalvmThread, const_cast<char *>("propA"), const_cast<char *>("5")));
  REQUIRE(native_test_check_system_property(otherGraalvmThread,
                                            const_cast<char *>("propA"),
                                            const_cast<char *>("test")));
  REQUIRE(native_test_check_system_property(graalvmThread, const_cast<char *>("propA"), const_cast<char *>("5")));
  REQUIRE(native_test_check_system_property(otherGraalvmThread,
                                            const_cast<char *>("propA"),
                                            const_cast<char *>("test")));
}

TEST_CASE("Check event listener", "[TestNative]") {
  auto graal = native_test_api_create_isolate();
  auto listener = [](dxf_event_t **events, size_t size) {
    REQUIRE(size == 2);
    REQUIRE(events[0]->event_type == DXF_EVENT_TYPE_QUOTE);
    REQUIRE(events[1]->event_type == DXF_EVENT_TYPE_TIME_AND_SALE);
    for (int i = 0; i < size; ++i) {
      if (events[i]->event_type == 0) {
        auto quote = (dxf_event_quote_t *) events[i];
        REQUIRE(memcmp(quote->event.symbol_name, "😁", strlen(quote->event.symbol_name)) == 0);
        REQUIRE(quote->ask_price == 1);
        REQUIRE(quote->bid_price == 2);
      } else {
        auto tns = (dxf_event_time_and_sale_t *) events[i];
        REQUIRE(memcmp(tns->event.symbol_name, "ETH/USD:GDAX", strlen(tns->event.symbol_name)) == 0);
        REQUIRE(tns->event_flag == 3);
        REQUIRE(tns->index == 4);
      }
    }
  };
  native_create_events_and_call_listener(graal, listener);
}
}
}