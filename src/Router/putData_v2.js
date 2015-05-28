var redis = require('redis'),
    RDS_PORT = 6379,            //Port
    RDS_HOST = '192.168.0.30',     //IP
    RDS_OPTS = { 'return_buffers': true },              //Setting
    client = redis.createClient(RDS_PORT, RDS_HOST, RDS_OPTS);

var program = require('commander');
var fs = require('fs');
program
    .version('0.0.1')
    .usage('[options] [value ...]')
    .option('-c, --count <n>', 'input a integet argument.', parseInt)
    .option('-k, --kind <n>', 'input a integet argument.', parseInt);

program.parse(process.argv);

var arData = [200133, 500131, 888185, 1165743, 1497601];
//Size : 131350832 328172096 582781328 764891648 982642416

if (program.count <= 0 || program.kind < 0 || program.kind > 4) {
    console.log("Wrong Params");
    process.exit();
}

console.log("Count : " + program.count);
console.log("Kind : " + program.kind);

client.on('ready', function (res) {
    console.log('ready');

    for (var c = 1; c <= program.count; c++) {
        var sPath = "/data/dash/plaintext/" + arData[program.kind] + "/";
        sPath += "RedBull_15" + c + ".m4s";

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
        })(c);

    }

});


