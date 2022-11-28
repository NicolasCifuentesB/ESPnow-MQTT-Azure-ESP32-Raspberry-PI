import paho.mqtt.client as mqtt
import sqlite3
import datetime
from azure.iot.device import IoTHubDeviceClient, Message


def iothub_client_init():
    # Create an IoT Hub client
    CONNECTION_STRING = "HostName=iot-redes.azure-devices.net;DeviceId=proyecto;SharedAccessKey=wyfpIyoYF77Wpri5tYeM7nOl+W7k+zyzaVJ9f5KFO34="
    client = IoTHubDeviceClient.create_from_connection_string(
        CONNECTION_STRING)
    return client


def azure_upload(table, value, azure_client):

    try:
        message_json = '{{"From": "Esp32-Pi","To": "Azure",{}: {}}}'.format(
            str(table), value)
        message = Message(message_json)
        print("Sending message: {}".format(message))
        azure_client.send_message(message)
        print("Message successfully sent")

    except KeyboardInterrupt:
        print("Some stop")


def query(table, column, value, cursor, conexion):
    now = datetime.datetime.now()
    print('insert into {} (register_time,{}) values ({},{});'.format(
        table, column, now, value))
    cursor.execute(
        'insert into {} (register_time,{}) values ("{}",{});'.format(table, column, now, value))
    conexion.commit()


def on_connect(client, userdata, flags, rc):
    print('Se conecto con mqtt'+str(rc))
    client.subscribe('temperature')
    client.subscribe('luminosity')
    client.subscribe('hygrometry')


def on_message(client, userdata, msg):
    if msg.topic == 'temperature':
        temperature = str((msg.payload.decode()))
        print(f'Temperatura: {temperature} °C')
        query('Temperature', 'temperature', float(
            temperature), cursor, conexion)
        azure_upload('Temperature', float(temperature), azure_client)
    elif msg.topic == 'luminosity':
        luminosity = str((msg.payload.decode()))
        print(f'Luminosidad: {luminosity}')
        query('Luminosity', 'luminosity', float(luminosity), cursor, conexion)
        azure_upload('Luminosity', float(luminosity), azure_client)
    elif msg.topic == 'hygrometry':
        hygrometry = str((msg.payload.decode()))
        print(f'Hygrometry: {hygrometry} %')
        query('Hygrometry', 'hygrometry', float(hygrometry), cursor, conexion)
        azure_upload('Hygrometry', float(hygrometry), azure_client)


azure_client = iothub_client_init()
conexion = sqlite3.connect('mqtt.db')
cursor = conexion.cursor()

try:
    cursor.execute('''create table Temperature(id integer primary key autoincrement,
    register_time timestamp not null,temperature float not null);''')
    cursor.execute('''create table Luminosity(id integer primary key autoincrement,
    register_time timestamp not null,luminosity float not null);''')
    cursor.execute('''create table Hygrometry(id integer primary key autoincrement,
    register_time timestamp not null,hygrometry float not null);''')
    print('Se creo la base de datos')
except:
    print('Ya existe la base de datos')

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect('192.168.178.38', 1883, 60)

client.loop_forever()

conexion.close()
