# -*- coding:utf-8 -*-

import base64
import copy
import json
import mysql.connector
import os
import sys
import tornado.ioloop
import tornado.web

# 数据库参数设置：
mysql_host = "127.0.0.1"
mysql_user = "root"
mysql_password = "root"
mysql_database_name = "runner"


def database_connect():
    return mysql.connector.connect(
        host=mysql_host, user=mysql_user, password=mysql_password, db=mysql_database_name)


# 初始化数据库表函数
def init_database():
    db = None
    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()
        # 如果数据表已经存在使用 execute() 方法删除表。
        cursor.execute("DROP TABLE IF EXISTS users;")
        cursor.execute("DROP TABLE IF EXISTS sports;")
        cursor.execute("DROP TABLE IF EXISTS friends;")
        # 创建用户表users
        sql = """CREATE TABLE users (
                        user_id CHAR(22) PRIMARY KEY,
                        password CHAR(22),
                        sex CHAR(22), 
                        age INT,
                        height DOUBLE, 
                        weight DOUBLE, 
                        fat_rate DOUBLE, 
                        target_weight DOUBLE, 
                        target_fat_rate DOUBLE)
                        CHARACTER SET = utf8;"""
        cursor.execute(sql)
        # 创建运动数据表sports
        sql = """CREATE TABLE sports (
                        sports_id INT AUTO_INCREMENT, 
                        user_id CHAR(22), 
                        start_longitude DOUBLE, 
                        start_latitude DOUBLE, 
                        full_range_longitude_latitude BLOB, 
                        full_range_speed DOUBLE, 
                        full_range_distance DOUBLE,
                        start_time DATETIME, 
                        end_time DATETIME, 
                        FOREIGN KEY(user_id) REFERENCES users(user_id),
                        PRIMARY KEY(sports_id))
                        CHARACTER SET = utf8;"""
        cursor.execute(sql)
        # 创建好友表friends
        sql = """CREATE TABLE friends (
                        user_id CHAR(22) PRIMARY KEY,
                        friend_id CHAR(22),
                        FOREIGN KEY(friend_id) REFERENCES users(user_id))
                        CHARACTER SET = utf8;"""
        cursor.execute(sql)
    except Exception, e:
        print e
        print "Database init failed!"
        sys.exit(1)
    finally:
        if db is not None:
            db.close()


# 维护数据库中用户表
def users(method, user_id, password, sex, age, height,
          weight, fat_rate, target_weight, target_fat_rate):
    db = None
    detail = {}
    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()

        # 注册用户
        if method == "signup":
            # 插入数据库
            sql = "INSERT INTO users VALUES(" \
                  "'{}', '{}', '{}', {}, {}, {}, {}, {}, {});".format(
                    user_id, password, sex, age, height, weight,
                    fat_rate, target_weight, target_fat_rate)
            print sql
            cursor.execute(sql)
            db.commit()
        # 更新用户信息
        elif method == "update":
            # 筛选其中不为0和""的值, 存入to_update列表
            dic = {"password": password,
                   "sex": sex,
                   "age": age,
                   "height": height,
                   "weight": weight,
                   "fat_rate": fat_rate,
                   "target_weight": target_weight,
                   "target_fat_rate": target_fat_rate}
            for k, v in dic.items():
                if v == 0 or v == "":
                    dic.pop(k)
            to_update = []
            for k, v in dic.items():
                if isinstance(v, basestring):
                    to_update.append(k + "='" + str(v) + "'")
                else:
                    to_update.append(k + "=" + str(v))
            # 更新数据库
            sql = "UPDATE users SET {} WHERE user_id = '{}';".format(",".join(to_update), user_id)
            print sql
            cursor.execute(sql)
            db.commit()
        # 检查用户信息
        elif method == "check":
            sql = "SELECT * FROM users WHERE user_id = '{}';".format(user_id)
            print sql
            cursor.execute(sql)
            record = cursor.fetchone()
            detail = {'user_id': record[0],
                      'password': record[1],
                      'sex': record[2],
                      'age': record[3],
                      'height': record[4],
                      'weight': record[5],
                      'fat_rate': record[6],
                      'target_weight': record[7],
                      'target_fat_rate': record[8]}
        # method不存在
        else:
            return "no", {"method": method}

    except Exception, e:
        # 发生错误时回滚
        db.rollback()
        print e
        return "no", "{}".format(e)
    finally:
        if db is not None:
            db.close()
    return "yes", detail


