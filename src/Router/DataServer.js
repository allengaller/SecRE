var express = require('express');
var app = express();
var redis = require('redis'),
    RDS_PORT = 6379,            //Port
    RDS_HOST = '192.168.0.30',     //IP
    RDS_OPTS = { 'return_buffers': true },              //Setting
    client = redis.createClient(RDS_PORT, RDS_HOST, RDS_OPTS);


client.on('ready', function (res) {
    console.log('ready');

    app.get('/get/:skey', function (req, res) {
        //console.log("Get Key : " + req.params.skey);

        client.get(req.params.skey, function (err, reply) {

            if (err) {
                console.error(err);
            }
            //console.log("Get OK");
            //console.log(reply.length);
            res.write(reply, "binary");
            res.end();

        });
    });


    app.get('/put/:skey/:size', function (req, res) {
        console.log("Put Key : " + req.params.skey);
        var skey = req.params.skey;
        var size = req.params.size;
        console.log("skey = " + skey);
        console.log("size = " + size);
        var szData = new Buffer(size - 0);
        client.set(skey, szData, function (err, reply) {
            console.log(reply.toString());
            res.send(reply.toString());
        });
    });

    var server = app.listen(80, function () {

        var host = "192.168.0.30";
        var port = server.address().port;

        console.log('Example app listening at http://%s:%s', host, port);

    });



});



