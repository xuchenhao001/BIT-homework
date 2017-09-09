var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function (req, res, next) {
  if ("session" in req && req.session.user !== null) {
    res.render('index', {title: 'Express'});
  } else {
    console.log("error: please login first!");
    res.redirect('login');
  }

});

module.exports = router;
