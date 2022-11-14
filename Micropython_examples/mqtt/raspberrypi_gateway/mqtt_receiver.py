import paho.mqtt.client as mqtt

def on_connect(client,userdata,flags,rc) :
	print('Se conecto con mqtt'+str(rc))
	client.subscribe('temp_humidity')

def on_message(client,userdata,msg) :
	if msg.topic == 'temp_humidity' :
		temperature,humedity = str((msg.payload.decode())).split(',')
		print(f'Temperatura es: {temperature} °C')
		print(f'Humedad es: {humedity} %')
		print(f'Luminosidad: {light}')
		print(f'Humedad en tierra: {ground} %')
	print(msg.topic + ' ' + str(msg.payload.decode()))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect('xxx.xxx.xxx.xxx',1883,60) # Raspberry pi ip

client.loop_forever()

conexion.close()