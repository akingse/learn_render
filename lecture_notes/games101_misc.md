

## 【10】：几何（基本表示方法）

 几何表象，显式和隐式Implicit Explicit

隐式：判断点位容易，表示不直观；
$$
f(x,y,z)=0
$$
CSG 体素法

signed Distance Function距离函数

## 【11】：几何（曲线与曲面）

 显式表示法：三角面mesh，bezier曲面，细分曲面，NURBS，点云

polygon mesh 最广泛的应用；obj文件中，f v/vt/vn

Bezier曲线



## 【12】：几何（前沿动态）

 三角形面

mesh细分

loop subdivision 路普细分

mesh简化，二次误差度量

mesh正则化



 

- ## 【17】：材质和外观

 散热现象

Material==BRDF

glossy 哑光

镜面反射（入射角=出射角）Perfect Specular Reflection  

折射，（玻璃）refraction

折射定律，不同介质有不同的折射率
$$
\eta_i sin\theta_i=\eta_t sin\theta_t
$$
全反射现象，Total internal reflection，从大eta到小eta，可见视角变小；

URDF+UTDF=USDF 散射scattering（Transmit折射）

菲涅尔项 Fresnel Reflection  Term

简化近似公式，schlick appro

微表面 microfacet

法向分布-> 

- Concentrated <==> glossy  
- Spread <==> diffuse  

表面方向性

- 各向同性 Isotropic  
- 各向异性 Anisotropic  拉丝金属

性质

- Non-negativity  非负
- Linearity  线性
- Reciprocity principle  可逆性
- Energy conservation  能量守恒
- Isotropic vs. anisotropic  异同

测量 BRDF



## 18 先进渲染

高级光线传播

Unbiased light transport methods 无偏

- Bidirectional path tracing (BDPT)双向路径追踪，较单向更耗时
- Metropolis light transport (MLT) 人名，适合复杂场景，难以估计收敛



Biased light transport methods

- Photon mapping 光子映射（有偏）适合渲染caustics，光线聚集形成的亮点
- Vertex connection and merging (VCM)



Instant radiosity (VPL / many light methods) 实时辐射度

外观建模

• Non-surface models

- Participating media 云雾 散射函数
- Hair / fur / fiber (BCSDF) 头发，marschner模型
- Granular material 颗粒材质

• Surface models

- Translucent material (BSSRDF) 半透明，透射材质，dipole appro（皮肤，玉石）次表面反射； 
- Cloth 布料，常用的三种方法
- Detailed material (non-statistical BRDF) 微观领域，波动光学（相对几何光学），

• Procedural appearance  程序化生成，noise三维函数





## 19 相机 透镜 光场

成像=合成+捕捉

Field of View (FOV)  视场

可视角 FOV=2*arctan(h/2*f)

相机镜头，通常使用标准传感器36*24，计算出等效焦距；

Expose 曝光

H=T*E，time\*irradiance

aperture光圈，f-number，光圈越小越接近小孔成像，没有景深；f是直径的倒数；

shuterr speed快门时间，导致运动模糊，

iso gain感光度，像素乘一个数，会放大噪声；

Thin Lens Approximation  等效焦距

高斯定律，焦距/物距/相距的关系；

Defocus Blur  （景深）虚焦模糊，coc弥散圆 

![image-20240324231003099](../../../../../AppData/Roaming/Typora/typora-user-images/image-20240324231003099.png)

光圈数N=f/D，焦距除光圈直径

如何运用透镜公式做光追渲染；

Depth of Field  景深

在coc的前后一小段范围内足够小，能够清晰的成像；

light field / lumigraph  光场和流明图

全光函数

光场相机



## 【20】：颜色与感知

 可见光光谱，谱功率密度Spectral Power Distribution   SPD；

颜色，是人的感知；

Metamerism（同⾊异谱），混和光谱得到同一个颜色；

加色系统，减色系统；

颜色空间-色域

HSV 色调Hue 饱和度saturation 亮度value

LAB空间，大脑神奇的互补色；

视觉欺骗，颜色本身是感知，是相对的；

CMYK检色系统

