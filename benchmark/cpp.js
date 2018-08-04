var binding = require('../build/Release/node-microbench.node');
var functions = require('../wasm/functions');

suite('empty function call', function () {
    function jsfn() {}

    bench('js', function () { jsfn(); });
    bench('c++', function () { binding.testCall(); });
    bench('wasm', function () { functions._testCall(); });
    bench('wasm.asm', function () { functions.asm._testCall(); });
});

suite('async', function () {
    bench('nextTick', function (next) { process.nextTick(next); });
    bench('setTimeout_0', function (next) { setTimeout(next, 0); });
    if (global.setImmediate) bench('setImmediate', function (next) { setImmediate(next); });
    bench('c++ async', function (next) { binding.testAsync(next); });

    bench('c++ async parallelx3', function (next) {
        var cnt = 0;
        function cb() { if (++cnt === 3) next(); };
        binding.testAsync(cb);
        binding.testAsync(cb);
        binding.testAsync(cb);
    });

    bench('c++ async parallelx4', function (next) {
        var cnt = 0;
        function cb() { if (++cnt === 4) next(); };
        binding.testAsync(cb);
        binding.testAsync(cb);
        binding.testAsync(cb);
        binding.testAsync(cb);
    });
});

suite('string ret function call', function () {
    function jsfn() { return 'aaa'; }

    bench('js', function () { jsfn(); });
    bench('c++', function () { binding.testRetString(); });
});

suite('array ret function call', function () {
    function jsfn() { var arr = []; for (var i = 0; i < 16; i++) arr[i] = i; return arr; }

    bench('js', function () { jsfn(); });
    bench('c++', function () { binding.testRetArray(); });
});

suite('get array items', function () {
    var arr = [1, 2, 3, 4, 5, 6];
    function jsfn(arr) { var sum = 0; for (var i = 0; i < arr.length; i++) sum += arr[i]; return sum; }

    bench('js', function () { jsfn(arr); });
    bench('c++ args', function () { binding.testArgs.apply(null, arr); });
    bench('c++', function () { binding.testArrayGet(arr); });
});

suite('get object properties', function () {
    var obj = { a: 1, b: 2, c: 3 };
    function jsfn(o) { return o.a + o.b + o.c; }

    bench('js', function () { jsfn(obj); });
    bench('c++', function () { binding.testGet(obj); });
});

suite('set object properties & return', function () {
    function jsfn() {
        var obj = new Object();
        obj.a = '1234567890';
        obj.b = '234567890';
        obj.c = '34567890';
        return obj;
    }

    bench('js', function () { jsfn(); });
    bench('c++ execute js', function () { binding.testRun(); });
    bench('c++', function () { binding.testRetObj(); });
});

suite('object wrap', function () {
    var tw = new binding.TestWrap();
    //var ptr = binding.createIntPtr();

    bench('ctor:js', function () { new binding.TestWrap(); });
    bench('ctor:c++', function () { binding.testWrapNew(); });
    bench('function call', function () { tw.foo(); });
    bench('unwrap', function () { tw.unwrap(); });
    //bench('create IntPtr', function () { binding.createIntPtr(); });
    //bench('unwrap IntPtr', function () { binding.unwrapIntPtr(ptr); });
    //bench('val of IntPtr', function () { binding.valOfPtr(ptr); });
});

suite('class wrap', function () {
    var cw = new binding.TestClassWrap();

    bench('ctor:js', function () { new binding.TestClassWrap(); });
    bench('ctor:c++', function () { binding.testClassWrapNew(); });
    bench('function call', function () { cw.foo(); });
    bench('unwrap', function () { cw.unwrap(); });
});

suite('callback', function () {
    function jsfn(cb) { return cb(); }

    bench('js', function () { jsfn(function () {}); });
    bench('c++', function () { binding.testCallback(function () {}); });
});

suite('buffer', function () {
    function jsfn(sz) { return new Buffer(sz); }

    var buf = new Buffer(1024), i = 0;

    bench('js size 0', function () { jsfn(0); });
    bench('c++ size 0', function () { binding.testBuffer(0); });
    bench('js size 8', function () { jsfn(8); });
    bench('c++ size 8', function () { binding.testBuffer(8); });

    bench('c++:smalloc', function () { binding.testBufferSmalloc(); });
    bench('buffer [] access', function () { return buf[i++ % buf.length]; });
    bench('buffer readI32BE', function () { return buf.readUInt32BE(0); });
    bench('buffer writeI32BE', function () { return buf.writeUInt32BE(0, 0); });
});

suite('wasm ptr', function () {
    let ptr;
    before(function (next) {
        ptr = functions._createPtr(32);
        next();
    });

    after(function (next) {
        functions._deletePtr(ptr);
        next();
    });

    bench('create/delete', function () {
      const pp = functions._createPtr(32);
      functions._deletePtr(pp);
    });
    bench('set', function () { functions._setPtr(ptr, 0, 1); });
    bench('get', function () { functions._getPtr(ptr, 0); });
});
