## Games101作业

配置eigen和opencv

```
指定路径 
C:\Users\wangk\source\repos\TPL>
下载vcpkg
git clone https://github.com/Microsoft/vcpkg.git
执行
cd .\vcpkg\
.\bootstrap-vcpkg.bat 
安装 opencv
.\vcpkg.exe install opencv:x64-windows

集成到vs环境
.\vcpkg integrate install
.\vcpkg integrate remove //删除
```



[Assignment](http://games-cn.org/forums/topic/allhw/) 作业



[知乎参考](https://zhuanlan.zhihu.com/p/425153734)

### Bresenham直线算法

Bresenham直线算法是一种用于将两点之间的线段绘制在屏幕上的算法。它的特点是只用基本的加法、减法和比较操作就可以完成，是一种高效的绘线算法。是计算机图形学领域使用最广泛的直线扫描转换算法，其核心思想是由误差项符号决定下一个像素点取右边的一个点还是右上的一个点。

矩阵变换
$$
Matrix=PVM\\
Mat_{model}\\
Mat_{view}\\
Mat_{projection}\\
$$
矩阵左乘，相对世界坐标系的变换；

