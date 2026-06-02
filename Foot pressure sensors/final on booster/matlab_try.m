%%
port = "COM3";      % change this
baud = 9600;

s = serialport(port, baud);
configureTerminator(s, "LF");

figure
h = animatedline;
xlabel("Time")
ylabel("Pressure (Pa)")
grid on

startTime = datetime("now");
arrayData = NaN(1000, 1);

for N = 1:1000
    data = readline(s);
    value = str2double(data);

    t = seconds(datetime("now") - startTime);
    arrayData(N) = value;

    addpoints(h, t, value);
    drawnow limitrate
end
%%
windowSize = 10; 
averagedData = movmean(arrayData, windowSize);

% 3. Create the plot
figure;
hold on; % This allows us to plot multiple lines on the same graph

% Plot the raw data first. 
% Using a light gray color so it sits in the background.
plot(arrayData, 'Color', [0.7 0.7 0.7], 'DisplayName', 'Raw Data');

% Plot the averaged data on top.
% Using red and a thicker line width so it stands out.
plot(averagedData, 'Color', 'r', 'LineWidth', 1, 'DisplayName', ['Moving Average (N=', num2str(windowSize), ')']);

% 4. Make it look professional
xlabel('Sample Number');
ylabel('Pressure (Pa)'); % Assuming this is still your pressure data
title('Raw vs. Moving Average Filtered Data');
legend('Location', 'best');
grid on;
hold off;