# 维护数据库中运动表
def sports(method, user_id, start_longitude, start_latitude,
           full_range_longitude_latitude, full_range_speed, full_range_distance,
           start_time, end_time):
    db = None

    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()

        # 增加记录
        if method == "add":
            # serialized = base64.b64encode(full_range_longitude_latitude, 'utf-8')
            serialized = json.dumps(full_range_longitude_latitude)
            encrypt = base64.b64encode(serialized, 'utf-8')
            sql = "INSERT INTO sports " \
                  "(user_id, start_longitude, start_latitude, " \
                  "full_range_longitude_latitude, full_range_speed, " \
                  "full_range_distance, start_time, end_time) " \
                  "VALUES('{}', {}, {}, '{}', {}, {}, '{}', '{}');".format(
                    user_id, start_longitude, start_latitude,
                    encrypt, full_range_speed,
                    full_range_distance, start_time, end_time)
            print sql
            cursor.execute(sql)
            db.commit()
            detail = {}
        # 查找记录
        elif method == "check":
            results = []
            sql = "SELECT * FROM sports WHERE user_id = '{}';".format(user_id)
            print sql
            cursor.execute(sql)
            records = cursor.fetchall()
            for record in records:
                result = {
                    'start_longitude': record[2],
                    'start_latitude': record[3],
                    'full_range_longitude_latitude': json.loads(base64.b64decode(record[4])),
                    'full_range_speed': record[5],
                    'full_range_distance': record[6],
                    'start_time': str(record[7]),
                    'end_time': str(record[8])
                }
                results.append(copy.copy(result))
            db.commit()
            detail = {'user_id': user_id, 'sports_record': results}
        # method不存在
        else:
            return "no", {"method": method}

    except Exception, e:
        # 发生错误时回滚
        db.rollback()
        print e
        return "Events added failed: %s" % e
    finally:
        if db is not None:
            db.close()
    return "yes", detail


# 维护数据库中好友表
def friends(method, user_id, friend_id):
    return "", ""


# JSON REST服务处理
class MainHandler(tornado.web.RequestHandler):
    def data_received(self, chunk):
        pass

    def get(self):
        self.set_header("Content-Type", "application/json")
        data = {"status": "yes"}
        in_json = json.dumps(data, sort_keys=True, indent=4, encoding="utf-8", ensure_ascii=False)
        self.write(in_json)

    def post(self):
        data = json.loads(self.request.body)
        status = "no"
        detail = None

        # 判断消息类型
        if data["message"] == "users":
            (status, detail) = users(data["method"], data["user_id"], data["password"],
                                     data["sex"], data["age"], data["height"],
                                     data["weight"], data["fat_rate"],
                                     data["target_weight"], data["target_fat_rate"])
        elif data["message"] == "sports":
            (status, detail) = sports(data["method"], data["user_id"],
                                      data["start_longitude"], data["start_latitude"],
                                      data["full_range_longitude_latitude"],
                                      data["full_range_speed"], data["full_range_distance"],
                                      data["start_time"], data["end_time"])
        elif data["message"] == "friends":
            (status, detail) = friends(data["method"], data["user_id"], data["friend_id"])

        self.set_header("Content-Type", "application/json")
        data = {"status": status, "detail": detail}
        in_json = json.dumps(data, indent=4, encoding="utf-8", ensure_ascii=False)
        self.write(in_json)


# 文件上传下载处理
class FtpHandler(tornado.web.RequestHandler):
    def data_received(self, chunk):
        pass

    # 上传
    def post(self, arg):
        if self.request.files:
            upload_path = os.path.join(os.path.dirname(__file__), 'files')  # 文件暂存路径
            filename = self.request.files['file'][0]['filename']
            file_content = self.request.files['file'][0]['body']
            file_path = os.path.join(upload_path, filename)
            with open(file_path, 'wb') as upload:
                upload.write(file_content)
            self.redirect("/")

    # 下载
    def get(self, arg):
        filename = arg[1:]
        print filename
        try:
            file_content = open(os.path.join(os.path.dirname(__file__), 'files', filename), 'rb')
        except IOError:
            self.set_status(404)
            return
        self.set_header('Content-Type', 'application/octet-stream')
        self.add_header('Content-Disposition', 'attachment; filename=' + filename)
        self.finish(file_content.read())


def make_app():
    return tornado.web.Application([(r"/messages", MainHandler), (r"/files(/.+)?", FtpHandler), ])


def main():
    app = make_app()
    app.listen(10086)
    init_database()
    print "start serving..."
    tornado.ioloop.IOLoop.current().start()


if __name__ == '__main__':
    main()
