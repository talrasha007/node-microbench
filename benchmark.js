var test = require('./'),
    cnt = 1000 * 1000 * 1;

function benchmark(desc, fn) {
    var start = new Date();
    fn();
    console.log('%s: %d(ms)', desc, new Date() - start);
}

function testCallback_js() {
    for (var i = 0; i < cnt; i++) {
        (function (cb) { cb(); })(function () {});
    }
}

function testCallback_cpp() {
    for (var i = 0; i < cnt; i++) {
        test.testCallback(function () {});
    }
}

function testFoo() {
    for (var i = 0; i < cnt; i++) {
        test.testCall();
    }
}

function testRetString() {
    for (var i = 0; i < cnt; i++) {
        test.testRetString();
    }
}

function testRetObj_js() {
    for (var i = 0; i < cnt; i++) {
        (function () {
            var obj = new Object();
            obj.a = '1234567890';
            obj.b = '234567890';
            obj.c = '34567890';
        })();
    }
}

function testRetObj_cpp() {
    for (var i = 0; i < cnt; i++) {
        test.testRetObj();
    }
}

function testBuffer_js() {
    for (var i = 0; i < cnt; i++) {
        new Buffer('abc');
    }
}

function testBuffer() {
    for (var i = 0; i < cnt; i++) {
        test.testBuffer();
    }
}

function testBufferSmalloc() {
    for (var i = 0; i < cnt; i++) {
        test.testBufferSmalloc();
    }
}

function testWrapNew_js() {
    for (var i = 0; i < cnt; i++) {
        new test.TestWrap();
    }
}

function testWrapNew_cpp() {
    for (var i = 0; i < cnt; i++) {
        new test.testWrapNew();
    }
}

function testUnwrp() {
    var tw = new test.TestWrap();
    for (var i = 0; i < cnt; i++) {
        tw.unwrap();
    }
}

function testWrapFoo() {
    var tw = new test.TestWrap();
    for (var i = 0; i < cnt; i++) {
        tw.foo();
    }
}

console.log("run %d times:", cnt);
benchmark('testFoo', testFoo);
benchmark('testRetString', testRetString);
benchmark('testRetObj_js', testRetObj_js);
benchmark('testRetObj_cpp', testRetObj_cpp);
benchmark('testCallback_js', testCallback_js);
benchmark('testCallback_cpp', testCallback_cpp);
benchmark('testBuffer_js', testBuffer_js);
benchmark('testBuffer', testBuffer);
benchmark('testBufferSmalloc', testBufferSmalloc);
benchmark('testWrapNew_js', testWrapNew_js);
benchmark('testWrapNew_cpp', testWrapNew_cpp);
benchmark('testUnwrp', testUnwrp);
benchmark('testWrapFoo', testWrapFoo);

