// here configure your MySQL connection
let config = {
  connectionLimit : 10,
  host: 'localhost',
  user: 'root',
  password: 'root',
  database: 'draw-guess',
  port: 3306
};

/*

 CREATE DATABASE `draw-guess`;

 use `draw-guess`

 CREATE TABLE `term` (
   `termID` int NOT NULL,
   `termLen` int NOT NULL,
   `termType` varchar(10) NOT NULL,
   `termCon` varchar(10) DEFAULT NULL
 ) ENGINE=InnoDB DEFAULT CHARSET=utf8;

 ALTER TABLE `term`
   ADD PRIMARY KEY (`termID`);

 ALTER TABLE `term`
   MODIFY `termID` int NOT NULL AUTO_INCREMENT;

 CREATE TABLE `userInfo` (
   `email` char(50) NOT NULL,
   `nickname` varchar(22) NOT NULL,
   `password` varchar(22) NOT NULL,
   `points` int DEFAULT NULL
 ) ENGINE=InnoDB DEFAULT CHARSET=utf8;

 ALTER TABLE `userInfo`
   ADD PRIMARY KEY (`email`);
 */

module.exports=config;