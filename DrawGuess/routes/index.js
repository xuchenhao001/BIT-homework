var express = require('express');
var router = express.Router();
var session = require('express-session');


/* GET home page. */
router.get('/', function (req, res, next) {
  if (req.session.username) {
    res.render('index', {username: req.session.username });
  } else {
    res.redirect('login');
  }
});


module.exports = router;
