import React, { Component } from 'react'
import socketIOClient from 'socket.io-client';
import DatePicker from 'react-datepicker';
import _ from 'lodash';
import { Button, Input, Row, Col, Alert, FormGroup, Label } from 'reactstrap'
import { Link } from 'react-router-dom';
import "react-datepicker/dist/react-datepicker.css";
import { setHours } from 'date-fns';
import { setMinutes } from 'date-fns/esm';
import '../theme/timer.css'

const initData = [{
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

export default class ManageComponent extends Component {
  constructor(props) {
    super(props);
    this.state = {
      time: new Date(),
      endPoint: "http://localhost:4004",
      date: {
        led1: new Date(),
        led2: new Date()
      },
      intervalTime: {
        led1: 15,
        led2: 15
      },
      data: {},
      show: {
        led1: false,
        led2: false
      },
      success: {
        led1: false,
        led2: false
      },
      mode: {
        led1: 'turnOn',
        led2: 'turnOn'
      },
    }
  }
  componentDidMount() {
    const { endPoint } = this.state;
    const socket = socketIOClient(endPoint);
    socket.emit('get');
    socket.on('init data', data => this.setState({data}));
  }
  _handleChange(id, date) {
    this.state.date[id] = date;
    return this.setState({...this.state})
  }
  _setIntervalTime(id, event) { 
    const intervalTime = parseInt(event.target.value) > 0 ? parseInt(event.target.value) : 1;
    this.state.intervalTime[id] = intervalTime;
    return this.setState({...this.state});
  }
  _sendMessage(id, pubMessage){
    const socket = socketIOClient(this.state.endPoint);
    const data = {
      "time": this.state.date[id],
      "pubMessage": pubMessage,
      "message": this.state.mode[id] == 'turnOn' ? '1' : '0'
    }
    console.log(data);
    
    if(this.state.date[id] < Date.now()) {
      this.state.show[id] = true;    
      this.state.success[id] = false;
      return this.setState({...this.state});
    }
    socket.emit("timer mode", data);
    this.state.show[id] = true;    
    this.state.success[id] = true;
    return this.setState({...this.state});
  }
  _dataChangeHandler(data) {
    this.setState({data})
  }
  _selectModeHandle(id, event) {
    const mode = event.target.value;
    this.state.mode[id] = mode;
    return this.setState({...this.state});
  }
  render() {
    const { data} = this.state;
    const customData = _.isEmpty(data) ? initData : data;
    const socket = socketIOClient(this.state.endPoint);
    socket.on('data change', data => this._dataChangeHandler(data));  
    return (
      <div className='timercontroller-wrapper'>
      <Link to='/'><Button color="danger" outline size="sm">Quay Lại</Button></Link>
      <Row>
        { customData.map((da) => {
          return this._renderTimerDevice(da);
        })}
      </Row>
      </div>
    )
  }
  _renderTimerDevice(data) {
    const { intervalTime } = this.state;
    return (
    <Col key={data.id} sm={{size: 6}}>
    <Row>
      <Col>
        <div className="device-name">{data.name}</div>
        <div className={`${data.pubMessage}-icon-2nd-${data.status}`}/>
        {this._renderStatus(data)}
      </Col>
    </Row>
    <Row>
      <Col sm={{size: 8, offset: 2}}>
        <DatePicker
          key={data.id}
          placeholderText="Select time"
          dateFormat="dd/MM/yyyy HH:mm"
          selected={this.state.date[data.id]}
          onChange={this._handleChange.bind(this, data.id)}
          timeIntervals={intervalTime[data.id]}
          minDate={new Date()}
          className="custome-form"
          minTime={setHours(setMinutes(new Date(), this.state.time.getMinutes()), this.state.time.getHours())}
          maxTime={setHours(setMinutes(new Date(), 59), 23)}
          withPortal
          isClearable={true}
          showTimeSelect
          showMonthDropdown
          useShortMonthInDropdown
        />
      </Col>
    </Row>
    <Row>
      <Col sm={{size: 8, offset: 2}}>
        <FormGroup>
          <Label for="setIntervalTime">Chọn khoảng cách các mốc thời gian (phút)</Label>
          <Input type="number" id="setIntervalTime" value={intervalTime[data.id]} onChange={this._setIntervalTime.bind(this, data.id)} />
        </FormGroup>
      </Col>
    </Row>
    <Row>
      <Col sm={{size: 8, offset: 2}}>
        <FormGroup>
          <Label for="setMode">Lựa chọn hành động (Bật/ Tắt)</Label>
          <Input type="select" id="setMode" onChange={this._selectModeHandle.bind(this, data.id)}>
            <option value='turnOn'>Hẹn giờ bật</option>
            <option value='turnOff'>Hẹn giờ tắt</option>
          </Input>            
        </FormGroup>
      </Col>
    </Row>
    <Row>
      <Col sm={{size: 8, offset: 2}}>
        <Button color="primary" size="lg" block onClick={this._sendMessage.bind(this, data.id, data.pubMessage)}>Xác nhận hẹn giờ</Button>
      </Col>
    </Row>
    {this.state.show[data.id] && this._renderMessageBox(this.state.success[data.id], data.id)}
    </Col>)
  }
  _renderMessageBox(status, id){
    setTimeout(()=>{
      this.state.show[id] = false
      return this.setState({...this.state})
    }, 3000);
    return status ? (
      <Col>
          <Alert color="success">
            Hẹn giờ thành công
          </Alert>
      </Col>
    ) : (
      <Col>
          <Alert color="danger">
            Hẹn giờ không thàn công. Vui lòng chọn thời gian sau thời điểm hiện tại!
          </Alert>
      </Col>
    )
  }
  _renderStatus(data) {
    return <div className="device-status">Trạng thái: {data.status ? 'Mở' : 'Tắt'}</div>
  }
}
