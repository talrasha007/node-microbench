var http = require('http'),
    net = require('net'),
    dgram = require('dgram');

suite('tcp', function () {
    var server;
    before(function (cb) {
        server = http.createServer(function (req, res) {
            res.writeHead(200, {'Content-Type': 'text/plain'});
            res.end('okay');
        });

        server.listen(29998, cb);
    });

    bench('request', function (next) {
        http.get('http://127.0.0.1:29998/', function () {
            next();
        });
    });

    after(function () {
        server.close();
    });
});

suite('tcp', function () {
    var server, client;

    before(function (cb) {
        server = net.createServer(function(c) {
            c.pipe(c);
        });

        server.listen(39998, function () {
            client = net.connect({ port: 39998 }, function () { cb(); });
        });
    });

    bench('request', function (next) {
        var c = net.connect({ port: 39998 }, function () { c.end('foo'); });
        c.on('data', function () { });
        c.on('end', next);
    });

    bench('echo', function (next) {
        client.once('data', next);
        client.write('okay');
    });

    after(function () {
        client.end();
        server.close();
    });
});

suite('udp', function () {
    var server, client;

    before(function (cb) {
        server = dgram.createSocket('udp4');
        client = dgram.createSocket('udp4');
        server.on("message", function (message, rinfo) {
            server.send(message, 0, message.length, rinfo.port, rinfo.address);
        });
        server.on('listening', cb);

        server.bind(49998);
    });

    var hello = new Buffer('hello');
    bench('echo', function (next) {
        client.once('message', next);
        client.send(hello, 0, hello.length, 49998, '127.0.0.1');
    });

    after(function () {
        server.close();
    });
});
