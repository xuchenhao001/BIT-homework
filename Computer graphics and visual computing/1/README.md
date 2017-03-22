# OpenGL图元绘制

需要OpenGL运行库

## 功能介绍
* 空格键切换运行状态。运行状态有三种：画闪烁的星星，将最近的星星连线，取最近的星星画三角带。
* 支持窗口拉伸：在变换窗口大小后切换运行状态同上。
* 代码首部常量可根据情况自定义如下：

	`TIME = 200;`//动画每一帧时长，单位为毫秒，默认为200ms。

	`STAR_NUM = 10000;`//星星总数量，默认为10000个。

	`width = 1280, height = 720;`//星星绘制窗口默认大小为1280x720，运行后可任意更改。

	`int mode = 0;`//程序运行时的默认状态：0：闪烁星星 1：星星连线 2：三角带。

## 运行截图
* 画闪烁星星状态
![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/1/1.jpg)
* 将最短距离星星连线
![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/1/2.jpg)
* 取最近的星星画三角带
![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/1/3.jpg)

## 程序逻辑调用层次

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/1/4.png)