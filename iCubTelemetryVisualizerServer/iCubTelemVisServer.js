// Server http (non secure)

// require and setup basic http functionalities
var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);

// require yarp.js and setup the communication
//with the browser using websockets
var yarp = require('../yarp');
yarp.browserCommunicator(io);

// setup static folders
app.use(express.static(__dirname + '/../node_modules'));
app.use(express.static(__dirname + '/../js'));
app.use(express.static(__dirname));

// setup default page
app.get('/', function(req, res){
  res.sendFile('index.html',{ root : __dirname});
});

// Basic implementation of a history and realtime server.
var ICubTelemetry = require('./icubtelemetry');
var RealtimeServer = require('./realtime-server');
var HistoryServer = require('./history-server');

// Setup 'express-ws' in order to add WebSocket routes
var expressWs = require('express-ws');
expressWs(app);

// Create the servers
var icubtelemetry = new ICubTelemetry();
var realtimeServer = new RealtimeServer(icubtelemetry);
var historyServer = new HistoryServer(icubtelemetry);
app.use('/realtime', realtimeServer);
app.use('/history', historyServer);

var port = process.env.PORT || 8080

// Start history and realtime servers
app.listen(port, function () {
    console.log('ICubTelemetry History hosted at http://localhost:' + port + '/history');
    console.log('ICubTelemetry Realtime hosted at ws://localhost:' + port + '/realtime');
});

// start the server!
http.listen(3000, function(){
  console.log('listening on *:3000');
});
