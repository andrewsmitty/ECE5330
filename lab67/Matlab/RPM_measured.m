clc, clear, close all;
data = csvread('Forward_Control_Data.txt');
plot(1:length(data(:,1)),data(:,1),1:length(data(:,2)),data(:,2));
grid()
legend('Measured','Target','Location','NorthWest');
title('Forward Control');
xlabel('Time (sec)');
ylabel('RPMS');