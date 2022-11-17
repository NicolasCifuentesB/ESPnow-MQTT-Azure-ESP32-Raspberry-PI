import network
from machine import Pin, ADC
from time import sleep
from umqtt.simple import MQTTClient

SERVER = '192.168.131.38'  # MQTT SERVER ADDRESS (raspberry)
CLIENT_ID = 'HYM'
TOPIC = b'hygrometry'

client = MQTTClient(CLIENT_ID, SERVER, keepalive=30)
client.connect()

hym = ADC(Pin(34))
hym.atten(ADC.ATTN_11DB)

while True:
    msg = (b'{0:3.1f}'.format(hym.value()))
    print('Hygrometry: {}'.format(hym.value()))
    client.publish(TOPIC, msg)
