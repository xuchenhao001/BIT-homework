let express = require('express');
let router = express.Router();
let bodyParser = require('body-parser');

/* parse application/json */
router.use(bodyParser.json());

/* GET sign up page. */
router.get('/', function (req, res) {
  res.render('signUp');
});

/* POST sign up info. */
router.post('/', function (req, res) {
  let mysql = require("../db/MySQLConnection");
  let query = "SELECT * FROM userInfo WHERE email = '" + req.body.email + "';";
  let insert = "INSERT INTO userInfo (`email`, `nickname`, `password`, `points`) VALUES ('" +
    req.body.email + "', '" + req.body.nickname + "', '" + req.body.password +
    "', 0);";
  mysql.executeQuery(query, function (status, result) {
    if (status === "OK") {
      // if email does not exist
      if (result.rows.length === 0) {
        mysql.executeQuery(insert, function (status) {
          if (status === "OK") {
            res.send("OK");
          } else {
            res.send("ERR");
          }
        });
      }
      // if email exists
      else {
        res.send("EXIST");
      }
    }
    // or error from mysql
    else {
      res.send("ERR");
    }
  });
});

module.exports = router;
