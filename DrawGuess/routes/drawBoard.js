let express = require('express');
let router = express.Router();
let session = require('express-session');
let nsp;

/* GET drawBoard page. */
router.get("/", function (req, res, next) {

  // if already logged in
  if (req.session.email) {

    // set about the websocket
    nsp = io.of(req.session.roomName);
    nsp.on("connection", function (socket) {

      // first remove all repeat server side listeners
      socket.removeAllListeners("nextRound");
      // listen to the next round signal (whatever client sent)
      socket.on("nextRound", function () {
        // select one room leader and broadcast
        let onlineSockets = Object.keys(nsp.sockets);
        let index = Math.floor((Math.random() * onlineSockets.length));
        for (let i = 0; i < onlineSockets.length; i++) {
          if (i === index) {
            // just send "1" to specified client as room leader
            nsp.to(onlineSockets[i]).emit("nextRound", "1");
          } else {
            nsp.to(onlineSockets[i]).emit("nextRound", "0");
          }
        }

        // generate a random term from MySQL
        let mysql = require("../db/MySQLConnection");
        let query = "SELECT * FROM term WHERE termID >= " +
          "((SELECT MAX(termID) FROM term) - (SELECT MIN(termID) FROM term)) * RAND() " +
          "+ (SELECT MIN(termID) FROM term)  LIMIT 1;";
        mysql.executeQuery(query, function (status, result) {
          nsp.term = result.rows[0];
        });
      });

      // listen to drawing operation, and just relay images
      // first remove all repeat server side listeners
      socket.removeAllListeners("drawing");
      socket.on("drawing", function (img) {
        nsp.emit("drawing", img);
      });

      // listen to the messages
      // first remove all repeat server side listeners
      socket.removeAllListeners("message");
      socket.on("message", function (message, email, nickname, roomLeader, term, roundStatus) {

        // correct answer, do not sent by room leader, and round didn't finished
        if (message === term.termCon && roomLeader !== "1" && !roundStatus) {

          // broadcast the succeed message to other player
          nsp.emit("message", nickname, message, "1");

          // update points
          // add 10 points to winner
          let mysql = require("../db/MySQLConnection");
          let updateWinner = "UPDATE userinfo SET points=points+10 where email='" + email + "';";
          mysql.executeQuery(updateWinner, function (status, result) {
          });
        }

        // the answer is wrong or submit by host
        else {
          nsp.emit("message", nickname, message, "0");
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

    // periodically broadcast online player number
    setInterval(function () {
      nsp.emit("playerNum", Object.keys(nsp.connected).length);
    }, 3000);

    // update points
    let points = null;
    let mysql = require("../db/MySQLConnection");
    let queryPoints = "SELECT points FROM userinfo WHERE email='" + req.session.email + "';";
    mysql.executeQuery(queryPoints, function (status, result) {
      if (status === "OK") {
        points = result.rows[0].points;

        // render new page
        res.render("drawBoard", {
          email: req.session.email,
          nickname: req.session.nickname,
          roomName: req.session.roomName,
          points: points,
          endTime: nsp.endTime,
          playerNum: Object.keys(nsp.connected).length,
          roomLeader: roomLeader
        });
      }

      // if server is not ready to handle the request
      else {
        res.sendStatus(503);
      }
    });
  }

  // if not login
  else {
    res.redirect("login");
  }
});

/* POST info. */
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
    nsp.startTime = (new Date()).valueOf();
    // set round interval(s)
    nsp.interval = 80 * 1000;
    // set first round end timestamp
    nsp.endTime = nsp.startTime + nsp.interval;

    res.send("OK");
  }

  // if query the term
  else if (req.body.type === "queryTerm") {
    res.send(nsp.term);
  }

  // if logout
  else if (req.body.type === "logout") {
    req.session.email = null;
    res.send("OK");
  }

  // or others...
  else {
    res.sendStatus(404);
  }
});

module.exports = router;