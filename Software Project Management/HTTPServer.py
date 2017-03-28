# -*- coding:utf-8 -*-

import sys
import os
import copy
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
                        event_id INT,
                        topic CHAR(40),
                        date CHAR(20),
                        description TEXT,
                        FOREIGN KEY(id) REFERENCES memberships(id),
                        PRIMARY KEY(id, event_id))
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
        return "Membership '%s' registered failed: %s" % (membership_id, e)
    finally:
        if db is not None:
            db.close()

    return "Membership '%s' registered successfully!" % membership_id


# 向数据库中添加事件记录
def add_event_database(membership_id, detail):
    db = None

    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()
        # 插入数据库
        for event in detail:
            sql = "INSERT INTO events (id, event_id, topic, date, description)\
                    VALUES('%s', %d, '%s', '%s', '%s')" \
              % (membership_id, event['event_id'], event['topic'], event['date'], event['description'])
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

    return "Events added successfully!"


# 从数据库中检查用户id
def check_id_database(membership_id, membership_password):
    db, results = None, None

    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()
        # 插入数据库
        sql = "SELECT * FROM memberships WHERE id = '%s' AND password = '%s'"\
              % (membership_id, membership_password)
        cursor.execute(sql)
        # 获取所有记录列表
        results = cursor.fetchall()
    except Exception, e:
        print e
        return "Check membership id failed: %s" % e
    finally:
        if db is not None:
            db.close()

    return len(results)


# 从数据库中查找用户id对应的事件
def search_events_database(membership_id):
    db, records = None, None
    results, result = [], {}

    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()
        # 插入数据库
        sql = "SELECT * FROM events WHERE id = '%s'" % membership_id
        cursor.execute(sql)
        # 获取所有记录列表
        records = cursor.fetchall()
        for membership_id, event_id, topic, date, description in records:
            result['id'] = membership_id
            result['event_id'] = event_id
            result['topic'] = topic
            result['date'] = date
            result['description'] = description
            results.append(copy.copy(result))
        return results

    except Exception, e:
        print e
        return "Insert events failed: %s" % e
    finally:
        if db is not None:
            db.close()


# 从数据库中删除id对应用户,或id用户的所有事件记录
def delete_database(flag_id, flag):
    db = None

    try:
        # 打开数据库连接
        db = database_connect()
        # 使用cursor()方法获取操作游标
        cursor = db.cursor()

        # 从数据库中删除对应用户或用户所有事件
        sql = "DELETE FROM events WHERE id = '%s'" % flag_id
        cursor.execute(sql)
        db.commit()
        if flag == "membership":
            sql = "DELETE FROM memberships WHERE id = '%s'" % flag_id
            cursor.execute(sql)
            db.commit()
            return "Membership '%s' deleted successfully!" % flag_id
        elif flag == "events":
            return "Events deleted successfully!"
    except Exception, e:
        # 发生错误时回滚
        db.rollback()
        print e
        return "Delete membership/events failed: %s" % e
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
        return "Update membership failed: %s" % e
    finally:
        if db is not None:
            db.close()

    return "Membership '%s' updated successfully!" % membership_id


# 更新用户事件
def update_events_database(membership_id, detail):

    detail1 = delete_database(membership_id, "events")
    detail2 = add_event_database(membership_id, detail)
    return "%s, %s" % (detail1, detail2)


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
        detail = None

        # 判断操作类型
        if data["type"] == "registration":
            detail = add_membership_database(data["id"], data["password"])
        elif data["type"] == "add_events":
            detail = add_event_database(data["id"], data["detail"])
        elif data["type"] == "check_id":
            detail = check_id_database(data["id"], data["password"])
        elif data["type"] == "search_events":
            detail = search_events_database(data["id"])
        elif data["type"] == "delete":
            detail = delete_database(data["id"], data["flag"])
        elif data["type"] == "update_membership":
            detail = update_membership_database(data["id"], data["password"])
        elif data["type"] == "update_events":
            detail = update_events_database(data["id"], data["detail"])

        self.set_header("Content-Type", "application/json")
        data = {"status": "yes", "detail": detail}
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
            file_content = open(os.getcwd() + '/files/' + filename, 'rb')
        except IOError:
            self.set_status(404)
            return
        self.set_header('Content-Type', 'application/octet-stream')
        self.add_header('Content-Disposition', 'attachment; filename=' + filename)
        self.finish(file_content.read())


def make_app():
    return tornado.web.Application([(r"/", MainHandler), (r"/files(/.+)?", FtpHandler), ])


def main():
    app = make_app()
    app.listen(8888)
    init_database()
    print "start serving..."
    tornado.ioloop.IOLoop.current().start()


if __name__ == '__main__':
    main()
