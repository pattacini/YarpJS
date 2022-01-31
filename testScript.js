var yarp = require('./yarp')
var port = yarp.portHandler.open('/yarpjs:i')
port.onRead(function (bottle){console.log(bottle.toArray())})
yarp.Network.connect('/icubSim/torso/stateExt:o','/yarpjs:i')

