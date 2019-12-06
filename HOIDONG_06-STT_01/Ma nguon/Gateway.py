#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" A simple continuous receiver class. """

# Copyright 2015 Mayer Analytics Ltd.
#
# This file is part of pySX127x.
#
# pySX127x is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
# License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# pySX127x is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
# details.
#
# You can be released from the requirements of the license by obtaining a commercial license. Such a license is
# mandatory as soon as you develop commercial activities involving pySX127x without disclosing the source code of your
# own applications, or shipping pySX127x with a closed source product.
#
# You should have received a copy of the GNU General Public License along with pySX127.  If not, see
# <http://www.gnu.org/licenses/>.

from time import sleep
import time
import json
import packer
import datetime
import thread
import MySQLdb
import sys
import numpy as np
sys.path.insert(0, '../../pySX127x')        
from SX127x.LoRa import *
from SX127x.board_config import BOARD
import paho.mqtt.client as mqtt

mode = 0 #use for control (mode = 0 auto mode = 1 manual)
pump1 = 0
pump2 = 0
temp = 0
mistspray = 0
pump1m = 0
pump2m = 0
tempm = 0
mistspraym = 0
templimit = 0
pump1limit = 0
pump2limit = 0
mistspraylimit = 0
fingerid =  ""
fingerid_temp =  ""
camera = "1"
add = "0"
i = 0
# Open database connection
db = MySQLdb.connect("localhost","at24","24012013","sensordata" )

#send data to database
            
def senddata(name,node,value):
    #Get the date and time
    i = datetime.datetime.now()
    # prepare a cursor object using cursor() method
    cursor = db.cursor()
    cursor.execute ("""INSERT INTO sensordata(name,node,datetime,value) values(%s, %s, %s, %s)""", (name,node,i,value));
    db.commit()

def selectuser(value):
    cursor = db.cursor()
    cursor.execute ("""SELECT name FROM username WHERE id = (SELECT MAX(id) FROM username WHERE value = %s)""", (value));
    record = cursor.fetchall()
    return record

def deleteuser(value):
    # prepare a cursor object using cursor() method
    cursor = db.cursor()
    cursor.execute ("""DELETE FROM username WHERE value = %s""", (value));
    db.commit()
    
def sendlog(name,value):
    #Get the date and time
    i = datetime.datetime.now()
    # prepare a cursor object using cursor() method
    cursor = db.cursor()
    cursor.execute ("""INSERT INTO log(name,datetime,value) values(%s, %s, %s)""", (name,i,value));
    db.commit()
    
def userdata(name,value):
    #Get the date and time
    i = datetime.datetime.now()
    # prepare a cursor object using cursor() method
    cursor = db.cursor()
    cursor.execute ("""INSERT INTO username(name,datetime,value) values(%s, %s, %s)""", (name,i,value));
    db.commit()
    
def on_connect(mqttc, obj, flags, rc):
    pass

def on_message(mqttc, obj, msg):
    global vantay
    global mode
    global tempm
    global pump1m
    global pump2m
    global mistspraym
    global templimit
    global pump1limit
    global pump2limit
    global fingerid_temp
    global mistspraylimit
    print(msg.topic+" "+str(msg.qos)+" "+str(msg.payload))
    if(msg.topic == "farm/user"):
        name = json.loads(msg.payload)
        userdata(name['name'],fingerid_temp)
        fingerid_temp = ""
    if(msg.topic == "farm/control/mode"):
        if(str(msg.payload) == "auto"): 
            mode = 1
        elif(str(msg.payload) == "manual"):
            mode = 0
            
    if(mode == 0):
        if(msg.topic == "farm/control/manual"):
            if(str(msg.payload) == "p1on"): 
                pump1m = 1
                print pump1m
            elif(str(msg.payload) == "p1off"): 
                pump1m = 0
                print pump1m
            elif(str(msg.payload) == "p2on"): 
                pump2m = 1
                print pump2m
            elif(str(msg.payload) == "p2off"): 
                pump2m = 0
                print pump2m
            elif(str(msg.payload) == "mson"): 
                mistspraym = 1
                print mistspraym
            elif(str(msg.payload) == "msoff"):
                mistspraym = 0
                print mistspraym
            elif(str(msg.payload) == "ton"):
                tempm = 1
                print tempm
            elif(str(msg.payload) == "toff"):
                tempm = 0
                print tempm
    if(msg.topic == "farm/control/autotemp"):
        templimit = int(msg.payload)
        print templimit
    elif(msg.topic == "farm/control/automs"):
        mistspraylimit = int(msg.payload)
        print mistspraylimit
    elif(msg.topic == "farm/control/autop1"):
        pump1limit = int(msg.payload)
        print pump1limit
    elif(msg.topic == "farm/control/autop2"):
        pump2limit = int(msg.payload)
        print pump2limit
            
