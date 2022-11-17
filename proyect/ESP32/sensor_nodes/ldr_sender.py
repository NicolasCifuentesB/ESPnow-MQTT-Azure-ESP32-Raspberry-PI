import network
from machine import Pin, ADC
from time import sleep
from umqtt.simple import MQTTClient

SERVER = '192.168.131.38'  # MQTT SERVER ADDRESS (raspberry)
CLIENT_ID = 'LDR'
TOPIC = b'luminosity'

client = MQTTClient(CLIENT_ID, SERVER)  # keepalive=30
client.connect()

ldr = ADC(Pin(34))
ldr.atten(ADC.ATTN_11DB)

while True:
    sensor.measure()
    print('Luminosity: {}'.format(ldr.value()))
    client.publish(TOPIC, ldr.value())
