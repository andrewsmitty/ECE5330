clc, clear, close all;
data = csvread('Feedback_Control_Data.txt');
plot(1:length(data(:,1)),data(:,1),1:length(data(:,2)),data(:,2));
grid()
legend('Measured','Target','Location','SouthWest');
title('Feedback Control');
xlabel('Time (sec)');
ylabel('RPMS');