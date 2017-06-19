 1 %%
 2 clc;
 3
 4 global t;
 5 global x;
 6 global ii;%数组下标
 7
 8 global m_x;%3轴加速度
 9 global m_y;
10 global m_z;
11
12 global d_a;%差分
13
14 t = [0];
15 ii = 0;
16 x = -100;
17
18 m_x = [0];
19 m_y = [0];
20 m_z = [0];
21 d_a = [0];
22
23 subplot(1,2,1);%将x,y,z轴加速度图像绘制在整个1X2界面中的第1格
24 p = plot(t,m_x,t,m_y,t,m_z,'EraseMode','background','MarkerSize',5);%初始化图像，图像中有3条线（t,m_x），（t,m_y），（t,m_z）
25 axis([x-200 x+200 -1600 1600]);%设置显示窗口，前两个是x轴的最小、最大极限；后两个是y轴最小、最大极限
26 grid on;%绘制网格（如果不加这句就不绘制网格）
27 subplot(1,2,2);
28 q = plot(t,d_a,'EraseMode','background','MarkerSize',5);%同理，这里只有一条线
29 axis([x-200 x+200 -1600 1600]);
30 grid on;
31
32 %%
33
34 try
35     s=serial('com5');
36 catch
37     error('cant serial');
38 end
39 set(s,'BaudRate', 38400,'DataBits',8,'StopBits',1,'Parity','none','FlowControl','none');%设置串口属性等
40 s.BytesAvailableFcnMode = 'terminator';
41 s.BytesAvailableFcn = {@callback,p,q};%设置串口回调函数，串口有数据传输过来就会转到回调函数，p、q为参数
42
43 fopen(s);%打开串口
44
45 pause;%按任一按键结束
46 fclose(s);
47 delete(s);
48 clear s
49 close all;
50 clear all;

　　该文件是主文件，首先声明各种全局变量（matlab全局变量其他函数使用时需要用global声明），然后对这些全局变量初始化，接着实例化两个绘图窗口（第一个窗口3条线——分别是x、y、z三轴的加速度随时间变化曲线；第二个窗口一条线——是差分随时间变化曲线）。34行之后为设置串口、打开串口，接下来动态绘图所有操作均在回调函数中进行了。

     来看看回调函数

callback.m

 1 %%
 2 function callback(s,BytesAvailable,p,q)
 3
 4     global t;               %引用全局变量（4-12行）
 5     global x;
 6     global ii;%数组下标
 7
 8     global m_x;%3轴加速度
 9     global m_y;
10     global m_z;
11
12     global d_a;%差分
13
14     out = fscanf(s);%读取串口数据，转换为num数据存储在data中
15     data = str2num(out);
16
17     t = [t ii];%数组插入最新数据在最后面
18     m_x = [m_x data(1,1)];
19     m_y = [m_y data(1,2)];
20     m_z = [m_z data(1,3)];
21
22     d_a = [d_a abs(m_x(1,ii+2)-m_x(1,ii+1))+abs(m_y(1,ii+2)-m_y(1,ii+1))+abs(m_z(1,ii+2)-m_z(1,ii+1))];
23
24
25     set(p(1), 'XData',t,'YData',m_x(1,:));%用新数据更新图
26     set(p(2), 'XData',t,'YData',m_y(1,:));
27     set(p(3), 'XData',t,'YData',m_z(1,:));
28
29     set(q(1), 'XData',t,'YData',d_a(1,:));
30
31     drawnow %重新绘制图，并移动两个图的窗口，使之呈现运动效果
32     x = x + 1;
33     subplot(1,2,1)
34     axis([x-200 x+200 -1600 1600]);
35     subplot(1,2,2)
36     axis([x-200 x+200 -1600 1600]);
37     ii=ii+1;
38 end

