let express = require('express');
let router = express.Router();
let session = require('express-session');
// let sio = require('socket.io');
// let io, connection_socket;

/* GET home page. */
router.get('/', function (req, res, next) {
  if (req.session.username) {
    // maintain room
    //let rooms = Object.keys(connection_socket.rooms);
    let rooms = Object.keys(global.io.sockets.adapter.rooms);
    rooms.forEach(function (room) {
      if (room !== req.session.roomName) {
        global.connection_socket.leave(room, function () {
          console.log("leave: " + room);
        });
      }
    });
    console.log(req.session.roomName);
    if (req.session.roomName) {
      global.connection_socket.join(req.session.roomName, function () {
        rooms = Object.keys(global.io.sockets.adapter.rooms);
        console.log("room list: " + rooms);
        res.render('index', {
          username: req.session.username,
          rooms: rooms
        });
      });
    } else {
      rooms = Object.keys(global.io.sockets.adapter.rooms);
      console.log("room list: " + rooms);
      res.render('index', {
        username: req.session.username,
        rooms: rooms
      });
    }
  } else {
    res.redirect('login');
  }
});

/* POST create/select room or logout info. */
router.post('/', function (req, res) {
  if (req.body.type === 'create') {

    // maintain room
    let rooms = Object.keys(global.connection_socket.rooms);
    rooms.forEach(function (room) {
      if (room !== req.session.roomName) {
        global.connection_socket.leave(room, function () {
          console.log("leave: " + room);
        });
      }
    });

    global.connection_socket.join(req.body.roomName);
    console.log("create: " + req.body.roomName);
    req.session.roomName = req.body.roomName;
    res.send('OK');
  } else if (req.body.type === 'select') {
    // maintain room
    let rooms = Object.keys(global.connection_socket.rooms);
    rooms.forEach(function (room) {
      if (room !== req.session.roomName) {
        global.connection_socket.leave(room, function () {
          console.log("leave: " + room);
        });
      }
    });
    global.connection_socket.join(req.body.roomName);
    console.log("join: " + req.body.roomName);
    req.session.roomName = req.body.roomName;
    res.send('OK');
  } else if (req.body.type === 'logout') {
    req.session.username = null;
    res.send('OK');
  } else {
    res.sendStatus(404);
  }
});


// router.prepareSocketIO = function (server) {
//   io = sio.listen(server);
//   io.sockets.on('connection', function (socket) {
//     connection_socket = socket;
//   });
// };

module.exports = router;
