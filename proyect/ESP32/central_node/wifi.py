import network
station = network.WLAN(network.STA_IF)
station.active(True)

if not station.isconnected():
    station.connect('Nicolas1', 'clavejeje')
    print(station.ifconfig())
else:
    print(station.ifconfig())
