var mysql = require('mysql');
var express = require('express');
var bodyParser = require('body-parser');
var server = require('http').Server(app);
var app=express();

server.listen(80);
app.use(bodyParser.json());
// var router = express.Router();
// router.use(bodyParser.urlencoded({extended:false}));
// router.use(bodyParser.json());
var typeid=Math.random()*10+2;
typeid = Math.floor(typeid);
console.log(typeid);

//数据库连接
var connection = mysql.createConnection({
  host     : 'localhost',
  user     : 'root',
  password : 'root',
  database : 'draw_something'
});
connection.connect();

//查询数据并返回
 connection.query('SELECT termType,termNum,termCon term FROM `term` WHERE `termID` = ' + typeid,
		 function (error, results) {
		if (error) throw error;
		console.log('---------------------------------SELECT--------------');
		console.log(results);
		console.log('-----------------------------------------------------');
		var obj1 = {
			"termID":typeid,
			"termType":results[0]['termType'],
			"termNum":results[0]['termNum'],
			"termCon":results[0]['term']
		};
		var obj2 = eval(obj1); 
		console.log(obj2);
		// app.post('../js/sql_connect.js', function(req, res){
	 //    	console.log("Hello World");
	 //    	res.send(obj1);
		// });
		app.post('../html/drawborer.html', function(req, res){
    		console.log("Hello World");
    		res.send("1");
		});
	});

connection.end();
app.listen(3000,function(){
	console.log("app is listening");
});
