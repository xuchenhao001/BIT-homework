let express = require('express');
let router = express.Router();
let session = require('express-session');

/* GET home page. */
router.get('/', function (req, res, next) {

  // if login
  if (req.session.email) {

    // namespaces garbage collection ----didn't finish!
    // Object.keys(io.nsps).forEach(function (room) {
    //   console.log("now room: " + room);
    //
    //   let namespaceToDel = io.of(room);
    //   let connectedNamespaceSockets = Object.keys(namespaceToDel.connected);
    //
    //   console.log("connected: " + connectedNamespaceSockets);
    //
    //   if (connectedNamespaceSockets.length === 0) {
    //     connectedNamespaceSockets.forEach(function (socketID) {
    //       // Disconnect Each socket
    //       namespaceToDel.connected[socketID].disconnect();
    //     });
    //     namespaceToDel.removeAllListeners();
    //     delete io.nsps[room];
    //   }
    // });

    let rooms = Object.keys(io.nsps).map(function (room) {
        return room.substr(1);
    });

    // update points
    let points = null;
    let mysql = require("../db/MySQLConnection");
    let queryPoints = "SELECT points FROM userinfo WHERE email='" + req.session.email + "';";
    mysql.executeQuery(queryPoints, function (status, result) {
      if (status === "OK") {
        points = result.rows[0].points;

        res.render('index', {
          nickname: req.session.nickname,
          points: points,
          rooms: rooms
        });
      }

      // if server is not ready to handle the request
      else {
        res.sendStatus(503);
      }
    });

  }

  // please login first
  else {
    res.redirect('login');
  }
});

/* POST create/select room or logout info. */
router.post('/', function (req, res) {

  // create new room
  if (req.body.type === 'create') {
    let nsp = io.of(req.body.roomName);
    nsp.on('connection', function (socket) {

    });
    req.session.roomName = req.body.roomName;

    console.log("create: " + req.body.roomName);
    res.send({redirect: '/drawBoard'});
  }

  // select a room
  else if (req.body.type === 'select') {
    req.session.roomName = req.body.roomName;
    res.send({redirect: '/drawBoard'});
  }

  // logout
  else if (req.body.type === 'logout') {
    req.session.email = null;
    res.send('OK');
  }

  // or others...
  else {
    res.sendStatus(404);
  }
});

module.exports = router;
