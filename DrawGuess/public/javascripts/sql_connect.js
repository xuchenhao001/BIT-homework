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
  password : '',
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

















// connection.query('SELECT 1 + 1 AS solution', function (error, results, fields) {
//   if (error) throw error;
//   console.log('The solution is: ', results[0].solution);
// });

// var  addSql = 'INSERT INTO websites(Id,name,url,alexa,country) VALUES(0,?,?,?,?)';
// var  addSqlParams = ['菜鸟工具', 'https://c.runoob.com','23453', 'CN'];
// //增
// connection.query(addSql,addSqlParams,function (err, result) {
//         if(err){
//          console.log('[INSERT ERROR] - ',err.message);
//          return;
//         }        
 
//        console.log('--------------------------INSERT----------------------------');
//        //console.log('INSERT ID:',result.insertId);        
//        console.log('INSERT ID:',result);        
//        console.log('-----------------------------------------------------------------\n\n');  
// });

// var modSql = 'UPDATE websites SET name = ?,url = ? WHERE Id = ?';
// var modSqlParams = ['菜鸟移动站', 'https://m.runoob.com',6];
// //改
// connection.query(modSql,modSqlParams,function (err, result) {
//    if(err){
//          console.log('[UPDATE ERROR] - ',err.message);
//          return;
//    }        
//   console.log('--------------------------UPDATE----------------------------');
//   console.log('UPDATE affectedRows',result.affectedRows);
//   console.log('-----------------------------------------------------------------\n\n');
// });

// var delSql = 'DELETE FROM websites where id=6';
// //删
// connection.query(delSql,function (err, result) {
//         if(err){
//           console.log('[DELETE ERROR] - ',err.message);
//           return;
//         }        
 
//        console.log('--------------------------DELETE----------------------------');
//        console.log('DELETE affectedRows',result.affectedRows);
//        console.log('-----------------------------------------------------------------\n\n');  
// });