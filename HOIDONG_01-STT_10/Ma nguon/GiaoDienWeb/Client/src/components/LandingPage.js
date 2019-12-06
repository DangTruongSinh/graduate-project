import React from 'react'
import {Link } from 'react-router-dom'
import { Button, Col, Row } from 'reactstrap'
import '../theme/landing.css'
const LandingPage = () => {
  return (
    <div className="landingContainer">
      <h1 className="landing-header">Đồ án tốt nghiệp</h1>
      <div className="editorGroup">
        <div className="landing-editors">Sinh viên thực hiện: Trần Nguyễn Hòa Đồng</div>
        <div className="landing-supporter">Giảng viên hướng dẫn: ThS. Trương Quang Phúc</div>
      </div>
      <div className="controlBlockWrapper">
        <h1 className="selectMode">Chọn chế độ điều khiển</h1>
        <Row className="controlBlock">
          <Col sm={{size: 2, offset: 5}}>
            <Link to='/manual'><Button color="primary" size="lg" block>Điều khiển trực tiếp</Button></Link>
          </Col>
          <Col sm={{size: 2, offset: 1}}>
            <Link to='/timer'><Button color="primary" size="lg" block>Hẹn giờ tự động</Button></Link>
          </Col>
        </Row>
      </div>
    </div>
  )
}
export default LandingPage;