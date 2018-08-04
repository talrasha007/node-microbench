#include <emscripten.h>

extern "C" {
  EMSCRIPTEN_KEEPALIVE void testCall() { }
  EMSCRIPTEN_KEEPALIVE char *createPtr(size_t sz) { return new char[sz]; }
  EMSCRIPTEN_KEEPALIVE void deletePtr(char *ptr) { delete [] ptr; }
  EMSCRIPTEN_KEEPALIVE void setPtr(char *ptr, size_t offset, char v) { ptr[offset] = v; }
  EMSCRIPTEN_KEEPALIVE char getPtr(char *ptr, size_t offset) { return ptr[offset]; }
}