def on_publish(mqttc, obj, mid):
    print("mid: "+str(mid))
 
def on_subscribe(mqttc, obj, mid, granted_qos):
    pass
 
def on_log(mqttc, obj, level, string):
    pass

class LoRaRxTx(LoRa):
    def __init__(self, verbose=False):
        super(LoRaRxTx, self).__init__(verbose)
        self._id = "4"
        self.rx_done = False

    def on_rx_timeout(self):
        print("\non_RxTimeout")
        print(self.get_irq_flags())
        
    def on_rx_done(self):
        global fingerid
        global camera
        global mode
        global pump1
        global pump2
        global temp
        global mistspray
        global templimit
        global pump1limit
        global pump2limit
        global mistspraylimit
        global fingerid_temp
        camera = "0"
        add = "0"
        print '----------------------------------'
        self.clear_irq_flags(RxDone=1)
        payload = self.read_payload(nocheck=True)
        data = ''.join([chr(c) for c in payload])
        
        try:
            if len(data) < 45:
                jsonData = json.loads(data) # data from sensor node
                senddata("Temp",jsonData['n'],jsonData['t'])
                senddata("Air Humidity",jsonData['n'],jsonData['ah'])
                senddata("Soil Humidity",jsonData['n'],jsonData['sh'])
                print "do dai chuoi: ", len(data)
                if jsonData['n'] == "1":
                    client.publish("farm/check1","check")
                else:
                    client.publish("farm/check2","check")
                if (mode == 1):
                    if (jsonData['n'] == "1"):
                        if(jsonData['sh'] > pump1limit):
                            pump1 = 0
                        else:
                            pump1 = 1
                        if(jsonData['t'] > mistspraylimit):
                            mistspray = 0
                        else:
                            mistspray = 1
                    elif (jsonData['n'] == "2"):
                        if(jsonData['sh'] > pump2limit):
                            pump2 = 0
                        else:
                            pump2 = 1
                        if(jsonData['t'] > templimit):
                            temp = 0
                        else:
                            temp = 1
                print "Receive:", data
            else:
                #data from camera or fingerprint
                _length, _data = packer.Unpack_Str(data)
                data_temp = _data.replace("'", "\"")
                print data_temp
                print "do dai chuoi: ", len(data)
                jsonData = json.loads(data_temp)
                if jsonData['ty'] == "1":
                    fingerid = jsonData['d']
                    add = jsonData['a']
                    delete = jsonData['del']
                    if add== "1":
                        fingerid_temp = fingerid
                        client.publish("farm/fingerprint",True)
                    elif delete == "1":
                        deleteuser(fingerid)
                    else:
                        try:
                            username = selectuser(fingerid)
                            sendlog(username,fingerid)
                            client.publish("farm/inout",True)
                        except:
                            fingerid_temp = fingerid
                            client.publish("farm/fingerprint","miss")
                            print "This id is not available"
                    print "Receive: data from vantay ", fingerid
                else:
                    camera = jsonData['d']
                    client.publish("farm/camera",True)
                    print "Receive: data from camera ", camera
        except: 
            print "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
            print "Non-hexadecimal digit found..."
            print "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
            print "Receive:", data  
##        for i in range(2):
##            self.set_mode(MODE.STDBY)
##            self.clear_irq_flags(TxDone=1)
##            data = {"id": self._id, "data": packer.ACK}
##            _length, _ack = packer.Pack_Str( json.dumps(data) )
##            ack = [int(hex(ord(c)), 0) for c in _ack]
##            print "ACK:", self._id
##            self.write_payload(ack)                                       
##            self.set_mode(MODE.TX)
##
            # ALOHA(1~3)
