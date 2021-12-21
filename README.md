

# yarp.js

[![DOI](https://zenodo.org/badge/53891456.svg)](https://zenodo.org/badge/latestdoi/53891456)


A JavaScript set of bindings for YARP.

* [Introduction](#introduction)
* [Installation](#installation)
* [Examples](#examples)
* [Server](#server)
    - [Port Communication](#port-communication)
        - [Reading](#port-reading)
        - [Writing](#port-writing)
        - [RPC](#port-rpc)
* [Client (Browser)](#browser)
    - [Server Setup](#browser-server-setup)
    - [Browser Setup](#browser-client-setup)
    - [Port Communication](#browser-port-communication)
        - [Open/Close](#browser-open-close)
        - [Read/Write](#browser-read-write)
        - [Handling Connections](#browser-handling-connections)


<a name='introduction'></a>
## Introduction

This library has been developed with the idea of bringing YARP to any device without heavy code dependencies (It just needs an Internet Browser!). The idea is to have a server in charge of directly interacting with the YARP C++ layer (and the rest of the YARP network) while clients are completely implementation and hardware independent. Indeed, code is shipped by the server to all clients via [websockets](https://en.wikipedia.org/wiki/WebSocket?oldformat=true) (in particular the [Socket.io](http://socket.io/) Node module), **without the need to install anything on the client machine** (a part the Browser of course).

<p align='center'>
<img src="https://github.com/robotology/yarp.js/blob/master/images/yarp.js_Network.png" width="60%">
</p>

The library is based on node.js which can be natively integrated with external C++ code via [Node addons](https://nodejs.org/api/addons.html).

**Tested OS:** yarp.js has been tested on **OSX** and **Ubuntu**. This goes for the server side. The client side goes on any machine with a version of Google Chrome or Firefox installed.


<a name='installation'></a>
## Installation

**Client dependencies (on any device on your network)**: 
* [Google Chrome](https://www.google.com/chrome/)
* [Firefox](https://www.mozilla.org/en-US/firefox/products/)

**Server dependencies**: 
* [YARP](https://github.com/robotology/yarp) (Duh!). With OpenCV!
* [node.js](https://nodejs.org/en/). Version = 14.17.0. Follow the [official guide](https://nodejs.org/en/download/package-manager/).
* Dependencies automatially handled by **Node.js** package manager (**npm)**:
    - [cmake]() ≥ 6.3.0
    - [express]()
    - [jquery]()
    - [nan]()
    - [socket.io]() ≥ 4.4.0
    - [socket.io-stream]()
    - [three]() ≥ 0.125.0

**Note**: 
- **Please make sure** that your Node version is = 14.17.0. This repository hasn't been tested with newer Node versions. It is easier to handle Node.js versions through the Node version manager (NVM). Refer to [Reinstall npm with a node version manager](https://docs.npmjs.com/resolving-eacces-permissions-errors-when-installing-packages-globally#reinstall-npm-with-a-node-version-manager) for further details.
- **Linux**. If you installed Node.js from the package manager, it could happen that the command `node` is not in your path, but rather `nodejs` is. To this end, run `$> sudo ln -s /usr/bin/nodejs /usr/bin/node`.


Once you have all dependencies installed, go to the folder where you have cloned this repository and run:
```
$> sudo npm install
```
The npm install script defined in `./package.json` runs `./node_modules/cmake-js/bin/cmake-js` on its own.

**Optional**: although the command `sudo npm install` installs the node dependencies *locally*, it needs administrative permissions. If you want to avoid this (and just use `npm install`), follow the [official npm guide](https://docs.npmjs.com/getting-started/fixing-npm-permissions). This is not required to use yarp.js though.


<a name='examples'></a>
## Examples

<p align='center'>
<img style='border:1px solid green; box-shadow: 0 0 10px rgba(0,0,0, .65);' src="https://github.com/robotology/yarp.js/blob/master/images/example_browser_landing.png" width="60%">
</p>

We have prepared a series of tutorials and examples on how to use yarp.js. To get things started, go [here](/examples#tutorials) to find the instruction to write and run a minimalistic node server that uses yarp.js. A website will be locally served on your network, showcasing the following demo applications:

* [Speech Recognition and Synthesis](/examples/speech_recognition)
* [Visualization with WebGL - Point Cloud](/examples/point_cloud) 
* [Transmitting Inertial Data](/examples/inertial_data)
* [Stream Audio](/examples/stream_audio)
* [Stream Video (a yarpview with yarp.js)](/examples/stream_video) 
* [Secure Domains (HTTPS)](/examples#secure-domains)


<a name='server'></a>
## Server

In this section we descirbe how to use yarp.js on the server side.

<a name='port-communication'></a>
### Port Communication

Ports are a key component to YARP. In this section we are going to see how yarp.js allows us to use YARP ports from JavaScript (in a Node environment).

Throughout we will assume that a **yarp server** is running on our network (if it's not your case, just run `$> yarp server` in a shell). You can find the example described in this section in the script `examples/port_basics.js`.

From the folder where you have cloned this repository run `$> node` to enter in the interactive Node.js interface. Then run:
```js
var yarp = require('./yarp');
```
The first command loads the yarp.js module that will allow us to use YARP from JS. The second command is necessary to register the current session on the YARP network. We can finally create and open a port:
```js
var port = new yarp.Port('bottle');
port.open('/yarpjs/example');
```
You should receive a message from YARP saying something like: `yarp: Port /yarpjs/example active at tcp://some.ip:address` At the time being yarp.js can open buffered ports for YARP `bottle`s and `image`s.

<a name='port-writing'></a>
#### Writing on a Port

Let's write something from our port!
```js
var bottle = new yarp.Bottle();
bottle.fromString('This is a Bottle');

port.write(bottle);
```
So, if someone is listening to our port (e.g. run in a different shell `$> yarp read ... /yarpjs/example`), she will receive a YARP Bottle containing the string `'This is a Bottle'`. **Note**: you can create more complex YARP Bottles with the standard YARP sintax (e.g. `bottle.fromString('(This is) (a Bottle)')` creates a bottle with two bottles inside, containing respectively the strings `'This is'` and `'a Bottle'`.



<a name='port-reading'></a>
#### Reading from a Port

Let's set up the callback for when a message is sent from another port to `/yarpjs/example`:
```js
port.onRead(function(msg){
    console.log('Message received: ' + msg.toString());
});
```
This way, whenever a message arrives, it is printed on screen. You can try it out by going on another bash and type:
```
$> yarp write ... /yarpjs/example
>> Hello yarp.js!
```
On the shell where you run yarp.js you should be able to see the message: `Message received: Hello "yarp.js!"`

**Note**: The `onRead` method of a yarp port takes in input a callback. Everytime the port reads something on the network the callback is called with that objects as message `msg`.

<a name='port-rpc'></a>
#### RPC Communication

You can also set the RPC behavior for your port. For instance
```js
port.onRPC(function(msg){
    console.log(msg.toString());

    var bottle = new yarp.Bottle();
    bottle.fromString('Reply (' + msg.toString() + ')');

    port.reply(bottle);
});
```
You can try this behavior by running in another shell `$> yarp rpc /yarpjs/example` and then typing a message. You should receive a reply `Reply (<your message>)`.**Note**: the `onRPC` callback behave exactly like the `onRead`. Howevere it assumes that the method `reply` will be eventuallycalled. As a good practice we suggest to always put the `reply` command inside the `onRPC` callback. 


<a name='browser'></a>
## Client (Browser)

This section describes how a yarp.js server communicates with the browser. **This section is in alpha and it is likely to be subject to changes**

**Note** yarp.js uses the websocket library [Socket.io](http://socket.io/) to exchange messages between server and the browser. However, the node.js wrapper and the browser communication are completely decoupled, so you can implement your own communication protocol.

<a name='browser-server-setup'></a>
### Setup for the Server


To setup the server side create a js file for your server, e.g. `server.js`. The minimal code to run the server is 

```js
// Basic required node plugins to start the http server and socket.io
var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);

// yarp.js
var yarp = require('../yarp');  // <-- the path to yarp.js in the folder where you cloned this repo.
yarp.browserCommunicator(io);   // <-- The service handling communication with the browser.

// The Express.js routing to your index file (change it to your desired homepage, e.g. index.html)
app.get('/', function(req, res){
  res.sendFile('index.html',{ root : __dirname});
});

/*
    Your code here
*/

// Run the server on locahlhost:3000
http.listen(3000, function(){
  console.log('listening on *:3000');
});
```

This is enough to have a yarp.js server up and running and ready to communicate with the browser.

Note that we use [Express.js](http://expressjs.com/) since it is used in Socket.io.


<a name='browser-client-setup'></a>
### Setup for the Browser

To use yarp.js on your browser simply include the following library in your html file (e.g. index.html)
```html
<script src="/socket.io/socket.io.js"></script>
<script src="/yarp.js"></script>
```
Then, within a script tag 
```js
var socket = io();  // for socket io
yarp.init(socket); // to initialize the yarp.js wrapper with socket.io
```

**Note.** In order to start working with ports, yarp.js needs to be initialized with socket io. This means that all the code related to yarp.js (e.g. opening/closing ports, writing from ports or set up `onRead` callbacks **must**  be included in

```js
yarp.onInit(function() {

    // open ports
    // onRead callbacks

}
```



<a name='browser-port-communication'></a>
### Port Communication

A browser cannot directly open a yarp port, but has to ask the server to open one. Therefore, in a nutshell, the communication between the YARP network and a browser works as follows: Suppose the server has a port open, let's say `/myport`. The Browser asks the server (via websockets) to registers to that port. If the port does not exist the server opens one for the browser. From that moment on, any message read from `/myport` is forwarded (via websockets) to the browser. The same works the other way around for the browsers to send messages on a YARP port.

<a name='browser-open-close'></a>
#### Opening/Closing Ports

To register the browser to a YARP port
```js
var port = new Port(port_type); // port_type default: 'bottle'
port.open(port_name);
```
If the port does not exist, the server opens one. Note that `port.open` returns a bool (`true` if the port was opened `false` otherwise). If the browser has already a port opened with that name, it does not open a new one and returns `false`.

You can close the port with `port.close()`, however note that since multiple clients could be connected to the same server.  
<a name='browser-read-write'></a>
#### Reading/Writing

Reading from a port is analogous to server side: you can pass a callback function that is invoked whenever a yarp port is read (on the server) to the port we are registered to. For instance
```js
port.onRead(function(message){console.log(message);});
```

Writing is analogous: `port.write(message)`. For instance if you want to send a vector (automatically transformed to a YARP Bottle over the nework):
```js
port.write([1,2,3]);
```
<a name='browser-handling-connections'></a>
#### Handling Connections

You can also (ask the server to) connect YARP ports with
```js
yarp.Network.connect('/writing/port:o','/reading/port:i');
```
and disconnect them with `yarp.Network.disconnect('/writing/port:o','/reading/port:i');`


## License

Material included in yarp.js is released under the terms of the LGPLv3.
