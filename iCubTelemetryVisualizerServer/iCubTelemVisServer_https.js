// examples_https.js

// require yarp.js
var yarp = require('../yarp');

// setup static folders
var express = require('express');
var app = express();
app.use(express.static('node_modules'));
app.use(express.static('js'));

// setup default page
app.get('/', function(req, res){
  res.sendFile('index.html',{ root : __dirname});
});
app.get('/yarp.js', function(req, res){
  res.sendfile('/yarp.js',{ root : __dirname});
});

// provided a self-signed certificate to run local
// applications from an HTTPS domain
var fs = require('fs');
var options = {
    key: fs.readFileSync('./iCubTelemetryVisualizerServer/https/conf/server.key'),
    cert: fs.readFileSync('./iCubTelemetryVisualizerServer/https/conf/server.crt'),
    requestCert: false,
    rejectUnauthorized: false
};

// require and setup basic http functionalities
var https = require('https');

// start the server!
var server = https.createServer(options, app).listen(3000, function(){
    console.log('listening on *:3000');
});

// setup the communication with the browser using websockets
var io = require('socket.io')(server);
yarp.browserCommunicator(io);
