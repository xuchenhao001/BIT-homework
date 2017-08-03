# -*- coding:utf-8 -*-

import os
import pydot
import numpy as np
from sklearn import tree
from sklearn.externals import joblib
from sklearn.externals.six import StringIO


# 用户数据格式转换
def user_to_train(user):
    # user list格式:
    # sex  age  height  weight  fat_rate  target_weight  target_fat_rate
    # train_x list格式:
    # 1/0  age  height  weight  fat_rate  1/-1/0  1/-1/0
    # train_y list格式:
    # 运动方式(str) 运动心率下限(int) 运动心率上限(int) 持续时间(int) 一周运动频率(int)
    user[1] = int(user[1])
    user[2] = float(user[2])
    user[3] = float(user[3])
    user[4] = float(user[4])
    user[5] = float(user[5])
    user[6] = float(user[6])

    train = []
    if user[0] == 'male' or user[0] == 'man':  # sex
        train.append(1)
    elif user[0] == 'female':
        train.append(0)
    train.extend(user[1:5])  # age  height  weight  fat_rate
    if (user[3] - user[5]) / user[3] < -0.03:  # 增肥
        train.append(1)
    elif (user[3] - user[5]) / user[3] > -0.03:  # 减肥
        train.append(-1)
    else:  # 维持体重
        train.append(0)
    if (user[4] - user[6]) / user[4] < -0.03:  # 增脂
        train.append(1)
    elif (user[4] - user[6]) / user[4] > -0.03:  # 减脂
        train.append(-1)
    else:  # 维持体脂
        train.append(0)
    return train


# 训练数据
def sport_recommend_train(path, generate):
    # 生成3种决策树和决策图
    # train_x list格式:
    # 1/0  age  height  weight  fat_rate  1/-1/0  1/-1/0
    # train_y list格式:
    # 运动方式(str) 运动心率下限(int) 运动心率上限(int) 持续时间(int) 一周运动频率(int)
    # train_x \t train_Y
    try:
        if os.path.exists(path) is False:
            print "training file '{}' does not exist".format(path)
            return False
        if os.path.exists(generate + '/tree_sportstyle.m'):
            print "already trained"
            return True
        os.mkdir(generate)
        train_x = []
        train_y_sportstyle = []
        train_y_heartrate = []
        train_y_time_rate = []
        train_file = open(path)
        for line in train_file:
            tmp = line.split('\t')
            if len(tmp) == 1:
                continue
            X = tmp[0]
            Y = tmp[1]
            train_x.append(user_to_train(X.split()))
            Y = Y.split()
            style = Y[0]
            train_y_sportstyle.append(Y[0])
            train_y_sportstyle[-1] = style
            train_y_heartrate.append([int(Y[1]), int(Y[2])])
            train_y_time_rate.append([int(Y[3]), int(Y[4])])

        clf = tree.DecisionTreeClassifier(criterion='gini')
        dot_data = StringIO()
        clf.fit(train_x, train_y_sportstyle)
        tree.export_graphviz(clf, out_file=dot_data, filled=True)
        graph = pydot.graph_from_dot_data(dot_data.getvalue())
        graph[0].write_pdf(generate + "/tree_sportstyle.pdf")
        joblib.dump(clf, generate + '/tree_sportstyle.m')

        clf.fit(train_x, train_y_heartrate)
        tree.export_graphviz(clf, out_file=dot_data, special_characters=True)
        graph = pydot.graph_from_dot_data(dot_data.getvalue())
        graph[0].write_pdf(generate + "/tree_heartrate.pdf")
        joblib.dump(clf, generate + '/tree_heartrate.m')

        clf.fit(train_x, train_y_time_rate)
        tree.export_graphviz(clf, out_file=dot_data, rounded=True)
        graph = pydot.graph_from_dot_data(dot_data.getvalue())
        graph[0].write_pdf(generate + "/tree_time_rate.pdf")
        joblib.dump(clf, generate + '/tree_time_rate.m')

        print "training finished!"
        return True

    except Exception, e:
        print e
        return False


# 生成推荐
def recommend(record):
    if True is sport_recommend_train('train/train.txt', 'train_output'):
        if record is not None:
            user_data = np.array(
                user_to_train(
                    list(record)[2:])).reshape(1, -1)
            clf1 = joblib.load('train_output/tree_sportstyle.m')
            clf2 = joblib.load('train_output/tree_heartrate.m')
            clf3 = joblib.load('train_output/tree_time_rate.m')
            sport_style_data = clf1.predict(user_data).reshape(-1).tolist()
            heart_rate_data = clf2.predict(user_data).reshape(-1).tolist()
            time_rate_data = clf3.predict(user_data).reshape(-1).tolist()
            detail = {
                'sport_style': sport_style_data[0],
                "min_heart_rate": heart_rate_data[0],
                "max_heart_rate": heart_rate_data[1],
                "sports_duration": time_rate_data[0],
                "sports_frequency": time_rate_data[1]
            }
            return "yes", detail
        else:
            return "no", {"user_id": record[0]}
    else:
        return "no", {}
