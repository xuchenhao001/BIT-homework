//server
var express = require('express');
var app = express();
var server = require('http').Server(app);
var	io = require('socket.io').listen(server);

server.listen(80);

app.use('/', express.static(__dirname + '/client'));

console.log('server started');

//socket
io.sockets.on('connection', function(socket){

    // maintain system messages
	socket.on('__system', function(roomID){
		console.log("system: " + roomID);
	});

	// maintain room
	socket.on('__connect', function (roomID, username) {
	    //leave all rooms
        var rooms = Object.keys(socket.rooms);
        console.log(rooms);
        rooms.forEach(function (room) {
            socket.leave(room);
            console.log("leave: " + room);
        });
        socket.join(roomID);
        console.log("connected: " + username + ": " + roomID);
    });

	// maintain messages
    socket.on('__message', function (roomID, username, message) {
        console.log(roomID + ": " + username + ": " + message);
        console.log(socket.rooms);
        //io.sockets.emit("__message", message);
        // broadcast to a room
        io.to(roomID).emit("__message", message);
    });
});