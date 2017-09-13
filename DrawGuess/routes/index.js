let express = require('express');
let router = express.Router();
let session = require('express-session');

/* GET home page. */
router.get('/', function (req, res, next) {
  if (req.session.username) {
    let rooms = Object.keys(io.nsps).map(function (room) {
      return room.substr(1);
    });
    console.log("rooms: " + rooms);
    res.render('index', {
      username: req.session.username,
      rooms: rooms
    });
  } else {
    res.redirect('login');
  }
});

/* POST create/select room or logout info. */
router.post('/', function (req, res) {
  // create new room
  if (req.body.type === 'create') {
    let nsp = io.of(req.body.roomName);
    nsp.on('connection', function (socket) {
      console.log(req.body.roomName)
    });
    req.session.roomName = req.body.roomName;
    console.log("create: " + req.body.roomName);
    res.send('OK');
  }
  // select a room
  else if (req.body.type === 'select') {
    console.log("join: " + req.body.roomName);
    req.session.roomName = req.body.roomName;
    res.send({redirect: '/drawBoard'});
  }
  // logout
  else if (req.body.type === 'logout') {
    req.session.username = null;
    res.send('OK');
  } else {
    res.sendStatus(404);
  }
});

module.exports = router;
