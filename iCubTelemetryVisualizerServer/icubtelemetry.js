/*
 icubtelemetry.js simulates a small spacecraft generating telemetry.
*/

function ICubTelemetry() {
    this.state = {
        "sens.imu": 0
    };
    this.history = {};
    this.listeners = [];
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    const ONE_SECOND_REPEAT_INTERVAL = 1000;
    setInterval(function () {
        this.updateState();
        this.generateTelemetry();
    }.bind(this), ONE_SECOND_REPEAT_INTERVAL);

    console.log("iCub Telemetry server launched!");
};

ICubTelemetry.prototype.updateState = function () {
    this.state["sens.imu"] = 10;
};

/**
 * Takes a measurement of spacecraft state, stores in history, and notifies
 * listeners.
 */
ICubTelemetry.prototype.generateTelemetry = function () {
    var timestamp = Date.now();
    Object.keys(this.state).forEach(function (id) {
        var state = { timestamp: timestamp, value: this.state[id], id: id};
        this.notify(state);
        this.history[id].push(state);
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
