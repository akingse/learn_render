# Games Family Meals

 Graphics And Mixed Environment Symposium

[官网](https://games-cn.org/)

仿真=解偏微分方程
渲染=解积分方程

[tinyrender](https://github.com/ssloy/tinyrenderer)

[piccolo](https://github.com/BoomingTech/Piccolo)



### 安装unity

[zhihu](https://zhuanlan.zhihu.com/p/618206622?utm_id=0)

注册账号

akingse@qq.com

142857Un

unity游戏引擎，可重复利用的代码和各种功能编辑器，占有率超过50%；

C#-高安全性；编辑器-所见即所得；

游戏：节奏光剑，半条命；

unity hub版本管理工具；





---



## GAMES101:现代计算机图形学入门

全面而系统地介绍现代计算机图形学的四大组成部分：（1）光栅化成像，（2）几何表示，（3）光的传播理论，以及（4）动画与模拟。每个方面都会从基础原理出发讲解到实际应用，并介绍前沿的理论研究。通过本课程，你可以学习到计算机图形学背后的数学和物理知识，并锻炼实际的编程能力。



---

## GAMES102:几何建模与处理

三维几何模型是描述三维空间内物体的表面或内部的数学表达，是计算机图形学的主要研究对象，是建模、仿真、渲染等处理过程的基础操作对象。本课程将介绍几何模型的各种表达方式、建模手段及处理方法，使得初步接触该领域的学生能够快速入门并掌握几何处理的基础算法及处理方法。主要内容包括：曲线曲面的基本理论、参数样条曲线曲面、三角网格曲面、离散微分几何、细分曲面及隐式曲面、网格曲面处理（去噪、参数化、纹理合成、简化、编辑、修复） 、形状分析与理解等。

[课程及ppt](http://staff.ustc.edu.cn/~lgliu/Courses/GAMES102_2020/default.html)

三维几何建模与处理的基础知识和方法

图形学三部分：建模，动画，渲染；

图像：离散像素化；

图形：具有数学表达的几何对象（点线面）

制作三维数据：几何数据，UV展开，贴图纹理，材质，灯光，动画

连续建模和离散建模

#### 函数拟合

数学是自然科学的语言，一种对客观世界规律进行描述的符号系统；

数学的应用：用数学语言进行建模，问题->模型->算法->代码

万能逼近定理，Weierstrass逼近定理

 定理 1：闭区间上的连续函数可用多项式级 数一致逼近 

 定理 2：闭区间上周期为2π的连续函数可用 三角函数级数一致逼近

拟合问题 fitting

逼近问题 

插值，容易被噪声outliers带歪；

---

## GAMES103：基于物理的动画入门



本课程将作为基于物理的计算机动画技术入门，着重介绍各种基础的物理动画模拟技术。

主要涵盖四个方向，分别为: 

1)刚体模拟; 2)质点弹簧、约束与布料模拟; 3)基于有限元的弹性体模拟; 4)流体模拟。

由于开发学习物理模拟需要一定的数学基础，课程初始阶段也会花一定的时间复习必备的数学知识。

物理模拟算法

课程：线性代数（SVD），微积分（链式法则，梯度，泰勒展开），编程

粒子法，网格法，MPM

Graphics图形学（geometry rendering animation），3D->2D images

Stanford Bunny 兔子 

模型描述 representation

mesh，volume mesh四面体网格，

粒子 particle，点云 point cloud，构造mesh；

网格 grid，内存占用高

渲染，photo realistic 真实感渲染，渲染管线，光线追踪，材质；

动画，人体动画，物理动画，求解物理算法；

设定时间点，更新这些时间点的状态；

方向：刚体，衣服头发，软体，流体；

混和，hybrid

### 数学基础

#### 向量 矩阵

左右手坐标系，与屏幕空间有关，右手z朝外，左手朝内；

![image-20231003214744728](../../../../../AppData/Roaming/Typora/typora-user-images/image-20231003214744728.png)

点乘叉乘

三角形重心权重,gourand shading
$$
p=b_0*x_0+b_1*x_1+b_2*x_2 \\
b_0+b_1+b_2+=1
$$
四面体 tetrahedral ，体积公式，重心权重
$$
p=b_0*x_0+b_1*x_1+b_2*x_2+b_3*x_3 \\
b_0+b_1+b_2+b_3=1
$$
MT算法

矩阵类型：转置，对角，单位阵； 

交换律x，结合律，转置，对称矩阵，逆矩阵，正交矩阵，

UDV分解
$$
A=UDV^T \\
D是对角矩阵，对角的值是奇异值 singular\\
UV是正交矩阵 ortho\\
$$
任意线性变换，是rot()\*scale()\*rot 的组合 

特征值分解 eigenvalue
$$
A=UDU^{-1} \\
只考虑对称矩阵 \\
D是对角矩阵，对角的值是奇异值 singular\\
U是正交矩阵 ortho\\
$$
对称且正定，SPD=Symmetric Positive Definiteness，

eigenvalue分解，特征值均为正；eigenvalue分解计算量太大，使用对角占优dominant判定更快；



#### Tensor Calculus 张量微积分









---

## Games104-现代游戏引擎

本课程将介绍现代游戏引擎所涉及的系统架构，技术点，引擎系统相关的知识。通过该课程，你能够对游戏引擎建立起一个全面且完整的了解。

构建游戏引擎，它是集结了现代计算机科学所有的最前沿的技术；皇冠山的钻石；

应用：虚拟人机交互，影视拍摄，军事模拟，数字孪生；

carmack卡神

显卡的出现，硬件的发展，使得系统复杂度增大；

定义：一系列工具软件的合集；

构建虚拟世界的技术框架，生产力工具，艺术的集合；

![image-20231002220849275](../../../../../AppData/Roaming/Typora/typora-user-images/image-20231002220849275.png)

现实世界引擎：无限算例，零带宽延迟（光速），无限内存，

游戏引擎：1/30s计算，带宽，延迟；

引擎用户：程序员，设计师，美工；

建立知识的体系结构，building framework

课程内容：

### 基础构建

### rendering渲染

Model, Material, Shader, Texture
Light and Shadow
Render Pipeline
Sky, Terrain, etc

### 动画 animation

![image-20231002224521550](../../../../../AppData/Roaming/Typora/typora-user-images/image-20231002224521550.png)

### 物理

Basic Concepts of Physics System
Gameplay Applications
Performance Optimization

### 游戏架构Gameplay

Event System
Scripts System
Graph Driven

### 特效系统 misc

### 工具链 tool set

### 网路 inline gaming

### 前言技术  advanced

PCG算法自动生成地图 



乐趣：像上帝一样在创造一个世界的法则；看的我热血沸腾；





---

