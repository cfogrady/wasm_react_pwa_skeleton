import React, { Component } from 'react';
import logo from './logo.svg';
import WebAssemblyLoader from './components/WebAssembly/WebAssemblyLoader';

import './App.css';

class App extends Component {
  constructor(props) {
    super(props);
    this.showModule = this.showModule.bind(this);
    this.closeModule = this.closeModule.bind(this);
    this.state = {
      showModule: false,
    };
  }

  showModule() {
    this.setState({
      showModule: true,
    });
  }

  closeModule() {
    this.setState({
      showModule: false,
    });
  }

  render() {
    return (
      <div className="App">
        <header className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          { this.state.showModule ? (
            <div>
              <WebAssemblyLoader moduleName="SkeletonModule" onClose={this.closeModule}/>
              <p>Loading...</p>
            </div>
          ) : (
            <button onClick={this.showModule}>Run Module</button>
          )}
        </header>
      </div>
    );
  }
}

export default App;
