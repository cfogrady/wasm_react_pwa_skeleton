import React, { Component } from 'react';
import PropTypes from 'prop-types';
import SimpleModal from '../SimpleModal';
import './WebAssemblyLoader.css';

class WebAssemblyLoader extends Component {
    constructor(props) {
        super(props);
        this.waitUntilModuleOnDOM = this.waitUntilModuleOnDOM.bind(this);
        this.runModule = this.runModule.bind(this);
        this.state = {
            initialized: false,
        };
        this.canvasRef = React.createRef();
    }

    runModule() {
        const moduleMethod = window[this.props.moduleName];
        this.module = {
            canvas: (() => this.canvasRef.current)(),
            print: text => console.log(text),
            printErr: text => console.error(text),
        };
        moduleMethod(this.module).then(finishedModule => {
            this.setState({
                initialized: true,
            });
            this.module = finishedModule;
        });
    }

    waitUntilModuleOnDOM() {
        if(window[this.props.moduleName]) {
            this.runModule();
        } else {
            setTimeout(this.waitUntilModuleOnDOM, 250);
        }
    }

    componentDidMount() {
        if(window[this.props.moduleName]) {
            this.runModule();
        } else {
            const script = document.createElement('script');
            script.src = this.props.moduleName+".js";
            document.body.appendChild(script);
            this.waitUntilModuleOnDOM();
        }
    }

    render() {
        return (
            <SimpleModal show={this.state.initialized}>
                <div className="canvas-assembly-container">
                    <canvas ref={this.canvasRef} className="canvas" onContextMenu={event => event.preventDefault()}/>
                    <div className="button-container">
                        <button onClick={this.props.onClose}>Close</button>
                        <button onClick={() => this.module.requestFullscreen(true, false)}>Fullscreen</button>
                    </div>
                </div>            
            </SimpleModal>
        );
  }
}

WebAssemblyLoader.propTypes = {
    moduleName: PropTypes.string.isRequired,
    onClose: PropTypes.func.isRequired,
}

WebAssemblyLoader.defaultProps = {

}

export default WebAssemblyLoader;
