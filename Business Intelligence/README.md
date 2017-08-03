# Backend MySQL Database Explanation

For our Business Intelligence class project. Database table structure & REST APIs of backend MySQL database.

## Database table structure

### users

| Name            | Type     | Remarks     |
| --------------- | -------- | ----------- |
| user_id         | CHAR(22) | PRIMARY KEY |
| password        | CHAR(22) |             |
| sex             | CHAR(22) |             |
| age             | INT      |             |
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
| full_range_longitude_latitude | BLOB     |                             |
| full_range_speed              | DOUBLE   |                             |
| full_range_steps              | INT      |                             |
| full_range_distance           | DOUBLE   |                             |
| start_time                    | DATETIME |                             |
| end_time                      | DATETIME |                             |

### friends

| Name        | Type     | Remarks                  |
| ----------- | -------- | ------------------------ |
| user_id     | CHAR(22) | PRIMARY KEY, FOREIGN KEY |
| longitude   | DOUBLE   |                          |
| latitude    | DOUBLE   |                          |
| update_time | DATETIME |                          |

## REST APIs

You can test your connection to the server through `GET` method. And if it's okay, you will get a result:

```json
{
  "status": "yes"
}
```

After that, you can test or build your Android application with APIs below with `POST` method:

### Users

* Sign up

```json
{
  "message": "users",
  "method": "signup",
  "user_id": "your_id",
  "password": "your_password",
  "sex": "male|female",
  "age": 20,
  "height": 10.24,
  "weight": 10.24,
  "fat_rate": 10.24,
  "target_weight": 10.24,
  "target_fat_rate": 10.24
}
```

> `message`, `method`, `user_id`, `password` is necessary. `user_id` should be unique. 
>
> Result:
>
> ```json
> {
>   "status": "yes",
>   "detail": {}
> }
> ```

* Update

```json
{
  "message": "users",
  "method": "update",
  "user_id": "your_id",
  "password": "your_new_password",
  "sex": "male|female",
  "age": 21,
  "height": 10.24,
  "weight": 10.24,
  "fat_rate": 10.24,
  "target_weight": 10.24,
  "target_fat_rate": 10.24
}
```

> `message`, `method`, `user_id` is necessary. Each value here will cover the value set before.
>
> Result:
>
> ```json
> {
>   "status": "yes",
>   "detail": {}
> }
> ```

* Check

```json
{
  "message": "users",
  "method": "check",
  "user_id": "your_id"
}
```

> `message`, `method`, `user_id` is necessary. 
>
> Result:
>
> ```json
> {
>   "status": "yes",
>   "detail": {
>     "height": 10.25,
>     "target_fat_rate": 10.25,
>     "target_weight": 10.24,
>     "user_id": "your_id",
>     "fat_rate": 10.25,
>     "weight": 10.25,
>     "age": 21,
>     "password": "your_new_password",
>     "sex": "male"
>   }
> }
> ```

### Sports

* Add record

```json
{
  "message": "sports",
  "method": "add",
  "user_id": "your_id",
  "full_range_longitude_latitude": [
    {"longitude": 10.24, "latitude":10.24},
    {"longitude": 10.25, "latitude":10.25},
    ...
    {"longitude": 11.24, "latitude":11.24}
  ],
  "full_range_speed": 10.24,
  "full_range_steps": 1024,
  "full_range_distance": 10.24,
  "start_time": "YYYY-MM-DD HH:MM:SS",
  "end_time": "YYYY-MM-DD HH:MM:SS"
}
```

> `message`, `method`, `user_id` is necessary. `full_range_longitude_latitude` is an array that allows as much values as you want.
>
> Result:
>
> ```json
> {
>   "status": "yes",
>   "detail": {}
> }
> ```

* Check records

```json
{
  "message": "sports",
  "method": "check",
  "user_id": "your_id"
}
```

>  `message`, `method`, `user_id` is necessary.
>
>  Result:
>
>  ```json
>  {
>    "status": "yes",
>    "detail": {
>      "user_id": "your_id",
>      "sports_record": [
>        {
>          "end_time": "2014-04-14 20:14:14",
>          "start_time": "2014-04-14 20:14:04",
>          "full_range_longitude_latitude": [
>            {
>              "latitude": 10.24,
>              "longitude": 10.24
>            },
>            {
>              "latitude": 10.25,
>              "longitude": 10.25
>            },
>            {
>              "latitude": 11.24,
>              "longitude": 11.24
>            }
>          ],
>          "full_range_steps": 1024,
>          "full_range_distance": 10.24,
>          "full_range_speed": 10.24
>        },
>        ...
>        {
>          "end_time": "2015-04-14 20:14:14",
>          "start_time": "2015-04-14 20:14:04",
>          "full_range_longitude_latitude": [
>            {
>              "latitude": 10.24,
>              "longitude": 10.24
>            },
>            {
>              "latitude": 10.25,
>              "longitude": 10.25
>            },
>            {
>              "latitude": 11.24,
>              "longitude": 11.24
>            }
>          ],
>          "full_range_steps": 1024,
>          "full_range_distance": 10.24,
>          "full_range_speed": 10.24
>        }
>      ]
>    }
>  }
>  ```

### Friends

* Update

```json
{
  "message": "friends",
  "method": "update",
  "user_id": "your_id",
  "longitude": 10.24,
  "latitude": 10.24,
  "update_time": "2017-04-14 20:14:14"
}
```

> `message`, `method`, `user_id` is necessary.
>
> Result:
>
> ```json
> {
>   "status": "yes",
>   "detail": {}
> }
> ```

* Check friends

```json
{
  "message": "friends",
  "method": "check"
}
```

> `message`, `method` is necessary.
>
> Result:
>
> ```json
> {
>   "status": "yes",
>   "detail": {
>     "friends":[
>       {
>         "latitude": 10.24,
>         "update_time": "2017-04-14 20:14:14",
>         "user_id": "friend_id_1",
>         "longitude": 10.24
>       },
>       ...
>       {
>         "latitude": 10.24,
>         "update_time": "2015-04-14 20:14:14",
>         "user_id": "friend_id_2",
>         "longitude": 10.24
>       }
>     ]
>   }
> }
> ```

### Recommends

```json
{
  "message": "recommends",
  "user_id": "your_id"
}
```

> `message`, `user_id` is necessary.
>
> Result:
>
> ```json
> {
>   "status": "yes",
>   "detail": {
>     "min_heart_rate": 65,
>     "sports_duration": 40,
>     "sport_style": "篮球",
>     "sports_frequency": 3,
>     "max_heart_rate": 75
>   }
> }
> ```