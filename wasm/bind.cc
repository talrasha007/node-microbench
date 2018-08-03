#include <string>

#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace std;
using namespace emscripten;

void testCall() { }
val testRetString() { return val(string("1234567890")); }

EMSCRIPTEN_BINDINGS (c) {
  emscripten::function("testCall", &testCall);
  emscripten::function("testRetString", &testRetString);
}