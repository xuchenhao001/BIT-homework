# Backend MySQL Database Explanation

For our Business Intelligence class project. Database table structure & REST APIs of backend MySQL database.

## Database table structure

### users

| Name            | Type     | Remarks     |
| --------------- | -------- | ----------- |
| user_id         | CHAR(22) | PRIMARY KEY |
| password        | CHAR(22) |             |
| sex             | INT      |             |
| height          | DOUBLE   |             |
| weight          | DOUBLE   |             |
| fat_rate        | DOUBLE   |             |
| target_weight   | DOUBLE   |             |
| target_fat_rate | DOUBLE   |             |

### sports

| Name                          | Type     | Remarks                     |
| ----------------------------- | -------- | --------------------------- |
| sports_id                     | INT      | AUTO_INCREMENT, PRIMARY KEY |
| user_id                       | CHAR(22) | FOREIGN KEY                 |
| start_longitude               | DOUBLE   |                             |
| start_latitude                | DOUBLE   |                             |
| full_range_longitude_latitude | STRING   |                             |
| full_range_speed              | DOUBLE   |                             |
| full_range_distance           | DOUBLE   |                             |
| start_time                    | DATETIME |                             |
| end_time                      | DATETIME |                             |

## REST APIs

You can test your connection to the server through `GET` method. And if it's okay, you will get a result:

```json
{
  "status": "yes"
}
```

After that, you can test or build your Android application with APIs below with `POST` method:

### Users

Sign up

```json
{
  "message": "users",
  "method": "signup",
  "user_id": "your_id",
  "password": "your_password",
  "sex": "male|female",
  "height": 10.24,
  "weight": 10.24,
  "fat_rate": 10.24,
  "target_weight": 10.24,
  "target_fat_rate": 10.24
}
```

> `message`, `method`, `user_id`, `password` is necessary. `user_id` should be unique. The othe values, which is `""` or `0`, will not be recorded, but they must be sent as normal.

Update

```json
{
  "message": "users",
  "method": "update",
  "user_id": "your_id",
  "password": "your_new_password",
  "sex": "male|female",
  "height": 10.24,
  "weight": 10.24,
  "fat_rate": 10.24,
  "target_weight": 10.24,
  "target_fat_rate": 10.24
}
```

> `message`, `method`, `user_id` is necessary. The othe values, which is `""` or `0`, will not be updated, but they must be sent as normal.

Check

```json
{
  "message": "users",
  "method": "check",
  "user_id": "your_id",
  "password": "",
  "sex": "",
  "height": 0,
  "weight": 0,
  "fat_rate": 0,
  "target_weight": 0,
  "target_fat_rate": 0
}
```

> `message`, `method`, `user_id` is necessary. Other values will be ignored whatever it is, but you must sent them as normal, or there will be an error occured.

### Sports

Add record

```json
{
  "message": "sports",
  "method": "add",
  "user_id": "your_id",
  "start_longitude": 10.24,
  "start_latitude": 10.24,
  "full_range_longitude_latitude": [
    {"longitude": 10.24, "latitude":10.24},
    {"longitude": 10.25, "latitude":10.25},
    ...
    {"longitude": 11.24, "latitude":11.24}
  ],
  "full_range_speed": 10.24,
  "full_range_distance": 10.24,
  "start_time": "YYYY-MM-DD HH:MM:SS",
  "end_time": "YYYY-MM-DD HH:MM:SS"
}
```

> Each key & value is necessary. `full_range_longitude_latitude` allow as much values as you want.

Check records

```json
{
  "message": "sports",
  "method": "check",
  "user_id": "your_id",
  "start_longitude": 0,
  "start_latitude": 0,
  "full_range_longitude_latitude": [],
  "full_range_speed": 0,
  "full_range_distance": 0,
  "start_time": "",
  "end_time": ""
}
```

>  `message`, `method`, `user_id` is necessary. Other values will be ignored whatever it is, but you must sent them as normal, or there will be an error occured.

### Friends

coming soon...