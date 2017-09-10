var express = require('express');
var router = express.Router();
var bodyParser = require('body-parser');

/* parse application/json */
router.use(bodyParser.json());

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
    req.session.username = req.body.username;
    res.send('OK');
  } else {
    res.sendStatus(404);
  }
});

module.exports = router;
