import psutil
import serial
from serial.tools import list_ports
import time
def get_cpu_usage():
    # 获取CPU占用
    return psutil.cpu_percent(interval=1)

def get_memory_usage():
    # 获取内存占用
    memory_info = psutil.virtual_memory()
    return memory_info.percent


def list_serial_ports():
    """获取系统中所有可用的串口设备"""
    ports = list_ports.comports()
    return [port.device for port in ports]

def main():
    # 串口配置
    # 列出所有可用的串口
    ports = list_serial_ports()
    if not ports:
        print("没有可用的串口设备。")
        return
    
    # 显示可用的串口设备
    print("可用的串口设备：")
    for index, port in enumerate(ports, start=1):
        print(f"{index}. {port}")

    # 让用户选择一个串口设备
    choice = int(input("请选择一个串口（输入序号）："))
    if choice < 1 or choice > len(ports):
        print("无效的选择。")
        return
    
    # 获取选择的串口设备
    selected_port = ports[choice - 1]
    
    # 打开串口
    try:
        ser = serial.Serial(selected_port, baudrate=9600, timeout=1)  # 使用波特率 9600
        print(f"连接到 {selected_port}")
        
        # 每秒发送一次
        while True:
            # 获取 CPU、内存和 GPU 占用信息2
            cpu_usage = int(get_cpu_usage())
            memory_usage = int(get_memory_usage())

            # 打包数据为字符串
            data = f"{cpu_usage},{memory_usage}\0"
            print(data)
            # 通过串口发送数据

            ser.write(data.encode())

            # 等待一段时间再进行下一次读取
            time.sleep(1)
            data = ser.readline().decode().strip()  # 解码为字符串并去除两端的空白字符
            
            # 如果读取到的数据不为空，进行处理
            if data:
                print(f"从串口读取到的数据：{data}")
    except serial.SerialException as e:
        print(f"无法连接到 {selected_port}: {e}")
    
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print(f"已关闭 {selected_port}")


if __name__ == "__main__":
    main()