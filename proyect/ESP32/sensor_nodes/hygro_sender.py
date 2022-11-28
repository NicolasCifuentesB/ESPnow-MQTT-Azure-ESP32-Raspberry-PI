import network
from machine import Pin, ADC
from time import sleep
from umqtt.simple import MQTTClient

SERVER = '192.168.178.38'  # MQTT SERVER ADDRESS (raspberry)
CLIENT_ID = 'HYM'
TOPIC = b'hygrometry'

client = MQTTClient(CLIENT_ID, SERVER, keepalive=30)
client.connect()

hym = ADC(Pin(34))
relehym = Pin(26, Pin.OUT)
hym.atten(ADC.ATTN_11DB)

while True:
    msg = (b'{0:3.1f}'.format(hym.read()))
    print('Hygrometry: {}'.format(hym.read()))
    client.publish(TOPIC, msg)

    if hym.read() <= 1035 and hym.read() > 900:
        relehym.value(1)
    else:
        relehym.value(0)
    sleep(2)
