# OPC UA
This project is an exercise to implement server and client using OPC UA(OPC Unified Architecture) library.
open62541(<http://open62541.org>) library is platform independent. All platform-specific functionality is implemented via exchangeable plugins. Plugin implementations are provided for the major operating systems.  
But, I recommend the Linux OS. In my case, I used Ubuntu 18.04 OS on VMware.  
My exercise programs are based on open62541 ver 1.1. You can download open62541 library from github (<https://github.com/open62541/open62541>)
  
## Prerequisite package download
1. General packages:
```
sudo apt-get install git
sudo apt-get dist-upgrade
```

2. Cmake package:
```
sudo apt-get install cmake cmake-curses-gui
```

3. opcua-client packages:
```
sudo apt-get install build-essential gcc python3-pip qttools5-dev python3-pyqt5
sudo apt install python-pip
sudo apt install python3-pip
sudo pip3 install opcua-client
```

4. Additional package:
```
sudo apt-get install libmbedtls-dev check libsubunit-dev python-sphinx graphviz python-sphinx-rtd-theme
```
  
## Install & Build & Run example server
```
git clone https://github.com/open62541/open62541.git
cd open62541
mkdir build
cd build
cmake ..
ccmake ..
Enable UA_BUILD_EXAMPLES, configure(c) and then generate (g)
make
cd bin/examples
./tutorial_server_firststeps ( Note: Any example server application can be used)
Open new terminal
opcua-client
```
  
Now you can see the server details listed in the client application.
![example_server](https://user-images.githubusercontent.com/34882947/73901032-ede9a380-48d4-11ea-9d92-f98eb370af77.JPG)
  
---------------------------
## Run the exercise1 server
```
git clone https://github.com/lsh940131/opcua.git
cd opcua/exercise1
gcc -std=c99 -o server server_ex1.c -lopen62541
./sever
Open new terminal
opcua-client
```

Now you can see this.
![exercise1_server](https://user-images.githubusercontent.com/34882947/73901088-14a7da00-48d5-11ea-9f06-20bfd39a4094.JPG)
  
---------------------------
## Run the exercise2 server & client
### Introduce
![exercise2_structure](https://user-images.githubusercontent.com/34882947/73901127-31441200-48d5-11ea-8e58-6e66c2214734.JPG)
Exercise2 server updates temperature value every 2 sec,  
Exercise2 client requests the data and gets. then, stores in influxdb.  
The grafana fetches the data from influxdb, then shows the data as a graph.
  
### Influxdb
#### Install
```
wget -qO- https://repos.influxdata.com/influxdb.key | sudo apt-key add -
source /etc/lsb-release
echo "deb https://repos.influxdata.com/${DISTRIB_ID,,} ${DISTRIB_CODENAME} stable" | sudo tee /etc/apt/sources.list.d/influxdb.list
sudo systemctl enable influxdb
sudo systemctl start influxdb
```
  
#### Start & Set up
```
sudo influx
```

```sql
	CREATE USER admin WITH PASSWORD 'admin' WITH ALL PRIVILEGES
	CREATE USER user WITH PASSWORD 'password'
	CREATE DATABASE OPC_UA_Data_table
	USE OPC_UA_Data_table
	INSERT temperature,where=office value=5 (This is inserting data into the table and creating the schema at the same time.)
	GRANT ALL ON "OPC_UA_Data_table" to "user"
	exit
```
```
nano /etc/influxdb/influxdb.conf 
modify "auth-enabled = true" then save & exit
sudo service influxdb restart  
```
From now on, you will need a username and password to access influxdb  
- ex) sudo influx -username 'user' -password 'password'
  
### Grafana
#### Install
```
sudo apt-get update
sudo apt-get upgrade
sudo reboot
sudo wget -q -O - https://packages.grafana.com/gpg.key | apt-key add -
sudo apt-get update
sudo apt-get install grafana
sudo systemctl enabled grafana-server
sudo systemctl start grafana-server
```

#### Start & Set up
1. Open web browser <localhost:3000>  
2. Create your first data source, and fill in InfluxDB Details. 

- ex) database = OPC_UA_Data_table, User = user, Password = password

3. Create your first dashboard  

  
### Run the exercise2 server
```
cd ~/opcua/exercise2
gcc -std=c99 -o server server_ex2.c -lopen62541
./server
```
  
### Run the exercise2 client
```
gcc -std=c99 -o client client_ex2.c -lopen62541
./client
```
  
### Result
![exercise2_server,client](https://user-images.githubusercontent.com/34882947/73901120-2c7f5e00-48d5-11ea-877c-83df9ceb0453.JPG)
In the above picture, the left terminal is server, and the right is client.
  
Now you can easily see the temperature through grafana.
![exercise2_grafana](https://user-images.githubusercontent.com/34882947/73901104-212c3280-48d5-11ea-9d58-f27b13a61641.JPG)