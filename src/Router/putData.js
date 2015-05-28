var redis = require('redis'),
    RDS_PORT = 6379,            //Port
    RDS_HOST = '192.168.0.30',     //IP
    RDS_OPTS = { 'return_buffers': true },              //Setting
    client = redis.createClient(RDS_PORT, RDS_HOST, RDS_OPTS);

var fs = require('fs');

//15s
//var arPath = [1165885, 127052, 1419205, 176031, 2061491, 215976, 2372196, 252582, 2863389, 315047, 3222578, 3464259, 365887, 3748236, 45351, 500530, 565835, 769540, 88563, 982168];
//1s
//var arPath = [1071529, 1312787, 135410, 1662809, 182366, 2234145, 226106, 2617284, 270316, 3305118, 352546, 3841983, 4242923, 424520, 46980, 4726737, 537825, 620705, 808057, 91917];
//6s
var arPath = [1006743, 1201426, 128256, 1461530, 176827, 2078678, 217651, 2395950, 255169, 2919314, 320564, 3297198, 3557500, 373935, 3858484, 45514, 506300, 573252, 779796, 89036];

client.on('ready', function (res) {
    console.log('ready');

    for (var i = 0; i < arPath.length; i++) {

        for (var c = 1; c <= 100; c++) {
            var sPath = "/data/dash/6/" + arPath[i] + "/";
            sPath += "BigBuckBunny_6s" + c + ".m4s";

            (function (key) {
                fs.readFile(sPath, function (err, data) {
                    if (err) {
                        console.log("Find Error");
                        console.error(err);
                        return;
                    }

                    console.log("Finish LOAD Begin to Put [ " + key + " ] = sizeof ( " + data.length + " )");

                    client.set(key, data, function (err, reply) {
                        console.log(reply.toString());
                    });


                });
            })(arPath[i] + "_" + c);

        }

    }



    /*

    var szData = new Buffer(10000000);
    client.set("1", szData, function (err, reply) {
    console.log(reply.toString());
    });

    */


});


