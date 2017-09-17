let express = require('express');
let router = express.Router();
let session = require('express-session');

/* GET drawBoard page. */
router.get("/", function (req, res, next) {

  // if already logged in
  if (req.session.username) {

    // set about the websocket
    let nsp = io.of(req.session.roomName);
    nsp.on("connection", function (socket) {

      // first remove all repeat server side listeners
      socket.removeAllListeners("message");
      // listen to the messages
      socket.on("message", function (message) {
        nsp.emit("message", message);
      });

      // first remove all repeat server side listeners
      socket.removeAllListeners("nextRound");
      // listen to the next round signal (whatever client sent)
      socket.on("nextRound", function () {
        // select one room leader and broadcast
        let onlineSockets = Object.keys(nsp.sockets);
        let index = Math.floor((Math.random()*onlineSockets.length));
        for (let i=0; i<onlineSockets.length; i++) {
          if (i === index) {
            // just send "1" to specified client as room leader
            nsp.to(onlineSockets[i]).emit("nextRound", "1");
          } else {
            nsp.to(onlineSockets[i]).emit("nextRound", "0");
          }
        }
      });

      // first remove all repeat server side listeners
      socket.removeAllListeners("drawing");
      // listen to drawing operation, and just relay coordinates
      socket.on("drawing", function (img) {
        nsp.emit("drawing", img);
      })
    });

    // detect room leader
    let roomLeader = null;
    if (req.session.roomLeader) {
      roomLeader = 1;
    } else {
      roomLeader = 0;
    }

    // render new page
    res.render("drawBoard", {
      username: req.session.username,
      roomName: req.session.roomName,
      endTime: nsp.flags.endTime,
      roomLeader: roomLeader
    });
  }

  // if not login
  else {
    res.redirect("login");
  }
});

/* POST logout info. */
router.post("/", function (req, res) {

  // if logout
  if (req.body.type === "logout") {
    req.session.username = null;
    res.send("OK");
  }

  // if next round, choose the room leader,
  // and then reset the timer
  else if (req.body.type === "nextRound") {

    // choose the room leader
    if (req.body.leader === 1) {
      req.session.roomLeader = 1;
    } else if (req.body.leader === 0) {
      req.session.roomLeader = 0;
    }

    // reset the timer
    let nsp = io.of(req.session.roomName);
    // set first round start timestamp
    nsp.flags.startTime = (new Date()).valueOf();
    // set round interval(s)
    nsp.flags.interval = 80 * 1000;
    // set first round end timestamp
    nsp.flags.endTime = nsp.flags.startTime + nsp.flags.interval;

    res.send("OK")
  }

  // or others...
  else {
    res.sendStatus(404);
  }
});

module.exports = router;