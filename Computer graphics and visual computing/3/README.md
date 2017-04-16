# OpenGL图元绘制

需要OpenGL运行库和`glew`库(星星近大远小控制)

## 功能介绍
* 运行瞬间即产生对`test.txt`的计算结果。计算功能有六种：矩阵相乘、矩阵乘向量、矩阵乘位置、矩阵设置旋转、矩阵设置平移、矩阵设置缩放、矩阵求逆。
* 支持`test.txt`文本自由添加更改计算条件。其中，矩阵相乘需要两个四维矩阵；矩阵乘向量、矩阵乘位置、矩阵设置平移、矩阵设置缩放需要一个四维矩阵和一个三维向量（或者点坐标），矩阵设置旋转需要一个四维矩阵，一个角度和一个三维向量，矩阵求逆需要一个可逆四维矩阵。
* 利用自己编写的矩阵和向量函数，实现了`w`,`a`,`s`,`d`,`q`,`e`,`i`,`k`,`j`,`l`,`u`,`o`按键长按漫游，按`F2`或者空格可以更换视点模式为自由视点变换和欧拉视点变换。
* 支持窗口拉伸：在变换窗口大小后图像状态不变。
* 实现了当前视点跟踪，画出`120`米的小尾巴，`100`米以内为实线，`100`~`120`米渐渐消失。
* 实现了星星大小随距离改变，越近的星星越大。但是配置原因，星星设置成圆的会导致星星大小不能变化，故暂时设置成方块，不影响远处观看效果。

## 运行截图
* 漫游效果

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/3/1.jpg)

* 测试文本

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/3/2.jpg)

* 测试输出结果

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/3/3.jpg)