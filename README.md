📝 项目简介
本项目是一个基于 STM32F103 微控制器的物联网传感器数据采集与上传节点。设备能够实时采集环境中的温湿度及有害气体浓度，通过 OLED 屏幕进行本地状态显示，并利用 ESP8266 WiFi 模块将数据通过 MQTT 协议打包（JSON 格式）上传至云端服务器（支持 OneNET 或自建 MQTT 服务器）。同时，设备支持接收云端下发的控制命令（如控制 LED 状态）。

🛠️ 硬件架构选型
主控芯片 (MCU): STM32F103c8t6 (基于 ARM Cortex-M3)

网络通信: ESP8266-01s 模块 (通过串口 AT 指令交互)

传感器:

DHT11: 温湿度传感器

MQ-2: 烟雾/可燃气体传感器

显示模块: 0.96 寸 I2C OLED 显示屏

其他外设: 独立按键、LED 指示灯

📂 目录结构说明
├── core/           # ARM Cortex-M3 内核文件及启动文件 (startup_stm32f10x_*.s)
├── fwlib/          # STM32F10x 标准外设库 (GPIO, USART, I2C, SPI, ADC, TIM 等)
├── hardware/       # 硬件外设驱动层
│   ├── inc/        # 驱动头文件
│   └── src/        # 驱动源文件 (led.c, key.c, dht11.c, mq2.c, i2c.c 等)
├── NET/            # 网络协议栈与云平台接入层
│   ├── CJSON/      # cJSON 轻量级 JSON 解析/封装库
│   ├── device/     # ESP8266 模块驱动 (AT 指令封装)
│   ├── MQTT/       # MQTT 协议底层实现 (MqttKit, 连包、心跳维持)
│   └── onenet/     # OneNET 平台接入协议 (鉴权算法 Base64, HMAC_SHA1)
├── OLED/           # OLED 屏幕驱动及字库文件
├── user/           # 用户应用层
│   └── main.c      # 主程序入口，包含业务逻辑状态机
├── .gitignore      # Git 版本控制忽略清单
└── stm32f103.uvprojx # Keil MDK 工程文件
✨ 核心功能特性
多传感器融合采集: 利用 ADC 和单总线/I2C 协议，稳定采集环境温湿度及多路气体浓度。

网络自动重连机制: 具备完整的 ESP8266 状态机管理，支持 AP 自动配网、WiFi 断线重连、MQTT 服务器掉线重连。

标准 MQTT 与 JSON 封包: 采用 cJSON 库将传感器数据格式化为标准的 JSON 字符串，兼容主流物联网平台的数据解析格式。

云端双向通信:

上行: 定时发布 (Publish) 传感器数据主题。

下行: 订阅 (Subscribe) 控制主题，实时响应云端或移动端 App 下发的控制指令。

OLED 本地交互: 实时显示网络连接状态、IP 地址、传感器数值，方便现场调试与监控。

🚀 编译与使用指南
开发环境: 请使用 Keil MDK-ARM v5 及以上版本打开。

工程文件: 双击根目录下的 stm32f103.uvprojx 即可加载完整工程。

修改配置: * 在 main.c 或对应的网络配置头文件中，修改你的 WiFi SSID 和密码。

修改 MQTT 服务器的 IP、端口号、DeviceID、ProductID 及鉴权 Token。

编译下载: 点击 Keil 的 Build (F7) 编译工程，0 Error 后通过 ST-Link 或 DAP-Link 将固件烧录至单片机。

📅 版本迭代历史 (Changelog)
本项目通过敏捷迭代开发完成，主要里程碑如下：

V1.0 - 基础框架: 完成系统时钟与基础串口通信初始化。

V2.0 - 传感接入: 集成 DHT11、MQ-2 传感器底层驱动及 ADC 采样。

V3.0 - 网络打通: 引入 ESP8266 驱动，实现 WiFi 连接。

V4.0 - 协议接入: 移植 MQTT 协议栈。

V5.0 - 平台登录: 完成 OneNET/MQTT 服务器鉴权登录机制。

V6.0 - 数据上云: 实现多路传感器数据 JSON 序列化并成功推送至服务器。

V7.0 - 反向控制: 增加云平台下发命令解析逻辑，实现远程控制 LED。

V8.0 - 界面显示: 接入 OLED 屏幕，实现系统状态可视化。

V9.0 - 移动端联动: 配合 HBuilderX 开发的前端 App，实现 Token 交互与 UI 联动。

V10.0~V11.0 - 架构优化: 剥离平台强耦合，全面适配自建 MQTT 服务器。

注：本仓库包含 Keil .uvprojx 工程，克隆后可直接进行二次开发。编译产生的中间文件已通过 .gitignore 过滤。
<img width="1770" height="1327" alt="IMG20260421213543" src="https://github.com/user-attachments/assets/51ede985-1238-4add-92e8-30b8815709d7" />
