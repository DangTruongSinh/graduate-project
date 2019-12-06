import React from 'react';
import ReactDOM from 'react-dom';
import { BrowserRouter, Route, Switch } from 'react-router-dom'
import './index.css';
import Manual from './components/Manual';
import Timer from './components/Timer';
import LandingPage from './components/LandingPage';
import NotFound from './components/NotFound';
import * as serviceWorker from './serviceWorker';
import 'bootstrap/dist/css/bootstrap.css';

ReactDOM.render((
<BrowserRouter>
  <Switch>
    <Route exact path='/' component={LandingPage}/>
    <Route path='/manual' component={Manual} />
    <Route path='/timer' component={Timer} />
    <Route path='*' component={NotFound} />
  </Switch>
</BrowserRouter>
),document.getElementById('root'));

serviceWorker.unregister();
