let express = require('express');
let router = express.Router();
let bodyParser = require('body-parser');

/* parse application/json */
router.use(bodyParser.json());

/* GET login page. */
router.get('/', function (req, res) {
  res.render('login');
});

/* POST login info. */
router.post('/', function (req, res) {
  let mysql = require("../db/MySQLConnection");
  let query = "SELECT * FROM userInfo WHERE email = '" + req.body.email + "';";
  mysql.executeQuery(query, function (status, result) {
    if (status === "OK") {
      console.log(req.body.email);
      console.log(result.rows.length);
      // if email does not exist
      if (result.rows.length === 0) {
        res.send("EMAIL_NOT_EXIST");
      }
      // if email exists & password is correct
      else if (result.rows[0].password === req.body.password) {
        req.session.email = result.rows[0].email;
        req.session.nickname = result.rows[0].nickname;
        req.session.points = result.rows[0].points;
        res.send("OK");
      }
      // or password is wrong
      else {
        res.send("PW_ERR");
      }
    }
    // or error from mysql
    else {
      res.send("ERR");
    }
  });
});

module.exports = router;
