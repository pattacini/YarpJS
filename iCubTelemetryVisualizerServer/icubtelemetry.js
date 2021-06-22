/*
 icubtelemetry.js simulates a small spacecraft generating telemetry.
*/

function ICubTelemetry() {
    this.state = {
        "sens.imu": 0
    };
    this.maxDepthSamples = 1000;
    this.history = {};
    this.listeners = [];
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    const TEN_MS_REPEAT_INTERVAL = 10;
    setInterval(function () {
        this.generateTelemetry();
    }.bind(this), TEN_MS_REPEAT_INTERVAL);

    console.log("iCub Telemetry server launched!");
};

ICubTelemetry.prototype.updateState = function (sensorSample) {
    this.state["sens.imu"] = sensorSample;
};

/**
 * Takes a measurement of spacecraft state, stores in history, and notifies
 * listeners.
 */
ICubTelemetry.prototype.generateTelemetry = function () {
    var timestamp = Date.now();
    Object.keys(this.state).forEach(function (id) {
        var telemetrySample = { timestamp: timestamp, value: this.state[id], id: id};
        this.notify(telemetrySample);
        this.history[id].push(telemetrySample);
        if (this.history[id].length > this.maxDepthSamples) {
          this.history[id].shift();
        }
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
