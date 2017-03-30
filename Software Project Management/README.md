# `Day`后台服务器说明

本说明包含`Day`项目后台服务器的以下介绍：
* 数据库字段设计
* `REST API`设计
* 文件上传/下载

## 数据库字段
**[memberships]**

	id          	CHAR(20) PRIMARY KEY
	password    	CHAR(50)

**[events]**

	id          	CHAR(20) NOT NULL
	event_id    	INT
	topic       	CHAR(40)
	date        	CHAR(20)
	description 	TEXT
	FOREIGN KEY(id) REFERENCES memberships(id)
	PRIMARY KEY(id, event_id)

## `REST API`设计

> **[post]** http://your_host_ip:8888

### 注册用户

	{	
	  "type":"registration",
	  "id":"your_id",
	  "password":"your_password"
	}

注册成功：

	{
	  "status": "yes",
	  "detail": "Membership 'your_id' registered successfully!"
	}

注册失败：

	{
	  "status": "yes",
	  "detail": "Membership 'your_id' registered failed: 1062 (23000): Duplicate entry 'your_id' for key 'PRIMARY'"
	}

### 添加用户的所有事件

	{
      "type":"add_events",
      "id":"your_id",
      "detail": [
      {
        "topic": "your_topic",
        "date": "event_date",
        "description": "event_description",
        "event_id": 1
      },
      {
        "topic": "your_topic",
        "date": "event_date",
        "description": "event_description",
        "event_id": 2
      },
      {
        "topic": "your_topic",
        "date": "event_date",
        "description": "event_description",
        "event_id": 3
      }
	 ]
	}

添加成功：

	{
	  "status": "yes",
	  "detail": "Events added successfully!"
	}

添加失败：

	{
	  "status": "yes",
	  "detail": "Events added failed: 1062 (23000): Duplicate entry 'event_id1' for key 'PRIMARY'"
	}

### 验证用户`id`和密码

	{
	  "type":"check_id",
	  "id":"your_id",
      "password":"your_password"
	}

验证成功：

	{
	  "status": "yes",
	  "detail": 1
	}

验证失败：

	{
	  "status": "yes",
	  "detail": 0
	}

### 查询用户所有事件

	{
	  "type":"search_events",
	  "id":"your_id"
	}

查询成功：

	{
	  "status": "yes",
	  "detail": [
      {
        "event_id": 1,
        "topic": "your_topic",
        "date": "event_date",
        "id": "your_id",
        "description": "event_description"
      },
      {
        "event_id": 2,
        "topic": "your_topic",
        "date": "event_date",
        "id": "your_id",
        "description": "event_description"
      },
      {
        "event_id": 3,
        "topic": "your_topic",
        "date": "event_date",
        "id": "your_id",
        "description": "event_description"
      }
	 ]
	}

查询失败：

	{
	  "status": "yes",
	  "detail": []
	}

### 根据用户`id`删除用户

	{
	  "type":"delete",
	  "flag":"membership",
	  "id":"your_id"
	}

删除成功：

	{
	  "status": "yes",
	  "detail": "Membership 'your_id' deleted successfully!"
	}

### 根据用户`id`删除所有事件

	{
	  "type":"delete",
	  "flag":"events",
	  "id":"your_id"
	}

删除成功：

	{
	  "status": "yes",
	  "detail": "Events deleted successfully!"
	}

### 根据用户`id`更新密码

	{
	  "type":"update_membership",
	  "id":"your_id",
	  "password":"new_password"
	}

更新成功：

	{
	  "status": "yes",
	  "detail": "Membership 'your_id' updated successfully!"
	}

### 根据用户`id`更新事件

	{
	  "type":"update_events",
	  "id":"your_id",
	  "detail": [
      {
        "topic": "your_topic",
        "date": "event_date",
        "description": "event_description",
        "event_id": 7
      },
      {
        "topic": "your_topic",
        "date": "event_date",
        "description": "event_description",
        "event_id": 8
      },
      {
        "topic": "your_topic",
        "date": "event_date",
        "description": "event_description",
        "event_id": 9
      }
	 ]
	}

更新成功：

	{
	  "status": "yes",
	  "detail": "Events deleted successfully!, Events added successfully!"
	}

## 文件上传下载

### 上传

> **[post]** http://your_host_ip:8888/files

### 下载

> **[get]** http://your_host_ip:8888/files/your_file_name

