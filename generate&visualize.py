import numpy as np
import matplotlib.pyplot as plt
import math
import random

def AMPD(data):
    """
    实现AMPD算法
    :param data: 1-D numpy.ndarray 
    :return: 波峰所在索引值的列表
    """
    data=np.array(data)
    p_data = np.zeros_like(data, dtype=np.int32)
    count = data.shape[0]
    arr_rowsum = []
    for k in range(1, count // 2 + 1):
        row_sum = 0
        for i in range(k, count - k):
            if data[i] > data[i - k] and data[i] > data[i + k]:
                row_sum += 1
        arr_rowsum.append(row_sum)
    min_index = np.argmax(arr_rowsum)
    max_window_length = min_index
    for k in range(1, max_window_length + 1):
        for i in range(k, count - k):
            if data[i] > data[i - k] and data[i] > data[i + k]:
                p_data[i] += 1
    return np.where(p_data == max_window_length)[0]

#文件流---------------------------------------------------------------
def write_signal(sequence,filepath = r'D:\VSprojects\summit\summit\data.txt'):
    delimiter = ','  # 分隔符
    with open(filepath, 'w') as file:
        for value in sequence:
            file.write(str(value) + delimiter)
        file.close()

def read_list(filepath="D:\\VSprojects\\summit\\summit\\output.txt"):
    data_list = []  # 创建一个空列表来存储数据

    with open(filepath, 'r') as file:
        data = file.read().split(',')  # 读取文件内容并按逗号分割成数据项
        data_list = [int(item) for item in data]  # 将数据项转换为浮点数并添加到列表中

    return data_list  # 返回读取的数据列表


#序列生成---------------------------------------------------------------
def generateSinSequence(start=0, end = np.pi*10, step=0.1,delta=0):
    sequence = []
    x = start
    while x <= end:
        y = math.sin(x) + random.uniform(-delta, delta)  # 在sin(x)上加上一个[-0.1, 0.1]之间的随机扰动值
        sequence.append(y)
        x += step
    write_signal(sequence)
    return sequence

def generate_modulated_signal(start_time = 0, end_time = 20 , frequency_range=(1,20), num_points=500):
    initial_amplitude = 1.0
    final_amplitude = 0.2
    initial_period = 1.0
    final_period = 5.0
    # 时间序列
    t = np.linspace(start_time, end_time, num_points)

    # 计算振幅和周期的变化率
    amplitude_rate = (final_amplitude - initial_amplitude) / (end_time - start_time)
    period_rate = (final_period - initial_period) / (end_time - start_time)

    # 计算振幅和周期随时间的变化
    amplitude = initial_amplitude + amplitude_rate * (t - start_time)
    period = initial_period + period_rate * (t - start_time)

    # 生成信号
    signal = amplitude * np.sin(2 * np.pi * t / period)+ 0.05*amplitude*np.random.randn(num_points)
    
    write_signal(signal)
    return signal

def sim_data():
    N = 1000
    x = np.linspace(0, 200, N)
    y = 2 * np.cos(2 * np.pi * 300 * x) \
        + 5 * np.sin(2 * np.pi * 100 * x) \
        + 4 * np.random.randn(N)
    
    write_signal(y)
    return y
def really_Complex_signal(time_start=0, time_end=10, num_points=4000):
    # 生成时间轴
    t = np.linspace(time_start, time_end, num_points)

    # 定义频率和振幅随时间变化的正弦波
    frequencies = np.linspace(0.2, 100, num_points)
    amplitudes = np.linspace(0.5, 1.5, num_points)

    # 生成复杂信号
    complex_signal = np.zeros_like(t)
    for i in range(num_points):
        frequency = frequencies[i]
        amplitude = amplitudes[i]
        noise_std = 0.05 * amplitude

        signal = amplitude * np.sin(2 * np.pi * frequency * t[i])
        noise = 3*np.random.normal(0, noise_std)

        complex_signal[i] = signal + noise
        write_signal(complex_signal)
    return complex_signal

def linearAndSin(k=2.5, a= 20, time_start=0, time_end=10, num_points=1000):
    # 生成时间轴
    t = np.linspace(time_start, time_end, num_points)

    # 计算信号
    signal = k * t + 10*np.sin(a * t)+ 0.1 * 10 * np.random.randn(num_points)

    write_signal(signal)
    return signal

def linearAndMultiSin(k=2.5, a=4, frequencies= [1.0, 2.5, 5.0], amplitudes= [1.5, 1.0, 2], time_start=0, time_end=10, num_points=1000):
    # 生成时间轴
    t = np.linspace(time_start, time_end, num_points)

    # 生成线性函数
    linear_function = k * t

    # 生成多个正弦波并叠加
    complex_signal = np.zeros_like(t)
    for freq, amp in zip(frequencies, amplitudes):
        sin_wave = amp * np.sin(2 * np.pi * freq * t)
        complex_signal += sin_wave

    # 添加线性函数
    complex_signal += linear_function + 0.5*np.random.randn(num_points)
    
    write_signal(signal)
    return complex_signal
#可视化---------------------------------------------------------------
def plot_list_data(data_list, MaximaList=None):
    # 绘制图形
    x_values = range(len(data_list))
    plt.plot(x_values, data_list)

    # 标记红色点
    if MaximaList is not None:
        for index in MaximaList:
            plt.plot(index, data_list[index], 'rx')

    plt.xlabel('Index')
    plt.ylabel('Value')
    plt.legend(["data","Peaks"],loc="upper right")
    plt.title('Plot of List Data')
    plt.show()
    


def vis():
    y = sim_data()
    plt.plot(range(len(y)), y)
    px = AMPD(y)
    plt.scatter(px, y[px], color="red")

    plt.show()