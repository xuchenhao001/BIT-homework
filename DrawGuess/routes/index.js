let express = require('express');
let router = express.Router();
let session = require('express-session');

/* GET home page. */
router.get('/', function (req, res, next) {

  // if login
  if (req.session.email) {

    // check sessionID for only one login per account check
    let mysql = require("../db/MySQLConnection");
    let querySession = "SELECT sessionID FROM userInfo WHERE email = '" + req.session.email + "';";
    mysql.executeQuery(querySession, function (status, result) {
      // if update session successfully
      if (status === "OK") {
        // if sessionID does not correct, someone else logged in.
        if (result.rows[0].sessionID !== req.sessionID) {
          res.redirect('login');
        }

        // if sessionID is correct
        else {

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
            return {
              roomName: room.substr(1),
              roomPlayerNum: Object.keys(io.nsps[room].connected).length
            };
          });

          // update points
          let points = null;
          let queryPoints = "SELECT points FROM userInfo WHERE email='" + req.session.email + "';";
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
      }

      // or error from mysql
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

  // create or select a room
  if (req.body.type === 'room') {
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
