##  Tencent Iot-Kit for RTthread Package 

### 概述

Tencent Iot-Kit for RTthread Package 是基于[腾讯云C-SDK](https://github.com/tencentyun/tencent-cloud-iotsuite-embedded-c.git)在RThread环境开发的软件包，基于该软件包提供的能力，可以实现腾讯云IOT平台的连接及应用。


## SDK接口说明
以下是C SDK 提供的功能和对应 API，用于设备端编写业务逻辑，API 接口暂不支持多线程调用，在多线程环境下，请勿跨线程调用。 更加详细的接口功能说明请查看 [include/tc_iot_export.h](include/tc_iot_export.h) 中的注释。

### 1. 日志接口

| 序号        | 函数名      | 说明        |
| ---------- | ---------- | ---------- |
| 1          | tc_iot_set_log_level | 设置打印的日志等级 |
| 2          | tc_iot_get_log_level | 返回日志输出的等级 |
| 3          | tc_iot_log_level_enabled | 判断当前等级日志是否打开 |

### 2. MQTT 鉴权接口
| 序号        | 函数名      | 说明        |
| ---------- | ---------- | ---------- |
| 1          | tc_iot_refresh_auth_token | 鉴权模式为动态令牌模式时，通过本接口获取访问 MQTT 服务端动态用户名和密码|

### 3. MQTT 接口

| 序号        | 函数名      | 说明        |
| ---------- | ---------- | ---------- |
| 1 | tc_iot_mqtt_client_construct | 构造 MQTT client，并连接MQ服务器 |
| 2 | tc_iot_mqtt_client_destroy | 关闭 MQTT client 连接，并销毁 MQTT client |
| 3 | tc_iot_mqtt_client_yield | MQTT Client 主循环，包含心跳维持、上行消息响应超时检测、服务器下行消息收取等操作。|
| 4 | tc_iot_mqtt_client_publish | 向指定的 Topic 发布消息 |
| 5 | tc_iot_mqtt_client_subscribe | 订阅指定 Topic 的消息 |
| 6 | tc_iot_mqtt_client_unsubscribe | 取消订阅已订阅的 Topic |
| 7 | tc_iot_mqtt_client_is_connected | 判断 MQTT client 目前是否已连接 |
| 8 | tc_iot_mqtt_client_disconnecd | 断开 MQTT client 与服务端的连接 |

### 4. 数据模板接口

| 序号        | 函数名      | 说明        |
| ---------- | ---------- | ---------- |
| 1 | tc_iot_server_init | 根据设备配置参数，初始化服务。 |
| 2 | tc_iot_server_loop | 服务任务主循环函数，接收服务推送及响应数据。 |
| 3 | tc_iot_server_destroy | 数据模板服务析构处理，释放资源。 |
| 4 | tc_iot_report_device_data | 上报设备数据模板参数最新数据状态，更新到服务端。|
| 5 | tc_iot_confirm_devcie_data | 根据设备控制端要求，发送设备数据模板参数控制指令，更新到服务端，推送给设备。 |

### 5. CoAP 接口

| 序号        | 函数名      | 说明        |
| ---------- | ---------- | ---------- |
| 1 | tc_iot_coap_construct | 根据 CoAP 设备配置参数，初始化服务。 |
| 2 | tc_iot_coap_destroy | CoAP 服务析构处理，释放资源。 |
| 3 | tc_iot_coap_auth | 发起认证，获取后续服务所需的设备 Token。 |
| 4 | tc_iot_coap_send_message | 向服务端发送 CoAP 消息。 |
| 5 | tc_iot_coap_yield | CoAP client 主循环，包含上行消息响应超时检测、服务器下行消息收取等操作。|
| 6 | tc_iot_coap_get_message_code | 获取 CoAP 消息请求或返回码。 |
| 7 | tc_iot_coap_get_message_payload | 获取 CoAP 消息的 Payload 内容。|

### 6. OTA 接口

| 序号        | 函数名      | 说明        |
| ---------- | ---------- | ---------- |
| 1 | tc_iot_ota_construct | 根据设备配置参数，初始化 OTA 服务。 |
| 2 | tc_iot_ota_destroy | OTA 服务析构处理，取消 OTA 消息订阅，释放资源。 |
| 3 | tc_iot_ota_report_firm | 上报设备系统信息，例如，硬件版本、MAC 地址、IMEI、固件版本、SDK 版本等。|
| 4 | tc_iot_ota_report_upgrade | OTA 升级执行过程中，上报固件下载及升级进度。 |
| 5 | tc_iot_ota_request_content_length | 向固件下载服务器，发起 HTTP HEAD 请求，获取固件长度。|
| 6 | tc_iot_ota_download | 根据指定的固件 URL 地址，下载固件 |

### 7. HTTP 接口
| 序号        | 函数名      | 说明        |
| ---------- | ---------- | ---------- |
| 1 | tc_iot_http_mqapi_rpc | 通过 HTTP 接口 调用数据模板数据服务。|



## 移植说明
### 硬件及操作系统平台抽象层（HAL 层）
SDK 抽象定义了硬件及操作系统平台抽象层（HAL 层），将所依赖的内存、定时器、网络传输交互等功能，
都封装在 HAL 层（对应库libtc_iot_hal）中，进行跨平台移植时，首先都需要根据对应平台的硬件及操作系统情况，
对应适配或实现相关的功能。


\src\platform\rtthread
Tencent Iot-Kit for RTthread Package软件包已完成基于Rtthread 平台的移植，移植基于RTthread SAL层提供的BSD 网络编程接口。
相关的头文件及源文件代码结构如下：
```shell
/src/platform
|-- rtthread                 # rtthread平台移植的目录，对应rtthread软件包要求的port目录
    |-- CMakeLists.txt
    |-- tc_iot_hal_net.c    # TCP 非加密直连方式网络接口实现
    |-- tc_iot_hal_os.c     # 延时、打印机时间戳实现
    |-- tc_iot_hal_timer.c  # 定时器相关实现
    |-- tc_iot_hal_tls.c    # TLS 加密网络接口实现
    |-- tc_iot_hal_udp.c    # UDP 接口实现
    |-- tc_iot_hal_dtls.c   # DTLS 加密网络接口实现
	|-- tc_iot_get_time.c   # NTP实现UTC时间获取
```

C-SDK 中提供的 HAL 层是基于 Linux 等 POSIX 体系系统的参考实现，但并不强耦合要求实现按照 POSIX 接口方式，移植时可根据目标系统的情况，灵活调整。

所有 HAL 层函数都在 include/platform/tc_iot_hal*.h 中进行声明，函数都以 tc_iot_hal为前缀。

以下是需要实现的 HAL 层接口，详细信息可以参考注释。

### 基础功能
| 功能分类    | 函数名     | 说明        | 是否可选   |
| ---------- | ---------- | ---------- | ---------- |
| 内存 | tc_iot_hal_malloc | 分配所需的内存空间，并返回一个指向它的指针。 | 基础必选 |
| 内存 | tc_iot_hal_free | 释放之前调用 tc_iot_hal_malloc 所分配的内存空间。 | 基础必选 |
| 输入输出 | tc_iot_hal_printf | 发送格式化输出到标准输出 stdout。 | 基础必选 |
| 输入输出 | tc_iot_hal_snprintf | 发送格式化输出到字符串。 | 基础必选 |
| 时间日期 | tc_iot_hal_timestamp | 系统时间戳，格林威治时间 1970-1-1 00点起总秒数 | 基础必选 |
| 定时器 | tc_iot_hal_sleep_ms | 睡眠挂起一定时长，单位：ms | 基础必选 |
| 定时器 | tc_iot_hal_timer_init | 初始化或重置定时器 | 基础必选 |
| 定时器 | tc_iot_hal_timer_is_expired | 判断定时器是否已经过期 | 基础必选 |
| 定时器 | tc_iot_hal_timer_countdown_ms | 设定定时器时延，单位：ms | 基础必选 |
| 定时器 | tc_iot_hal_timer_countdown_second | 设定定时器时延，单位：s | 基础必选 |
| 定时器 | tc_iot_hal_timer_left_ms | 检查定时器剩余时长，单位：ms | 基础必选 |
| 随机数 | tc_iot_hal_srandom | 设置随机数种子值 | 基础必选 |
| 随机数 | tc_iot_hal_random | 获取随机数 | 基础必选 |

### 网络功能（二选一或全选）
根据实际连接方式选择，如是否走MQTT over TLS加密，是否通过HTTPS接口获取Token等，选择性实现 TCP 或 TLS 相关接口。

#### 基于 MQTT 协议接入
##### TCP

| 功能分类    | 函数名     | 说明        | 是否可选   |
| ---------- | ---------- | ---------- | ---------- |
| TCP 连接 | tc_iot_hal_net_init | 初始化网络连接数据 | 可选，非加密直连时实现 |
| TCP 连接 | tc_iot_hal_net_connect | 连接服务端 | 可选，非加密直连时实现 |
| TCP 连接 | tc_iot_hal_net_is_connected | 判断当前是否已成功建立网络连接 | 可选，非加密直连时实现 |
| TCP 连接 | tc_iot_hal_net_write | 发送数据到网络对端 | 可选，非加密直连时实现 |
| TCP 连接 | tc_iot_hal_net_read | 接收网络对端发送的数据 | 可选，非加密直连时实现 |
| TCP 连接 | tc_iot_hal_net_disconnect | 断开网络连接 | 可选，非加密直连时实现 |
| TCP 连接 | tc_iot_hal_net_destroy | 释放网络相关资源 | 可选，非加密直连时实现 |

##### TLS

| 功能分类    | 函数名     | 说明        | 是否可选   |
| ---------- | ---------- | ---------- | ---------- |
| TLS 连接 | tc_iot_hal_tls_init | 初始化 TLS 连接数据 | 可选，基于TLS加密通讯时实现 |
| TLS 连接 | tc_iot_hal_tls_connect | 连接 TLS 服务端并进行相关握手及认证 | 可选，基于TLS加密通讯时实现 |
| TLS 连接 | tc_iot_hal_tls_is_connected | 判断当前是否已成功建立 TLS 连接 | 可选，基于TLS加密通讯时实现 |
| TLS 连接 | tc_iot_hal_tls_write | 发送数据到 TLS 对端 | 可选，基于TLS加密通讯时实现 |
| TLS 连接 | tc_iot_hal_tls_read | 接收 TLS 对端发送的数据 | 可选，基于TLS加密通讯时实现 |
| TLS 连接 | tc_iot_hal_tls_disconnect | 断开 TLS 连接 | 可选，基于TLS加密通讯时实现 |
| TLS 连接 | tc_iot_hal_tls_destroy | 释放 TLS 相关资源 | 可选，基于TLS加密通讯时实现 |

#### 基于 CoAP 协议接入
##### UDP

| 功能分类    | 函数名     | 说明        | 是否可选   |
| ---------- | ---------- | ---------- | ---------- |
| UDP 连接 | tc_iot_hal_udp_init | 初始化网络连接数据 | 可选，非加密直连时实现 |
| UDP 连接 | tc_iot_hal_udp_connect | 连接服务端 | 可选，非加密直连时实现 |
| UDP 连接 | tc_iot_hal_udp_is_connected | 判断当前是否已成功建立网络连接 | 可选，非加密直连时实现 |
| UDP 连接 | tc_iot_hal_udp_write | 发送数据到网络对端 | 可选，非加密直连时实现 |
| UDP 连接 | tc_iot_hal_udp_read | 接收网络对端发送的数据 | 可选，非加密直连时实现 |
| UDP 连接 | tc_iot_hal_udp_disconnect | 断开网络连接 | 可选，非加密直连时实现 |
| UDP 连接 | tc_iot_hal_udp_destroy | 释放网络相关资源 | 可选，非加密直连时实现 |

##### DTLS

| 功能分类    | 函数名     | 说明        | 是否可选   |
| ---------- | ---------- | ---------- | ---------- |
| DTLS 连接 | tc_iot_hal_dtls_init | 初始化 DTLS 连接数据 | 可选，基于DTLS加密通讯时实现 |
| DTLS 连接 | tc_iot_hal_dtls_connect | 连接 DTLS 服务端并进行相关握手及认证 | 可选，基于DTLS加密通讯时实现 |
| DTLS 连接 | tc_iot_hal_dtls_is_connected | 判断当前是否已成功建立 DTLS 连接 | 可选，基于DTLS加密通讯时实现 |
| DTLS 连接 | tc_iot_hal_dtls_write | 发送数据到 DTLS 对端 | 可选，基于DTLS加密通讯时实现 |
| DTLS 连接 | tc_iot_hal_dtls_read | 接收 DTLS 对端发送的数据 | 可选，基于DTLS加密通讯时实现 |
| DTLS 连接 | tc_iot_hal_dtls_disconnect | 断开 DTLS 连接 | 可选，基于DTLS加密通讯时实现 |
| DTLS 连接 | tc_iot_hal_dtls_destroy | 释放 DTLS 相关资源 | 可选，基于DTLS加密通讯时实现 |


## 软件包使用
### RTthread配置
- RT-Thread env开发工具中使用 `menuconfig` 使能 tencent-iotkit 软件包，配置产品及设备信息，并根据产品需求配置相应选项

```shell
RT-Thread online packages  --->
    IoT - internet of things  --->
        IoT Cloud  --->
			--- Tencent-iotkit:  Tencent Cloud SDK for IoT platform                                                                                               
                   Select Auth Mode (AUTH BY TOKEN)  --->                                                                                              
             (mqtt-1egtdjqza) Config Product Key                                                                                                       
             (rt_thread_dev1) Config Device Name                                                                                                       
             (92b0e676cdd608c4fb56386967613764) Config Device Secret                                                                                   
             [*]   Enable MQTT                                                                                                                         
                     Select MQTT data type (Enable MQTT Advanced)  --->                                                                                
             [ ]     Enable MQTT SSL                                                                                                                   
             [ ]   Enable COAP                                                                                                                         
             [ ]   Enable SCENARIOS                                                                                                                    
             [ ]   Enable HTTP                                                                                                                         
                          Version (latest)  --->
```

- 如果使能TLS功能，还需要选择mbedtls软件包
```shell
RT-Thread online packages  --->
    security packages  --->
         --- mbedtls:An open source, portable, easy to use, readable and flexible SSL library                                                                
		[*]   Store the AES tables in ROM (NEW)                                                                                                             
		(2)   Maximum window size used (NEW)                                                                                                                
		(3584) Maxium fragment length in bytes (NEW)                                                                                                        
		[ ]   Enable a mbedtls client example (NEW)                                                                                                         
		[ ]   Enable Debug log output (NEW)                                                                                                                 
			version (latest)  --->
```

- 使用 `pkgs --update` 命令下载软件包

### 编译及运行
1. 使用命令 scons --target=xxx 输出对应的工程，编译 


2. 执行示例程序：

### 运行demo程序
系统启动后，在 MSH 中使用命令执行：

- 数据模板例程

该示例程序演示了如何基于腾讯云数据模板，和云端实现布尔型、枚举型、数值型、字符串型数据交互，MSH 命令如下所示：

```shell
msh />tc_mqtt_shadow_example
TC MQTT Shadow Exmaple,TC_IOT_SDK_VER:2.6
TRACE tc_iot_server_init:375 [c->s] shadow_get{"method":"get","passthrough":{"sid":"89390000"},"metadata":false}
...
TRACE tc_iot_mqtt_refresh_dynamic_sign:1186 usename[rt_thread_dev1] password[productId=iot-1l60dtl0&nonce=1350022348&timestamp=1539578219&signature=d4bMuKE1NVvE1hjAPiXnXR
request username and password for mqtt success.
...
TRACE tc_iot_shadow_check_and_report:1035 report(first time): "param_bool":false
TRACE tc_iot_shadow_check_and_report:1035 report(first time): "param_enum":0
TRACE tc_iot_shadow_check_and_report:1035 report(first time): "param_number":0.000000
TRACE tc_iot_shadow_check_and_report:1035 report(first time): "param_string":""
TRACE tc_iot_shadow_check_and_report:1114 requesting with: {"method":"update","passthrough":{"sid":"196c0001"},"state":{"reported":{"param_bool":false,"param_enum":0,"param_number":0.000000,"param_string":""}}}
...
TRACE _tc_iot_report_message_ack_callback:47 [s->c] {"method":"reply","timestamp":1539578239,"payload":{"code":0,"status":"OK"},"passthrough":{"sid":"196c0001"}}
...
```

