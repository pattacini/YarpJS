
var _yarp = require('./build/Release/YarpJS');

var yarp = new Object();


// make sure to close stuff when Ctrl+C or closing signal arrives, but don't
// force the process to exit, let it do on its own.
// A second occurrence of the signal will be processed by the default handlers
// and force stop the process (except for SIGQUIT).
process.once('SIGINT',function(){
    process.emit('exit');
});
process.once('SIGTERM',function(){
    process.emit('exit');
});
process.once('SIGQUIT',function(){
    process.emit('exit');
});



// just open the network once
yarp.Network = new _yarp.Network();
process.once('exit',function(){
    yarp.Network.fini();
});



yarp.Bottle = function Bottle(_bottle) {

    if(_bottle == undefined)
        var _bottle = _yarp.Bottle();

    _bottle._copy = _bottle.copy;
    _bottle.copy = function (obj) {
        if(_bottle.getObjType == obj.getObjType)
            _bottle._copy(obj);
        else
            _bottle.fromString(obj.toString());
    }

    _bottle.toSend = function toSend() {

        return {
            obj_type: _bottle.getObjType(),
            content: _bottle.toObject(),
            contentStr: _bottle.toString()
        };
    }

    return _bottle;
}


yarp.Image = function Image(_image) {

    if(_image == undefined)
        var _image = _yarp.Image();

    // process.on('exit',function () {
    //     var b = _image;
    // });

    _image._copy = _image.copy;
    _image.copy = function (obj) {
        if(_image.getObjType == obj.getObjType)
            _image.copy(obj);
        else
            console.log('Error: impossible to copy image');
    }


    _image.toSend = function toSend() {

        return {
            obj_type: _image.getObjType(),
            compression_type: _image.getCompressionType(),
            buffer: _image.toBinary(100),
            heigth: _image.getHeight(),
            width: _image.getWidth()
        };
    }

    return _image;
}



yarp.Sound = function Sound(_sound) {

    if(_sound == undefined)
        var _sound = _yarp.Sound();


    _sound._copy = _sound.copy;
    _sound.copy = function (obj) {
        if(_sound.getObjType == obj.getObjType)
            _sound.copy(obj);
        else
        {
            let {buffer, samples, frequency, channels} = obj;
            _sound.fromBinary(buffer, samples, frequency, channels);
        }
    }


    _sound.toSend = function toSend() {

        return {
            obj_type: _sound.getObjType(),
            content: _sound.toBinary(),
            frequency: _sound.getFrequency(),
            samples: _sound.getSamples(),
            channels: _sound.getChannels(),
            width: _sound.getBytesPerSample()
        };
    }

    return _sound;
}



function _yarp_wrap_object(obj) {

    var wrapped_obj;

    switch(obj.getObjType())
    {
        case 'image':
            wrapped_obj = yarp.Image(obj);
            break;
        case 'bottle':
            wrapped_obj = yarp.Bottle(obj);
            break;
        case 'sound':
            wrapped_obj = yarp.Sound(obj);
            break;
        default:
            wrapped_obj = obj;
    }

    return wrapped_obj;
}






var _unnamed_port_counter = 0;
var _internal_port_manager = [];




yarp.Port = function Port(_port_type) {


    var port_name = undefined;
    var port_type = _port_type;

    var _port;

    // create the port
    switch(port_type)
    {
        case 'image':
            _port = new _yarp.BufferedPortImage();
            break;
        case 'bottle':
            _port = new _yarp.BufferedPortBottle();
            break;
        case 'sound':
            _port = new _yarp.BufferedPortSound();
            break;
        case 'rpc':
            _port = new _yarp.RPCPort();
            break;

        default:
            console.log('Error! "' + port_type + '" is not a valid type!');
    }



    // apply the on read callback to wrapped objects
    _port._onRead = _port.onRead;
    _port.onRead = function (cb) {
        _port._onRead(function (obj) {
            cb(_yarp_wrap_object(obj));
        });
    }


    // if it is an RPC port
    // apply the on read callback to wrapped objects
    _port._onReplyFromWrite = _port.onReplyFromWrite;
    _port.onReplyFromWrite = function (cb) {
        _port._onReplyFromWrite(function (obj) {
            cb(_yarp_wrap_object(obj));
        });
    }



    // make sure the port closes on exit (moreover this keeps the port from being inadvertently destroyed by te garbage collector)
    // process.once('exit', function _closeOnExit() {
    //     console.log(port_name);
    //     _port.close();
    // });

    // remove the _stayAliveCallback so that the port can be collected by garbage collector
    _port.clear = function() {
        _port.close();
        process.removeListener('exit',_stayAliveCallback);
    }


    _port.getName = function () { return port_name;}

    _port._open = _port.open;
    _port.open = function (_port_name) {

        var isOpen = false;
        if(_internal_port_manager[_port_name] == undefined && _port._open(_port_name))
        {
            isOpen = true;
            port_name = _port_name;
            _internal_port_manager[port_name] = _port;
        }

        return isOpen;
    }

    _port._close = _port.close;
    _port.close = function () {

        var isClosed = false;

        _port._close();

        if(port_name != undefined)
        {
            _internal_port_manager[port_name]=undefined;
            isClosed = true;
        }

        port_name = undefined;

        return isClosed;
    }


    // wrap the prepared objects
    _port._prepare = _port.prepare;
    _port.prepare = function() {
        return _yarp_wrap_object(_port._prepare());
    }



    if(port_type == 'rpc')
    {
        _port._write = _port.writeWithReply;
        _port.write = function(msg) {

            if(msg != undefined)
            {
                var b = new yarp.Bottle();

                if(b.getObjType == msg.getObjType)
                    b.copy(msg);
                else
                    b.fromString(msg.toString());
            }

            _port._write(b);
        }

    }
    else
    {
        _port._write = _port.write;
        _port.write = function(msg) {
            if(msg != undefined)
            {
                var b = _port.prepare();
                b.copy(msg);
            }

            _port._write();
        }

    }


    if(_port._reply == undefined)
        _port._reply = _port._write;

    _port._reply = _port.reply;
    _port.reply = function(msg) {
        if(msg != undefined)
        {
            var b = new _yarp.Bottle();

            if(b.getObjType == msg.getObjType)
                b.copy(msg)
            else
                b.fromString(msg.toString());
        }

        _port._reply(b);
    }





    // _port.callback = function (msg) {console.log(port_name + msg)};
    // (default) forward the message
    // _port.callback = function (msg) {_port.write(msg.toString())};
    _port.callback = function (msg) {_port.write(msg);};
    _port.callback_reply = function (msg) {_port.reply(msg);};


    return _port;
}




