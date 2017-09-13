let express = require('express');
let router = express.Router();
let session = require('express-session');

/* GET drawBoard page. */
router.get("/", function (req, res, next) {
  if (req.session.username) {
    let nsp = io.of(req.session.roomName);
    nsp.on("connection", function (socket) {
      console.log("someone again!!!");

      socket.on("message", function (message) {
        console.log(message);
        nsp.emit("message", message);
      });
    });
    res.render("drawBoard", {
      username: req.session.username,
      roomName: req.session.roomName
    });
  } else {
    res.redirect("login");
  }
});

/* POST logout info. */
router.post("/", function (req, res) {
  if (req.body.type === "logout") {
    req.session.username = null;
    res.send("OK");
  } else {
    res.sendStatus(404);
  }
});

module.exports = router;