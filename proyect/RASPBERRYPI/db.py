import sqlite3

base = input('Database name: ')
conexion = sqlite3.connect(base)
cursor = conexion.cursor()

cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
print(cursor.fetchall())

table = input('Table name: ')

for row in cursor.execute('select * from {};'.format(table)):
    print(row)

conexion.close()
