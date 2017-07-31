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
        host=mysql_host,
        user=mysql_user,
        password=mysql_password,
        db=mysql_database_name)


# 初始化数据库表函数
def init_database():
    db = None
    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()
        # 如果数据表已经存在使用 execute() 方法删除表。
        cursor.execute("DROP TABLE IF EXISTS sports;")
        cursor.execute("DROP TABLE IF EXISTS friends;")
        cursor.execute("DROP TABLE IF EXISTS users;")
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


def dic_values_to_string_list(dic_values):
    string_list = []
    for value in dic_values:
        if isinstance(value, basestring):
            string_list.append("'" + str(value) + "'")
        else:
            string_list.append(str(value))
    return string_list


# 维护数据库中用户表
def users(data):
    db = None
    try:
        # 打开数据库连接
        db = database_connect()
        cursor = db.cursor()

        method = data.pop("method")
        # 用户注册
        if method == "signup":
            to_insert = dic_values_to_string_list(data.values())
            sql = "INSERT INTO users ({}) VALUES ({});".format(
                ",".join(data.keys()),
                ",".join(to_insert))
            print sql
            cursor.execute(sql)
            db.commit()
            detail = {}
        # 更新用户
        elif method == "update":
            user_id = data.pop("user_id")
            to_update = []
            for k, v in data.items():
                if isinstance(v, basestring):
                    to_update.append(k + "='" + str(v) + "'")
                else:
                    to_update.append(k + "=" + str(v))
            sql = "UPDATE users SET {} WHERE user_id = '{}';".format(
                ",".join(to_update),
                user_id)
            print sql
            cursor.execute(sql)
            db.commit()
            detail = {}
        # 检查用户
        elif method == "check":
            user_id = data.pop("user_id")
            sql = "SELECT * FROM users WHERE user_id = '{}';".format(user_id)
            print sql
            cursor.execute(sql)
            record = cursor.fetchone()
            if record is not None:
                detail = {'user_id': record[0],
                          'password': record[1],
                          'sex': record[2],
                          'age': record[3],
                          'height': record[4],
                          'weight': record[5],
                          'fat_rate': record[6],
                          'target_weight': record[7],
                          'target_fat_rate': record[8]}
            else:
                return "no", {"user_id": user_id}
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
# def sports(method, user_id, start_longitude, start_latitude,
#            full_range_longitude_latitude, full_range_speed,
#            full_range_distance, start_time, end_time):


def sports(data):
    db = None
    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()

        method = data.pop("method")
        # 增加运动记录
        if method == "add":
            full_range_ll = data.pop("full_range_longitude_latitude")
            serialized = json.dumps(full_range_ll)
            encrypted = base64.b64encode(serialized, 'utf-8')
            to_insert_values = dic_values_to_string_list(data.values())
            to_insert_values.append("'" + encrypted + "'")
            to_insert_keys = data.keys()
            to_insert_keys.append("full_range_longitude_latitude")
            sql = "INSERT INTO sports ({}) VALUES({});".format(
                ",".join(to_insert_keys),
                ",".join(to_insert_values))
            print sql
            cursor.execute(sql)
            db.commit()
            detail = {}
        # 查找运动记录
        elif method == "check":
            results = []
            user_id = data.pop("user_id")
            sql = "SELECT * FROM sports WHERE user_id = '{}';".format(user_id)
            print sql
            cursor.execute(sql)
            records = cursor.fetchall()
            for record in records:
                result = {
                    'start_longitude': record[2],
                    'start_latitude': record[3],
                    'full_range_longitude_latitude':
                        json.loads(base64.b64decode(record[4])),
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
        in_json = json.dumps(data, sort_keys=True, indent=4,
                             encoding="utf-8", ensure_ascii=False)
        self.write(in_json)

    def post(self):
        data = json.loads(self.request.body)
        status = "no"
        detail = None

        # 判断消息类型
        message = data.pop("message")
        if message == "users":
            (status, detail) = users(data)
        elif message == "sports":
            (status, detail) = sports(data)
        elif message == "friends":
            (status, detail) = friends(data)

        self.set_header("Content-Type", "application/json")
        data = {"status": status, "detail": detail}
        in_json = json.dumps(
            data, indent=4, encoding="utf-8", ensure_ascii=False)
        self.write(in_json)


# 文件上传下载处理
class FtpHandler(tornado.web.RequestHandler):
    def data_received(self, chunk):
        pass

    # 上传
    def post(self, arg):
        if self.request.files:
            upload_path = os.path.join(
                os.path.dirname(__file__), 'files')  # 文件暂存路径
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
            file_content = open(os.path.join(
                os.path.dirname(__file__), 'files', filename), 'rb')
        except IOError:
            self.set_status(404)
            return
        self.set_header('Content-Type', 'application/octet-stream')
        self.add_header('Content-Disposition',
                        'attachment; filename=' + filename)
        self.finish(file_content.read())


def make_app():
    return tornado.web.Application(
        [(r"/messages", MainHandler),
         (r"/files(/.+)?", FtpHandler), ])


def main():
    app = make_app()
    app.listen(10086)
    init_database()
    print "start serving..."
    tornado.ioloop.IOLoop.current().start()


if __name__ == '__main__':
    main()
