let express = require('express');
let router = express.Router();
let session = require('express-session');

/* GET home page. */
router.get('/', function (req, res, next) {

  // if login
  if (req.session.email) {

    // rooms garbage collection
    Object.keys(io.nsps).forEach(function (room) {
      if (room !== "/") {
        let roomToDel = io.of(room);
        let connectedSockets = Object.keys(roomToDel.connected);

        // recycle the room if no one connected
        if (connectedSockets.length < 1) {
          roomToDel.removeAllListeners();
          delete io.nsps[room];
        }
      }
    });

    // update rooms info
    let rooms = Object.keys(io.nsps).map(function (room) {
      let roomDetail = {
        roomName: room.substr(1),
        roomPlayerNum: Object.keys(io.nsps[room].connected).length
      };
      return roomDetail;
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
