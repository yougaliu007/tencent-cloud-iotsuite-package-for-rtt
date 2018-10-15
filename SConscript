import os
from building import *
import rtconfig

cwd  = GetCurrentDir()

src  = []
CPPPATH = []
CPPDEFINES = []
LOCAL_CCFLAGS = ''

#Example
if GetDepend(['PKG_USING_TENCENT_IOTKIT_MQTT_ADVANCED']):
	src += Glob('examples/advanced_edition/mqtt/app_main.c')
	src += Glob('examples/advanced_edition/mqtt/tc_iot_device_logic.c')
	CPPPATH += [cwd + '/examples/advanced_edition/mqtt']
	
if GetDepend(['PKG_USING_TENCENT_IOTKIT_MQTT_BASIC']):
	src += Glob('examples/basic_edition/mqtt/demo_mqtt.c')
	CPPPATH += [cwd + '/examples/basic_edition/mqtt']

if GetDepend(['PKG_USING_TENCENT_IOTKIT_COAP_ADVANCED']):
	src += Glob('examples/advanced_edition/coap/coap_app_main.c')
	CPPPATH += [cwd + '/examples/advanced_edition/coap']	

if GetDepend(['PKG_USING_TENCENT_IOTKIT_COAP_BASIC']):
	src += Glob('examples/basic_edition/coap/coap_app_main.c')
	CPPPATH += [cwd + '/examples/basic_edition/coap']

if GetDepend(['PKG_USING_TENCENT_IOTKIT_HTTP']):
	src += Glob('examples/advanced_edition/http/http_rpc_main.c')
	CPPPATH += [cwd + '/examples/advanced_edition/http']   
	
	
if GetDepend(['PKG_USING_TENCENT_IOTKIT_OTA']):
	src += Glob('examples/scenarios/ota/app_main.c')
	src += Glob('examples/scenarios/ota/tc_iot_ota_logic.c')
	CPPPATH += [cwd + '/examples/scenarios/ota'] 
	
if GetDepend(['PKG_USING_TENCENT_IOTKIT_SMARTBOX']):
	src += Glob('examples/scenarios/smartbox/app_main.c')
	CPPPATH += [cwd + '/examples/scenarios/smartbox'] 


#include headfile
CPPPATH += [cwd + '/include']
CPPPATH += [cwd + '/include/iotsuite']
CPPPATH += [cwd + '/include/mqtt']
CPPPATH += [cwd + '/include/http']	
CPPPATH += [cwd + '/include/common']
CPPPATH += [cwd + '/include/coap']
CPPPATH += [cwd + '/include/platform']
CPPPATH += [cwd + '/include/platform/rtthread']	
	
#src/mqtt
src += Glob('src/mqtt/*.c')

#src/iotsuite
src += Glob('src/iotsuite/*.c')

#src/common
src += Glob('src/common/*.c')	

#external/jsmn
src += Glob('external/jsmn/*.c')
CPPPATH += [cwd + '/external/jsmn']	

#external/paho
src += Glob('external/paho.mqtt.embedded-c/MQTTPacket/src/*.c')
CPPPATH += [cwd + '/external/paho.mqtt.embedded-c/MQTTPacket/src']	

#src/platform/rtthread
src += Glob('src/platform/rtthread/*.c')
SrcRemove(src, 'src/platform/rtthread/tc_iot_hal_tls.c')


#TLS used
#if GetDepend(['PKG_USING_TENCENT_IOTKIT_MQTT_TLS']):
#	SrcRemove(src, 'src/platform/rtthread/tc_iot_hal_net.c')   #mbedtls c files to be added

#if GetDepend(['PKG_USING_TENCENT_IOTKIT_COAP_DTLS']):
#	SrcRemove(src, 'src/platform/rtthread/tc_iot_hal_net.c')   #mbedtls c files to be added
  

#CPPDEFINES += ['ENABLE_TC_IOT_LOG_TRACE', 'ENABLE_TC_IOT_LOG_DEBUG', 'ENABLE_TC_IOT_LOG_INFO', 'ENABLE_TC_IOT_LOG_WARN', 'ENABLE_TC_IOT_LOG_ERROR', 'ENABLE_TC_IOT_LOG_FATAL']

group = DefineGroup('tencent-iotkit', src, depend = ['PKG_USING_TENCENT_IOTKIT'], CPPPATH = CPPPATH, LOCAL_CCFLAGS = LOCAL_CCFLAGS, CPPDEFINES = CPPDEFINES)
Return('group')
