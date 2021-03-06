# OpenGL图元绘制

需要OpenGL运行库和`glew`库(星星近大远小控制)

## 功能介绍

* 开始运行瞬间产生并排序`10000`颗星星，默认开始为欧拉角视点模式，视角可自由变换，飞机默认朝向`z`轴正方向飞行，自由飞行模式。

* `F1`键可以控制变换飞机的飞行模式，当飞机处于自由状态下没有任何限制，一旦更改为跟随星迹航行，飞机会立即调转冲向下一个需要寻找的星星，之后一旦找到一个星星，会立即调转机头继续寻找下一个星星。在找星星过程中也可以进行视点的变换以及飞机姿态的调整，但是调整过后可能就无法达到目标星星。可以多次按下F1进行是否追踪的变换，每次重新跟踪都会在上一次追踪的基础上继续追踪下一个星星。

* `F2`键可以变换视点模式，默认为欧拉角视点控制，按下一次变为矩阵自由变换视点控制，再次按下又会回到欧拉角视点控制模式。

* 空格键可以控制视点是否跟踪飞机。默认为不跟踪飞机视点。按下一次空格可以瞬间跟踪飞机。在跟踪飞机的过程中，只有飞机的上下左右方向键和特技可以控制飞机和视点一起变化，其它控制自由变换视点的按键全部失效。

* `,<`键和`.>`键控制飞机飞行的加速和减速。每按下一次`,<`提速一次，松开后继续保持，`.>`同理。过多次按下`.>`键可能会导致速度减为负值，从而使飞船倒飞。

* 上下左右方向键控制飞船的上下左右航行方向的变化，`W`,`A`,`S`,`D`,`Q`,`E`控制自由视点的上下左右和前后移动，`I`,`K`,`J`,`L`,`U`,`O`控制自由视点的上下左右和摇摆旋转。

## 按键说明：

|按键	         |说明                                           |
|--------------|----------------------------------------------|
|`F1`            |变换飞机飞行模式，0为手动自由控制，1为跟随星迹航行|
|`F2`            |变换视点模式，0为欧拉角试图，1为自由变换视角      |
|`space`        |控制视点是否跟踪飞机，0为不跟踪飞机，1为跟踪飞机   |
|`,<`     |飞船加速飞行                                    |
|`.>`     |飞船减速飞行（过度减速可能会导致飞船倒飞）        |
|`/?`     |飞船特技（飞船边自转边向前飞行）                 |
|`↑`,`↓`,`←`,`→`|控制飞机飞行向上下左右飞行                       |
|`W`,`S`,`A`,`D` |控制视点上下左右移动【跟踪飞机时实效】            |
|`Q`,`E`        |控制视点向前向后移动【跟踪飞机时实效】            |
|`I`,`K`,`J`,`L` |控制视点上下左右旋转【跟踪飞机时实效】            |
|`U`,`O`           |控制视点左偏右偏旋转【跟踪飞机时实效】           |

## 运行截图
* 跟随星轨迹航行，跟踪飞机视角，飞机正沿着星迹（黄色）飞行：

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/5/01.jpg)

* 跟随星轨迹航行，自由观察视角，飞机正从自由位置追向下一个星星：

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/5/02.jpg)

* 跟随星轨迹航行，自由观察视角，飞机正沿着星迹（黄色）飞行，其中的转折点正是找到了一颗星星，继续追随下一个星星的拐点：

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/5/03.jpg)

* 自由飞行，自由观察视角，控制飞机画下我的学号`2053`：

![](https://github.com/xuchenhao001/BIT-homework/blob/master/Computer%20graphics%20and%20visual%20computing/5/04.jpg)

## 程序设计说明

实现题目要求：
* 完成实现Windows下的动态链接库引擎。
* 完成自己的向量类，矩阵类，欧拉角类和四元数类在引擎库中。
* 完成视点类和控制类。
* 完成前几次作业的大移植，包括星空的近大远小，星空闪烁，和星星按需连线功能。
* 完成键盘指挥飞船飞行的小游戏。实现加速减速，左转右转，向上向下飞行。做了一个非常简单的小特技动作（向前飞的同时自转）。
* 完成飞机后面尾迹设计，保证在速度变化以及视点变化情况下，尾迹仍然是飞机长度的`120`倍。且尾迹的最后`20%`为渐渐消失效果。
* 完成飞机可选飞行轨迹为星空连线轨迹。每次飞机沿着星空连线找到一个星星，连线会继续下一个星星，飞机也会随之调转机头飞向下一个星星。
