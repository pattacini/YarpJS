/*
 Spacecraft.js simulates a small spacecraft generating telemetry.
*/

function ICubTelemetry() {
    this.state = {
        "sens.imu": [0,0,0,0,0,0,0,0,0,0,0,0]
    };
    this.history = {};
    this.listeners = [];
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    setInterval(function () {
/*        this.updateState();*/
        this.generateTelemetry();
    }.bind(this), 1000);

    console.log("iCub Telemetry server launched!");
};

Spacecraft.prototype.updateState = function () {
    this.state["sens.imu"] = [1,1,1,1,1,1,1,1,1,1,1,1];
};

/**
 * Takes a measurement of spacecraft state, stores in history, and notifies
 * listeners.
 */
ICubTelemetry.prototype.generateTelemetry = function () {
    var timestamp = Date.now(), sent = 0;
    Object.keys(this.state).forEach(function (id) {
        var state = { timestamp: timestamp, value: this.state[id], id: id};
        this.notify(state);
        this.history[id].push(state);
        this.state["comms.sent"] += JSON.stringify(state).length;
    }, this);
};

ICubTelemetry.prototype.notify = function (point) {
    this.listeners.forEach(function (l) {
        l(point);
    });
};

ICubTelemetry.prototype.listen = function (listener) {
    this.listeners.push(listener);
    return function () {
        this.listeners = this.listeners.filter(function (l) {
            return l !== listener;
        });
    }.bind(this);
};

module.exports = function () {
    return new ICubTelemetry()
};
