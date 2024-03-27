

## 【6】：光栅化（离散化三角形）

**Rasterization**

透视投影，定义一个视锥，长宽比aspect ratio，视角field of view，

![image-20231005234332985](../../../../../AppData/Roaming/Typora/typora-user-images/image-20231005234332985.png)

after MVP，model view projection

Raster=Screen，pixel

像素坐标系，左下角原点，[0,0]-[width-1, height-1]

视口变换矩阵， M_viewport=translate(w/2,h/2,0)**scale(w/2,h/2,1)



## 【7】：光栅化（深度测试与抗锯齿）

三角形，最基础的多边形，一定共面，内外清晰，内部渐变； 

三角形与像素的关系，判断像素中心是否在三角形内； is_inside_2d(tri, x, y)

using包围盒，逐行扫描；锯齿aliasing

#### Antialiasing and Z-Buffering 抗锯齿

信号处理 jaggies锯齿 aliasing学名 artifact-固有缺陷

先模糊后采样Filter then sample，傅里叶展开，周期函数都可以写成cossin的线性组合

![image-20231025221000773](../../../../../AppData/Roaming/Typora/typora-user-images/image-20231025221000773.png)

MSAA 多重采样抗锯齿MultiSampling Anti-Aliasing

Occlusions and Visibility 遮挡和可见性





## 【8】：着色（光照与基本着色模型）

 illumination 光照

shading 着色

pipeline 图形管线

可见性 visibility occlusion

解决方法：深度缓存 z-buffer

画家算法，逐步遮挡覆盖

逐个像素计算深度

```c
for (each triangle T)
    for (each sample (x,y,z) in T)
        if (z < zbuffer[x,y]) // closest sample so far
            framebuffer[x,y] = rgb; // update color
            zbuffer[x,y] = z; // update depth
        else
        	; // do nothing, this sample is occluded

```



## 【9】：着色（着色频率、图形管线、纹理映射）

 shading 着色，引入明暗和颜色，应用不同材质 material；

shading是局部的，不考虑遮挡的阴影

布林冯-着色模型（3种光项，漫反射，高光，环境光）

漫反射 diffuse reflection

![image-20231116230138963](../../../../../AppData/Roaming/Typora/typora-user-images/image-20231116230138963.png)

max(0,n·l)，只考虑同向夹角；

漫反射与观察方向无关，即与v无关；

高光 specular

相机和光源夹角在范围内；半程向量，用于简化计算；

![image-20231124235354238](../../../../../AppData/Roaming/Typora/typora-user-images/image-20231124235354238.png)

衡量向量是否接近，指数p控制cos曲线下降，最终用于控制高光区域大小；

环境光 ambient

被环境（中的物体）反射的光线照亮；假设任何一个点接收到来自环境光是相同的；是一个常数，是某一种颜色，保证没有地方是黑的；
$$
L_a=k_a I_a\\

L=L_a+L_d+L_s
$$
着色频率 shading frequency

flat shading面face着色，gouraud shading顶点vertex着色，phong shading像素pixel着色；

如何确定polyface顶点的法向量，周围面法向量的（加权）平均；

逐像素的平均，Barycentric interpolation 重心插值

#### 渲染管线

 Graphics (Real-time Rendering)  Pipeline 合起来叫（图形概念太大）实时渲染管线；

 shader代码，控制顶点和像素如何进行着色；OpenGL vertex顶点着色器，fragment像素着色器

#### texture mapping 纹理贴图

三角形顶点到内部插值-重心坐标；

纹理是在定义三角形的像素属性；

插值

插值原因：（triangle）顶点才有数据，内部需要平滑过渡

插值内容：纹理坐标，颜色，法向量；

插值方法：重心坐标
$$
(x,y,z)=αA+βB +γC\\
α+β+γ=1\\
α>0,β>0,γ>0
$$
面积公式，简化叉乘面积公式，可得α,β,γ；

投影时，重心坐标可能会变，插值必须在投影前做；投影到二维后，使用逆变换把二维的像素点变回三维点，对三维点进行深度插值；

纹理应用Applying Textures

遍历光栅像素点(x,y)，重心插值出像素点的值，查询对应uv，应用值；

纹理放大 texture magn

bilinear双线性插值，插值3次；简单但效果差一些；

bicubic双三次插值；运算量较大，但效果更好；好的画质往往伴随更大的计算开销；

纹理过大，摩尔纹+锯齿；

 范围查询range query，用一种数据结构实现快速平均数计算；

mipmap，快速、近似、方形；

trilinear interpolation三线性插值在mipmap层与层之间进行插值；

各向异性过滤；

纹理应用-环境光照

in GPU，texture=data+mipmap

环境贴图（将环境光照记录作为贴图），凹凸贴图（记录相对高度，深度贴图）；

位移贴图（真的移动模型顶点），directX动态曲面细分；





