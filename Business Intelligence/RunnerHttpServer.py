# -*- coding:utf-8 -*-

import sys
import os
import json
import tornado.ioloop
import tornado.web
import mysql.connector

# 数据库参数设置：
host = "127.0.0.1"
user = "root"
password = "root"
database_name = "runner"


def database_connect():
    return mysql.connector.connect(host=host, user=user, password=password, db=database_name)


# 初始化数据库表函数
def init_database():
    db = None
    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()
        # 如果数据表已经存在使用 execute() 方法删除表。
        cursor.execute("DROP TABLE IF EXISTS users")
        cursor.execute("DROP TABLE IF EXISTS sports")
        cursor.execute("DROP TABLE IF EXISTS friends")
        # 创建用户表users
        sql = """CREATE TABLE users (
                        user_id CHAR(22) PRIMARY KEY,
                        password CHAR(22),
                        sex INT, 
                        height DOUBLE, 
                        weight DOUBLE, 
                        fat_rate DOUBLE, 
                        target_weight DOUBLE, 
                        target_fat_rate DOUBLE)
                        character set = utf8;"""
        cursor.execute(sql)
        # 创建运动数据表sports
        sql = """CREATE TABLE events (
                        sports_id INT AUTO_INCREMENT, 
                        user_id CHAR(22), 
                        start_longitude DOUBLE, 
                        start_latitude DOUBLE, 
                        full_longitude_latitude STRING, 
                        full_speed DOUBLE, 
                        full_distance DOUBLE,
                        start_time TIMESTAMP, 
                        end_time TIMESTAMP, 
                        duration_time TIMESTAMP,
                        FOREIGN KEY(user_id) REFERENCES users(user_id),
                        PRIMARY KEY(sports_id))
                        character set = utf8;"""
        cursor.execute(sql)
        # 创建好友表friends
        sql = """CREATE TABLE friends (
                        user_id CHAR(22) PRIMARY KEY,
                        friend_id CHAR(22),
                        FOREIGN KEY(friend_id) REFERENCES users(user_id))
                        character set = utf8;"""
        cursor.execute(sql)
    except Exception, e:
        print e
        print "Database init failed!"
        sys.exit(1)
    finally:
        if db is not None:
            db.close()


# 维护数据库中用户表
def users(method, user_id, password, sex, height,
          weight, fat_rate, target_weight, target_fat_rate):
    db = None
    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()
        if method == "signup":
            None
        # 插入数据库
        sql = "INSERT INTO users VALUES(" \
              "'%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')" \
              % (user_id, password, sex, height, weight, fat_rate, target_weight, target_fat_rate)
        cursor.execute(sql)
        # 提交到数据库执行
        db.commit()
    except Exception, e:
        # 发生错误时回滚
        db.rollback()
        print e
        return "Membership '%s' registered failed: %s" % (user_id, e)
    finally:
        if db is not None:
            db.close()

    return "", ""


# 维护数据库中运动表
def sports(method, user_id, start_longitude, start_latitude,
           full_range_longitude_latitude, full_range_speed, full_range_distance,
           start_time, end_time, duration_time):
    db = None

    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()
        if method == "signup":
            None
        # 插入数据库
        sql = "INSERT INTO events (user_id, start_longitude, start_latitude)\
                VALUES('%s', '%s', '%s')" \
              % (user_id, start_longitude, start_latitude)
        cursor.execute(sql)
        # 提交到数据库执行
        db.commit()
    except Exception, e:
        # 发生错误时回滚
        db.rollback()
        print e
        return "Events added failed: %s" % e
    finally:
        if db is not None:
            db.close()

    return "", ""


# 从数据库中检查用户id
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
                                     data["sex"], data["height"], data["weight"],
                                     data["fat_rate"], data["target_weight"], data["target_fat_rate"])
        elif data["message"] == "sports":
            (status, detail) = sports(data["method"], data["user_id"],
                                      data["start_longitude"], data["start_latitude"],
                                      data["full_range_longitude_latitude"],
                                      data["full_range_speed"], data["full_range_distance"],
                                      data["start_time"], data["end_time"], data["duration_time"])
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
    app.listen(8888)
    init_database()
    print "start serving..."
    tornado.ioloop.IOLoop.current().start()


if __name__ == '__main__':
    main()
