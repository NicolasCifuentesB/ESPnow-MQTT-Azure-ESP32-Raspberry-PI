import network
from machine import Pin, ADC
from time import sleep
from umqtt.simple import MQTTClient

SERVER = '192.168.178.38'  # MQTT SERVER ADDRESS (raspberry)
CLIENT_ID = 'LDR'
TOPIC = b'luminosity'

client = MQTTClient(CLIENT_ID, SERVER, keepalive=30)
client.connect()

ldr = Pin(15)

while True:
    msg = (b'{0:3.1f}'.format(ldr.value()))
    print('Luminosity: {}'.format(ldr.value()))
    client.publish(TOPIC, msg)
    sleep(2)
