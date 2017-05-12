# R作业教程

## 安装导入xlsx数据

下载安装R的xlsx包，用来读取xlsx文件。在弹出窗口选择china，如果提示有错误，关闭R，右键以管理员身份运行R

```
install.packages("xlsx")
```

加载xlsx包，不加载的包一律不能使用的

```
library(xlsx)
```

读入xlsx文件，注意其中路径的填写，每一层之间要写两个`\\`，最后要指向最终文件，后面的参数`sheetIndex=1`表明要加载xlsx中第一个sheet

```
read.xlsx2(file="C:\\Users\\Doraemon\\Desktop\\study\\Geevenchy\\BABA.xls",sheetIndex=1)
```

>Tips: 如果想要将xlsx读入某个变量，比如`Mydata`需要利用语句如下：
```
Mydata<-read.xlsx2(file="C:\\Users\\Doraemon\\Desktop\\study\\Geevenchy\\BABA.xls",sheetIndex=1)
```
## 提取某一列数据并转为向量

取出某一列的值

```
v=Mydata[5]
```

查看数据类型：（当前是list类型）

```
mode(v)
```

列表转向量：

```
v = as.vector(unlist(v[1]))
```

删除向量第一个元素`close`：

```
v = v[-1]
```