# OpenGL图元绘制

需要OpenGL运行库和`glew`库(星星近大远小控制)

## 功能介绍
1. 运行瞬间即产生对`test.txt`的计算结果。计算功能有16种：
	* 欧拉角转换向量
	* 向量转换欧拉角
	* 欧拉角转换四元数
	* 四元数转换欧拉角
	* 欧拉角转换矩阵
	* 矩阵转换欧拉角
	* 矩阵转换四元数
	* 四元数转换矩阵
	* 欧拉角标准化
	* 四元数标准化
	* 四元数相乘
	* 四元数求差
	* 四元数点乘
	* 四元数求逆
	* 四元数求角度和旋转轴
	* 四元数插值
2. 支持`test.txt`文本自由添加更改计算条件。具体各命令样例请看文件夹下例子`test.txt`
3. 利用自己编写的矩阵和向量函数，实现了`w`,`a`,`s`,`d`,`q`,`e`,`i`,`k`,`j`,`l`,`u`,`o`按键长按漫游，按`F2`或者空格可以更换视点模式为自由视点变换和欧拉视点变换。
4. 自己编写特殊的一个循环栈，支持多次保存视点位置状态，每次按下`F3`可保存当前视点位置状态，每次按下`F4`可恢复上一次保存点，相对的，每次按下`F5`可恢复下一次保存点。当保存数大于自定义存储栈大小时，会自动覆盖最初的存档。当返回某个存档点后继续保存，后续的原存档点会被舍弃。
5. 自己编写欧拉角类和四元数类，实现了向量、矩阵、欧拉角、四元数之间转换、运算的各类方法。利用四元数插值方法，实现了保存的视点位置之间平滑过渡，并且可以自行调整过渡动画效果的平滑程度。
6. 支持窗口拉伸：在变换窗口大小后图像状态不变。
7. 实现了当前视点跟踪，画出`120`米的小尾巴，`100`米以内为实线，`100`~`120`米渐渐消失。
8. 实现了星星大小随距离改变，越近的星星越大。但是配置原因，星星设置成圆的会导致星星大小不能变化，故暂时设置成方块，不影响远处观看效果。

## 运行截图
* 漫游效果

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/4/1.jpg)

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/4/2.jpg)

* 测试文本

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/4/3.jpg)

* 测试输出结果

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/4/4.jpg)

