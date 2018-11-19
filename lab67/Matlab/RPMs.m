clc, clear, close all;
duty = (1/8:1/8:1)*4095;
%measured = [240,12480,34800,56940,62820,91320,124440,150000];
%measured = [60,3630,8310,15090,30540,36390,42390,50670];
%measured = [30,4530,8880,13080,18810,27870,32250,36690];
measured = [60,9176,14719,22291,26164,36690,40000,45000];
%0:40000/8:40000
stem(duty,measured);
b1 = duty'\measured'

%m = (measured(8)-measured(2))/(duty(8)-duty(2));
%x = 0:1:4095;
%y = m*(x-duty(2))+measured(2);
%x = duty;
x = 0:1:4095;
y = b1*x;
hold on 
plot([0,x],[0,y])
grid()
xlim([0,4096])


