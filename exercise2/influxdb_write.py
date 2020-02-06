"""
 # Connect to server &
 # Get the temperature value every 2 sec
 # 
 # @author Lee, Sangheon
 # @version influxdb 1.7.9v
 # @see None
"""

import sys
from influxdb import InfluxDBClient

def main(value):
    host = '127.0.0.1'
    port = 8086
    user = 'tuv'
    password = 'floor29'
    dbname = 'OPC_UA_Data_table'

    client = InfluxDBClient(host, port, user, password)

    if(client.write(['temperature,where=office value=' + value],{'db':dbname},204,'line')):
        print(' * Successful save to influxdb *')
    else:
        print(' * Failed to save to influxdb *')

if __name__ == "__main__":
    # Get the parameter, started this program
    temperature = sys.argv[1]
    
    main(temperature)