# -*- coding:utf-8 -*-

import sys
import json
import tornado.ioloop
import tornado.web
import mysql.connector

# 数据库参数设置：
host = "127.0.0.1"
user = "root"
password = "root"
database_name = "day"


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
        cursor.execute("DROP TABLE IF EXISTS events")
        cursor.execute("DROP TABLE IF EXISTS memberships")
        # 创建数据表memberships
        sql = """CREATE TABLE memberships (
                        id CHAR(20) PRIMARY KEY,
                        password CHAR(20))
                        character set = utf8;"""
        cursor.execute(sql)
        # 创建数据表memberships
        sql = """CREATE TABLE events (
                        id CHAR(20) NOT NULL,
                        event_id CHAR(20) PRIMARY KEY,
                        topic CHAR(40),
                        date CHAR(20),
                        description CHAR(100),
                        url CHAR(30),
                        FOREIGN KEY(id) REFERENCES memberships(id))
                        character set = utf8;"""
        cursor.execute(sql)
    except Exception, e:
        print e
        print "Database init failed!"
        sys.exit(1)
    finally:
        if db is not None:
            db.close()


# 向数据库中添加注册用户信息
def add_membership_database(membership_id, membership_password):
    db = None
    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()
        # 插入数据库
        sql = "INSERT INTO memberships VALUES('%s', '%s')" % (membership_id, membership_password)
        cursor.execute(sql)
        # 提交到数据库执行
        db.commit()
    except Exception, e:
        # 发生错误时回滚
        db.rollback()
        print e
        print "Insert membership failed!"
    finally:
        if db is not None:
            db.close()

    return "Membership '%s' registered successfully!" % membership_id


# 向数据库中添加事件记录
def add_event_database(membership_id, event_id, event_topic, event_date, event_description):
    db = None

    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()
        # 插入数据库
        sql = "INSERT INTO events (id, event_id, topic, date, description)\
                    VALUES('%s', '%s', '%s', '%s', '%s')" \
                    % (membership_id, event_id, event_topic, event_date, event_description)
        cursor.execute(sql)
        # 提交到数据库执行
        db.commit()
    except Exception, e:
        # 发生错误时回滚
        db.rollback()
        print e
        print "Insert event failed!"
    finally:
        if db is not None:
            db.close()

    return "Event '%s' added successfully!" % event_topic


# 从数据库中检查用户id
def check_id_database(membership_id):
    db, results = None, None

    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()
        # 插入数据库
        sql = "SELECT * FROM memberships WHERE id = '%s'" % membership_id
        cursor.execute(sql)
        # 获取所有记录列表
        results = cursor.fetchall()
    except Exception, e:
        print e
        print "Check membership id failed!"
    finally:
        if db is not None:
            db.close()

    return len(results)


# 从数据库中查找id对应的事件
def search_event_database(membership_id):
    db, results = None, None

    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()
        # 插入数据库
        sql = "SELECT * FROM events WHERE id = '%s'" % membership_id
        cursor.execute(sql)
        # 获取所有记录列表
        results = cursor.fetchall()
    except Exception, e:
        print e
        print "Insert event failed!"
    finally:
        if db is not None:
            db.close()

    return results


# 从数据库中删除id对应用户,或event_id对应事件记录
def delete_database(flag_id, flag):
    db = None

    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()

        # 从数据库中删除对应项目
        if flag == "membership":
            sql = "DELETE FROM events WHERE id = '%s'" % flag_id
            cursor.execute(sql)
            db.commit()
            sql = "DELETE FROM memberships WHERE id = '%s'" % flag_id
            cursor.execute(sql)
            db.commit()
            return "Membership '%s' deleted successfully!" % flag_id
        elif flag == "event":
            sql = "DELETE FROM events WHERE event_id = '%s'" % flag_id
            cursor.execute(sql)
            db.commit()
            return "Event '%s' deleted successfully!" % flag_id
    except Exception, e:
        # 发生错误时回滚
        db.rollback()
        print e
        print "Delete membership/event failed!"
    finally:
        if db is not None:
            db.close()


# 更新用户密码
def update_membership_database(membership_id, membership_password):
    db = None
    try:
        db = database_connect()
        cursor = db.cursor()
        sql = "UPDATE memberships SET password = '%s' WHERE id = '%s'" % (membership_password, membership_id)
        cursor.execute(sql)
        db.commit()
    except Exception, e:
        # 发生错误时回滚
        db.rollback()
        print e
        print "Update membership failed!"
    finally:
        if db is not None:
            db.close()

    return "Membership '%s' updated successfully!" % membership_id


# 更新用户事件
def update_event_database(event_id, event_topic, event_date, event_description):
    db = None
    try:
        db = database_connect()
        cursor = db.cursor()
        sql = "UPDATE events SET topic = '%s', date = '%s', description = '%s' \
         WHERE event_id = '%s'" % (event_topic, event_date, event_description, event_id)
        cursor.execute(sql)
        db.commit()
    except Exception, e:
        # 发生错误时回滚
        db.rollback()
        print e
        print "Update event failed!"
    finally:
        if db is not None:
            db.close()

    return "Event '%s' updated successfully!" % event_topic


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
        detail = None

        # 判断操作类型
        if data["type"] == "registration":
            detail = add_membership_database(data["id"], data["password"])
        elif data["type"] == "add_event":
            detail = add_event_database(data["id"], data["event_id"], data["topic"], data["date"], data["description"])
        elif data["type"] == "check_id":
            detail = check_id_database(data["id"])
        elif data["type"] == "search_event":
            detail = search_event_database(data["id"])
        elif data["type"] == "delete":
            detail = delete_database(data["id"], data["flag"])
        elif data["type"] == "update_membership":
            detail = update_membership_database(data["id"], data["password"])
        elif data["type"] == "update_event":
            detail = update_event_database(data["event_id"], data["topic"], data["date"], data["description"])

        self.set_header("Content-Type", "application/json")
        data = {"status": "yes", "detail": detail}
        in_json = json.dumps(data, indent=4, encoding="utf-8", ensure_ascii=False)
        self.write(in_json)


def make_app():
    return tornado.web.Application([(r"/", MainHandler), ])


def main():
    app = make_app()
    app.listen(8888)
    init_database()
    print "star serving..."
    tornado.ioloop.IOLoop.current().start()

if __name__ == '__main__':
    main()
