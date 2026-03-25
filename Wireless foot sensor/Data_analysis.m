filename = '/Users/aarravanil/Documents/Tech United/pressure_data.csv';

if ~isfile(filename)
    error('Run the Python script first to generate data.');
end

% Load data: Col 1 = Timestamp (s), Col 2 = Pressure
raw_data = readmatrix(filename);

if isempty(raw_data)
    error('CSV is empty. Check your Python connection.');
end

% 1. Extract columns
raw_timestamps = raw_data(:, 1);
pressure = raw_data(:, 2);

% 2. Calculate relative time in Milliseconds
% (Current Time - Start Time) * 1000
time_ms = (raw_timestamps - raw_timestamps(1)) * 1000;

% 3. Plotting
figure(1);
plot(time_ms, pressure, 'Color', [0 .5 .5], 'LineWidth', 2);
grid on;

% Labeling
title('Pressure Sensor Data');
xlabel('Time (ms)');
ylabel('Pressure');

% 4. Diagnostics: Calculate Sample Rate
if length(time_ms) > 1
    avg_diff_ms = mean(diff(time_ms));
    fprintf('Average interval: %.2f ms\n', avg_diff_ms);
    fprintf('Estimated Frequency: %.1f Hz\n', 1000/avg_diff_ms);
end