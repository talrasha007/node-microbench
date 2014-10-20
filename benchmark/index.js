var binding = require('../build/Release/node-microbench.node');

suite('empty function call', function () {
    function jsfn() {}

    bench('js', function () { jsfn(); });
    bench('c++', function () { binding.testCall(); });
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
    bench('c++', function () { binding.testRetObj(); });
});

suite('object wrap', function () {
    var tw = new binding.TestWrap();

    bench('ctor:js', function () { new binding.TestWrap(); });
    bench('ctor:c++', function () { binding.testWrapNew(); });
    bench('function call', function () { tw.foo(); });
    bench('unwrap', function () { tw.unwrap(); });
});

suite('callback', function () {
    function jsfn(cb) { return cb(); }

    bench('js', function () { jsfn(function () {}); });
    bench('c++', function () { binding.testCallback(function () {}); });
});

suite('buffer', function () {
    function jsfn() { return new Buffer(0); }

    bench('js', function () { jsfn(function () {}); });
    bench('c++', function () { binding.testBuffer(); });
    bench('c++:smalloc', function () { binding.testBufferSmalloc(); });
});

