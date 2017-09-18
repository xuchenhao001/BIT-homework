let express = require('express');
let path = require('path');
let favicon = require('serve-favicon');
let logger = require('morgan');
let cookieParser = require('cookie-parser');
let bodyParser = require('body-parser');

let index = require('./routes/index');
let login = require('./routes/login');
let signUp = require('./routes/signUp');
let drawBoard = require('./routes/drawBoard');

let app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
//app.set('view engine', 'ejs');
app.engine("html",require("ejs").__express);
app.set('view engine', 'html');

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

/* session config */
let session = require("express-session")({
  secret: "keyboard cat",
  resave: true,
  saveUninitialized: true,
  cookie:{
    maxAge: 10*60*1000 //expiration set (ms)
  }
});

app.use(session);

// routing
app.use('/', index);
app.use('/login',login);
app.use('/signUp', signUp);
app.use('/home',index);
app.use('/drawBoard', drawBoard);
app.use("/logout",index);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  let err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = app;
