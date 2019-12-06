import React, { Component } from 'react';
import { Link } from 'react-router-dom'
import { Button, Row } from 'reactstrap'
import socketIOClient from 'socket.io-client';
import _ from 'lodash';
import Device from './Device';
import '../theme/controller.css'

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
class ControllerComponent extends Component {
  constructor(props) {
    super(props);
    this.state = {
      endPoint: "http://localhost:4004",
      connected: true,
      data: {},
    }
  }
  componentDidMount() {
    const { endPoint } = this.state;
    const socket = socketIOClient(endPoint);
    socket.emit('get');
    socket.on('init data', data => this.setState({data}));
  }
  _publishMessage(message) {
    const socket = socketIOClient(this.state.endPoint);
    socket.emit('change status', message);
  }
  _dataChangeHandler(data) {
    this.setState({data})
  }
  render() {
    const { data } = this.state;
    const socket = socketIOClient(this.state.endPoint);
    socket.on('data change', data => this._dataChangeHandler(data));
    const datas = _.isEmpty(data) ? initData : data;
    return (
      <div className="controller-container">
        <Link to='/'><Button color="danger" outline size="sm">Quay Lại</Button></Link>
        <Row className="device-wrapper">{datas.map((da) => {
            return (
              <Device
              key={da.id}
              sendData = {this._publishMessage.bind(this)}
              data = {da}
            />) 
          })}
        </Row>
      </div>
    );
  }
}

export default ControllerComponent;
