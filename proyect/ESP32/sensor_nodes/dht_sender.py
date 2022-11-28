import network
import dht
from machine import Pin
from time import sleep
from umqtt.simple import MQTTClient

SERVER = '192.168.178.38'  # MQTT SERVER ADDRESS (raspberry)
CLIENT_ID = 'DHT'
TOPIC = b'temperature'

client = MQTTClient(CLIENT_ID, SERVER, keepalive=30)  #
client.connect()

sensor = dht.DHT11(Pin(15))
reletemp = Pin(26, Pin.OUT)

while True:
    sensor.measure()
    temp = sensor.temperature()
    msg = (b'{0:3.1f}'.format(temp))
    print('Temperature: {}'.format(temp))
    client.publish(TOPIC, msg)
    if temp <= 20 and hym.read() > 5:
        reletemp.value(1)
    else:
        reletemp.value(0)
    sleep(2)
