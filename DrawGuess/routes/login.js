var express = require('express');
var router = express.Router();
var bodyParser = require('body-parser');
var session = require('express-session');

/* parse application/json */
router.use(bodyParser.json());

/* session config */
router.use(session({
  secret:'secret',
  resave:true,
  saveUninitialized:false,
  cookie:{
    maxAge: 10*60*1000 //expiration set (ms)
  }
}));

/* GET login page. */
router.get('/', function (req, res) {
  res.render('login');
});

/* POST login info. */
router.post('/', function (req, res) {
  console.log("用户名称为：" + req.body.username);
  var user = {
    username: 'admin'
  };
  if (req.body.username === user.username) {
    res.sendStatus(200);
  } else {
    res.sendStatus(404);
  }
});

module.exports = router;
