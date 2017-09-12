let express = require('express');
let router = express.Router();
let session = require('express-session');

/* GET drawBoard page. */
router.get('/', function (req, res, next) {
  if (req.session.username) {
    // maintain rooms
    let rooms = Object.keys(global.io.sockets.adapter.rooms);
    rooms.forEach(function (room) {
      if (room !== req.session.roomName) {
        global.connection_socket.leave(room, function () {
          console.log("leave: " + room);
        });
      }
    });

    res.render('drawBoard', {
      username: req.session.username,
      roomName: req.session.roomName
    });
  } else {
    res.redirect('login');
  }
});

/* POST logout info. */
router.post('/', function (req, res) {
  if (req.body.type === 'logout') {
    req.session.username = null;
    res.send('OK');
  } else {
    res.sendStatus(404);
  }
});
// // maintain messages
// global.connection_socket.on('__message', function (roomID, userID, message) {
//   console.log(roomID + ": " + userID + ": " + message);
//   console.log(global.connection_socket.rooms);
//   //io.sockets.emit("__message", message);
//   // broadcast to a room
//   global.io.to(roomID).emit("__message", message);
// });

module.exports = router;