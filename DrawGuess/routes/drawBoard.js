let express = require('express');
let router = express.Router();
let session = require('express-session');

/* GET drawBoard page. */
router.get('/', function (req, res, next) {
  if (req.session.username) {
    res.render('drawBoard', {
      username: req.session.username,
      roomName: req.session.roomName
    });
  } else {
    res.redirect('login');
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