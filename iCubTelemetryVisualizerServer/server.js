/**
 * Basic implementation of a history and realtime server.
 */

var ICubTelemetry = require('./icubtelemetry');
var RealtimeServer = require('./realtime-server');
var HistoryServer = require('./history-server');

var expressWs = require('express-ws');
var app = require('express')();
expressWs(app);

var icubtelemetry = new ICubTelemetry();
var realtimeServer = new RealtimeServer(icubtelemetry);
var historyServer = new HistoryServer(icubtelemetry);

app.use('/realtime', realtimeServer);
app.use('/history', historyServer);

var port = process.env.PORT || 8080

app.listen(port, function () {
    console.log('ICubTelemetry History hosted at http://localhost:' + port + '/history');
    console.log('ICubTelemetry Realtime hosted at ws://localhost:' + port + '/realtime');
});
