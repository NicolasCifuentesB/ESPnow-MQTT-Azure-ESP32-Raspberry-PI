import network
import dht
from machine import Pin
from time import sleep
from umqtt.simple import MQTTClient

SERVER = '192.168.131.38'  # MQTT SERVER ADDRESS (raspberry)
CLIENT_ID = 'DHT'
TOPIC = b'temperature'

client = MQTTClient(CLIENT_ID, SERVER)  # keepalive=30
client.connect()

sensor = dht.DHT11(Pin(15))

while True:
    sensor.measure()
    print('Temperature: {}'.format(sensor.temperature()))
    client.publish(TOPIC, sensor.temperature())