##            t1 = i*2 + np.random.random() * 3
##            sleep(t1)
            
        self.reset_ptr_rx()
        self.set_mode(MODE.RXCONT)
        self.rx_done = True

    def start(self):
        global mode
        global pump1
        global pump2
        global temp
        global mistspray
        global pump1m
        global pump2m
        global tempm
        global mistspraym
        global i
        while True:
            print '----------------------------------', pump1, pump2      
            try:
                if mode == 0:   # manual node 1 
                    if(pump1m == 0 and mistspraym == 0):
                        data1 = "0"
                    elif(pump1m == 0 and mistspraym == 1):
                        data1 = "1"
                    elif(pump1m == 1 and mistspraym == 0):
                        data1 = "2"
                    else:
                        data1 = "3"
                elif mode == 1: # auto node 1
                    if(pump1 == 0 and mistspray == 0):
                        data1 = "4"
                    elif(pump1 == 0 and mistspray == 1):
                        data1 = "5"
                    elif(pump1 == 1 and mistspray == 0):
                        data1 = "6"
                    else:
                        data1 = "7"
                if mode == 0: # manual node 2
                    if(pump2m == 0 and tempm == 0):
                        data2 = "0"
                    elif(pump2m == 0 and tempm == 1):
                        data2 = "1"
                    elif(pump2m == 1 and tempm == 0):
                        data2 = "2"
                    else:
                        data2 = "3"
                elif mode == 1: # auto node 2
                    if(pump2 == 0 and temp == 0):
                        data2 = "4"
                    elif(pump2 == 0 and temp == 1):
                        data2 = "5"
                    elif(pump2 == 1 and temp == 0):
                        data2 = "6"
                    else:
                        data2 = "7"
                tx_data = "AT" + data1 + data2
                print tx_data
            except KeyboardInterrupt:
                lora.set_mode(MODE.SLEEP)
                BOARD.teardown()
                print "exit()"
            
            if len(tx_data) < 200:
                self.set_mode(MODE.STDBY)
                self.clear_irq_flags(TxDone=1)
                datalist=list()
                datalist=[]
                i = 0
                for x in tx_data:
                    datalist.insert(i,ord(x))
                    i += 1
                print "data", datalist
                print "Rawinput:", tx_data
                sleep(1)
                self.write_payload(datalist)                                       
                self.set_mode(MODE.TX)
                sleep(.5)
                self.set_dio_mapping([0] * 6)
                sleep(.5)
                self.set_mode(MODE.STDBY)
                sleep(.5)
                self.reset_ptr_rx()
                self.set_mode(MODE.RXCONT)

                for _ in range(t2):
                    sleep(.2)

                    if self.rx_done == True:
                        self.rx_done = False
                        break
            
#
# initialize sx1278
# 
BOARD.setup()
sf = 7
bw = 8
cr = 1
t2 = sf * bw * cr
lora = LoRaRxTx()
##lora.set_mode(MODE.SLEEP)
lora.set_mode(MODE.STDBY)
lora.set_pa_config(pa_select=1)
lora.set_freq(433)
lora.set_spreading_factor(sf)  # 7-12
lora.set_bw(bw)  # 0-9 
lora.set_coding_rate(cr)  # 1-4         
lora.clear_irq_flags(RxDone=1)
lora.clear_irq_flags(TxDone=1)
print(lora)
client = mqtt.Client() #create new instance
client.connect("DATN.mshome.net", 1883, 60) #connect to broker
client.publish("farm/start","check")
mqttc = mqtt.Client()
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
 
mqttc.connect("DATN.mshome.net", 1883, 60)
mqttc.subscribe("farm/user", 0)
mqttc.subscribe("farm/control/mode", 0)
mqttc.subscribe("farm/control/manual", 0)
mqttc.subscribe("farm/control/autotemp", 0)
mqttc.subscribe("farm/control/automs", 0)
mqttc.subscribe("farm/control/autop1", 0)
mqttc.subscribe("farm/control/autop2", 0)
mqttc.loop_start()
client.loop_start()
try: 
    lora.start()
finally:
    lora.set_mode(MODE.SLEEP)
    BOARD.teardown()                    
    print "exit()"

