var binding = require('../build/Release/node-microbench.node');

suite('empty function call', function () {
    function jsfn() {}

    bench('js', function () { jsfn(); });
    bench('c++', function () { binding.testCall(); });
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
    var ptr = binding.createIntPtr();

    bench('ctor:js', function () { new binding.TestWrap(); });
    bench('ctor:c++', function () { binding.testWrapNew(); });
    bench('function call', function () { tw.foo(); });
    bench('unwrap', function () { tw.unwrap(); });
    bench('create IntPtr', function () { binding.createIntPtr(); });
    bench('unwrap IntPtr', function () { binding.unwrapIntPtr(ptr); });
    bench('val of IntPtr', function () { binding.valOfPtr(ptr); });
});

suite('callback', function () {
    function jsfn(cb) { return cb(); }

    bench('js', function () { jsfn(function () {}); });
    bench('c++', function () { binding.testCallback(function () {}); });
});

suite('buffer', function () {
    function jsfn() { return new Buffer(0); }

    var buf = new Buffer(1024), i = 0;

    bench('js', function () { jsfn(function () {}); });
    bench('c++', function () { binding.testBuffer(); });
    bench('c++:smalloc', function () { binding.testBufferSmalloc(); });
    bench('buffer [] access', function () { return buf[i++ % buf.length]; });
    bench('buffer readI32BE', function () { return buf.readUInt32BE(0); });
    bench('buffer writeI32BE', function () { return buf.writeUInt32BE(0, 0); });
});

