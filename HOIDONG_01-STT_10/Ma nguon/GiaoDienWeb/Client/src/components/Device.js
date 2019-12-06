import React, { Component } from 'react'
import { Button, Col, Row } from 'reactstrap'
import '../theme/light.css'

export default class Device extends Component {
  render() {
    const { data } = this.props;
    return (
      <Col sm={{size: 5,offset: 1}}>
        <Row>
          <Col>
              <div className="device-name">{data.name}</div>
              <div className={`${data.pubMessage}-icon-${data.status}`}/>
              {this._renderStatus(data)}
          </Col>
        </Row>
        <Row>
          <Col sm={{size: 8, offset: 2}}>
            { this._renderButton(data.status)}
          </Col>
        </Row>
      </Col>
    )
  }
 _turnOnHandler(){
  const message = `${this.props.data.pubMessage}1`
  this.props.sendData(message);
 }
 _turnOffHandler(){
  const message = `${this.props.data.pubMessage}0`
  this.props.sendData(message);
 }

 _renderButton(status) {
  return status ? 
  (<Button onClick={this._turnOffHandler.bind(this)} color="danger" block>Tắt thiết bị</Button>) :
  (<Button onClick={this._turnOnHandler.bind(this)} color="success" block>Bật thiết bị</Button>)  
 }
 _renderStatus(data) {
    return <div className="device-status">Trạng thái: {data.status ? 'Mở' : 'Tắt'}</div>
 }
}