yarp.portHandler = {

    get:    function get(port_name) {
        var port = undefined;
        if(port_name != undefined && _internal_port_manager[port_name] != undefined)
            port = _internal_port_manager[port_name];

        return port;
    },

    open:   function openPort(port_name,port_type) {

        var port = undefined;

        if(port_name == undefined)
        {
            _unnamed_port_counter++;
            port_name = '/yarpJS/tmp' + _unnamed_port_counter;
        }

        if(port_type == undefined)
            port_type = 'bottle';

        // if the port name is available, use it
        if(_internal_port_manager[port_name] == undefined)
        {
            port = new yarp.Port(port_type);
            port.open(port_name);
        }
        else
            port = _internal_port_manager[port_name];

        return port;
    },

    close: function close (port_name) {
        if (port_name == undefined || _internal_port_manager[port_name] == undefined)
            return false;

        _internal_port_manager[port_name].close();

    }

};






yarp.browserCommunicator = function (_io) {

    var io = _io;

    var open = function open(port_name,port_type) {

        var port = yarp.portHandler.open(port_name,port_type);

        if (port != undefined)
        {
            port.onRead(function (obj) {
                io.emit('yarp ' + port_name + ' message',obj.toSend());
                // io.emit('yarp ' + port_name + ' message',{obj_type:'none'});
            });

            if( port_type == 'rpc')
            {
               port.onReplyFromWrite(function (obj) {
                   io.emit('yarp ' + port_name + ' reply',obj.toSend());
               });
            }
        }

        return port;
    }


    var close = function close(port_name) {
        yarp.portHandler.close(port_name);
        io.emit(port_name + 'disconnected');
    }

    // initialization
    io.on('connection', function(socket){

        console.log('connected');

        // Network commands
        socket.on('yarp connect', function(connection_data) {
            yarp.Network.connect(connection_data.port_src,connection_data.port_dst);
        });

        socket.on('yarp disconnect', function(connection_data) {
            yarp.Network.disconnect(connection_data.port_src,connection_data.port_dst);
        });


        // send back the list of ports currently registered to yarp by this server
        socket.on('yarp get active ports',function(){
            io.emit('yarp active ports',Object.keys(_internal_port_manager));
        });


        socket.on('yarp open port',function(port_data){

            var port_name;
            var port_type = undefined;

            if (port_data.port_name != undefined)
            {
                port_name = port_data.port_name;
                port_type = port_data.port_type;
            }
            else
                port_name = port_data;

            port = open(port_name,port_type);

            if (port != undefined)
            {
                // if a message  is received
                socket.on('yarp ' + port_name + ' message', port.callback );
                socket.on('yarp ' + port_name + ' reply', port.callback_reply );


                socket.emit('yarp ' + port_name + ' connection success');
            }
            else
                socket.emit('yarp ' + port_name + ' connection error');
        });



        socket.on('yarp close port',function(port_data){

            var port = yarp.portHandler.get(port_data.port_name);

            if (port != undefined)
            {
                socket.removeAllListeners('yarp ' + port_data.port_name + ' message');

                // if it has NOT been asked to close only locally
                // close the port for good
                if(!port_data.locally)
                    yarp.portHandler.close(port_data.port_name);
            }

            io.emit('yarp ' + port_data.port_name + 'disconnected');
        });

    });


    return {
        open: open,
        close: close
    }

}





module.exports = yarp;
