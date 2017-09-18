let mysql = require('mysql');
let config = require("./MySQLConfig");

let pool  = mysql.createPool(config);

exports.executeQuery=function(query,callback){
  pool.getConnection(function(err,connection){
    if (err) {
      connection.release();
      callback("ERR", {detail: err});
    }

    console.log('connected as id ' + connection.threadId);

    connection.query(query, function(err,rows){
      connection.release();
      if(!err) {
        callback("OK", {rows: rows});
      } else {
        callback("ERR", {detail: err})
      }
    });

    connection.on('error', function(err) {
      callback("ERR", {detail: err});
    });
  });
};