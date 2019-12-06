const express = require('express');
const http = require('http');
const socketIO = require('socket.io');
const mqtt = require('mqtt');
const bodyParser = require('body-parser');
const cors = require('cors');
const schedule = require('node-schedule');

const port = process.env.PORT || 4004;
const subTopic = 'esp8266';
const mqttOptions = {
  port: 13098,
  host: 'mqtt://m24.cloudmqtt.com',
  clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
  username: 'test',
  password: '12345678',
  keepalive: 60,
  reconnectPeriod: 1000,
  protocolId: 'MQIsdp',
  protocolVersion: 3,
  clean: true,
  encoding: 'utf8'
};
const initialData = [{
  status: false,
  name: 'Thiết bị điện 1',
  pubMessage: 'sw1_',
  id: 'led1'
}, {
  status: false,
  name: 'Thiết bị điện 2',
  pubMessage: 'sw2_',
  id: 'led2'
}];

var dataCurrent;

function formatData(data) {
  const newData = initialData;
  newData[0].status = data[0] == '1' ? true : false;
  newData[1].status = data[1] == '1' ? true : false;
  return newData;
}

function formatDataChange(data) {
  const newData = dataCurrent && dataCurrent.length > 0 ? dataCurrent : initialData;
  switch (data) {
    case 'sw1_ON':
      newData[0].status = true;
      break;
    case 'sw1_OFF':
      newData[0].status = false;
      break;
    case 'sw2_ON':
      newData[1].status = true;
      break;
    case 'sw2_OFF':
      newData[1].status = false;
      break;
    default:
      break;
  }
  return newData;
}
function isDuplicated(data) {
  const existData = ['get', 'User connected', 'ESP_reconnected', 'sw1_1','sw1_0', 'sw2_1', 'sw2_0'];
  const dupData = existData.find((element) => {
    return element == data
  }); 
  return dupData ? true : false
}
function runAfterSeconds(time, message, pubMessage) {
  const status = parseInt(message, 10) == 1 ? '1' : '0';
  const sendMessage = `${pubMessage}${status}`;
  const setSchedule = schedule.scheduleJob(time, ()=>{
   client.publish(subTopic, sendMessage);
  })
 }

const app = express();
app.use(cors());
app.use(bodyParser.urlencoded({extended: true}));
const client = mqtt.connect('mqtt://m24.cloudmqtt.com', mqttOptions);
const server = http.createServer(app);

const io = socketIO(server);

client.on('connect', ()=> {
  client.subscribe(subTopic, (err)=> {
    if(!err) {
      console.log("Connected");
      client.publish(subTopic, 'get');
    }
  });
});

client.on('message', function (topic, message) {
  const data = message.toString();
  console.log(`Recived from ${topic} : ${data}`);
  if(!isDuplicated(data)) {
    if(data.length <= 3) {
      dataCurrent = formatData(data);
      io.sockets.emit('init data', dataCurrent);
    } else {
      dataCurrent = formatDataChange(data);
      console.log(dataCurrent);
      io.sockets.emit('data change', dataCurrent);  
    }
  }
});

io.on('connection', socket => {
  socket.on('change status', (message) => {
    console.log(`Send ${message}`);
    client.publish(subTopic, message);
  });
  socket.on('get', () => {
    client.publish(subTopic, 'get');
  });
  socket.on('timer mode', (data) => {
    const { time, message, pubMessage } = data;
    console.log({time, message, pubMessage});
    
    runAfterSeconds(time, message, pubMessage);
  });
  socket.on('disconnect', ()=>{
    console.log('User disconnected');
  });
});

server.listen(port, ()=> console.log(`Server is listen in port ${port}`));