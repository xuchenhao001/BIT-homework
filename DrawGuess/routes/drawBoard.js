let express = require('express');
let router = express.Router();
let session = require('express-session');
let nsp;

/* GET drawBoard page. */
router.get("/", function (req, res, next) {

  // if already logged in
  if (req.session.username) {

    // set about the websocket
    nsp = io.of(req.session.roomName);
    nsp.on("connection", function (socket) {

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

        // reset round unfinished status
        nsp.flags.roundFinish = 0;

        // generate a random term from MySQL
        let mysql = require("../db/MySQLConnection");
        let query = "SELECT * FROM term WHERE termID >= " +
          "((SELECT MAX(termID) FROM term) - (SELECT MIN(termID) FROM term)) * RAND() " +
          "+ (SELECT MIN(termID) FROM term)  LIMIT 1;";
        mysql.executeQuery(query, function (status, result) {
          nsp.flags.term = result.rows[0];
          console.log(nsp.flags.term);
        });
      });

      // listen to drawing operation, and just relay coordinates
      // first remove all repeat server side listeners
      socket.removeAllListeners("drawing");
      socket.on("drawing", function (img) {
        nsp.emit("drawing", img);
      });

      // listen to the messages
      // first remove all repeat server side listeners
      socket.removeAllListeners("message");
      socket.on("message", function (message, userID, roomLeader, term) {
        // correct answer, do not sent by room leader, and round didn't finished
        if (message === term.termCon && roomLeader !== "1" && !nsp.flags.roundFinish) {
          nsp.emit("message", userID + "答对了：" + message, "1");
          // set round finished status
          nsp.flags.roundFinish = 1;
        }
        // the answer is wrong
        else {
          nsp.emit("message", message, "0");
        }
      });
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

  // if next round, choose the room leader,
  // and then reset the timer
  if (req.body.type === "nextRound") {

    // choose the room leader
    if (req.body.leader === 1) {
      req.session.roomLeader = 1;
    } else if (req.body.leader === 0) {
      req.session.roomLeader = 0;
    }

    // reset the timer
    // set first round start timestamp
    nsp.flags.startTime = (new Date()).valueOf();
    // set round interval(s)
    nsp.flags.interval = 60 * 1000;
    // set first round end timestamp
    nsp.flags.endTime = nsp.flags.startTime + nsp.flags.interval;

    res.send("OK");
  }

  // if query the term
  else if (req.body.type === "queryTerm") {
    res.send(nsp.flags.term);
  }

  // if logout
  else if (req.body.type === "logout") {
    req.session.username = null;
    res.send("OK");
  }

  // or others...
  else {
    res.sendStatus(404);
  }
});

module.exports = router;