from machine import Pin
from time import sleep
import dht
from umqtt.simple import MQTTClient

SERVER = 'xxx.xxx.xxx.xxx' # MQTT SERVER ADDRESS (raspberry pi ip)
CLIENT_ID = 'ESP32_DHT'
TOPIC = b'temp_humidity'

client = MQTTClient(CLIENT_ID, SERVER)
client.connect()

sensor = dht.DHT11(Pin(15))

while True:
  try:
    sensor.measure()
    temperature = sensor.temperature()
    humedity = sensor.humidity()
    msg = (b'{0:3.1f},{1:3.1f}'.format(temperature,humedity))
    client.publish(TOPIC, msg)
    print(msg.decode())
    sleep(2)
  except OSError as e:
    print('Error al leer el sensor.')
    sleep(2)