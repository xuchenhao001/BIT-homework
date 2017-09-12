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
  console.log("用户名称为：" + req.body.username);
  if (req.body.username) {
    req.session.username = req.body.username;
    res.send('OK');
  } else {
    res.sendStatus(404);
  }
});

module.exports = router;
