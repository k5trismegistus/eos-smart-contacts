// React core
import React, { Component } from 'react';
import { startGame, getGame, submitChar } from './api'

class Game extends Component {
  constructor(props) {
    super(props)
    this.state = { game: null, val: '' }
  }

  async componentDidMount() {
    const game = await getGame(this.props.gameKey)
    this.setState(Object.assign({}, this.state, { game }))
  }

  changeVal(e) {
    this.setState(Object.assign({}, this.state, { val: e.target.value }))
  }

  async submit() {
    await submitChar(this.state.game.key, this.state.val)
    const game = await getGame(this.props.gameKey)
    this.setState(Object.assign({}, this.state, { game, val: '' }))
  }

  render() {
    if (!this.state.game) {
      return (<h1>Loading</h1>)
    }

    if (this.state.game.status === 1) {
      return (<h1>Cleard!!</h1>)
    }

    if (this.state.game.status === 2) {
      return (<h1>Failed!!</h1>)
    }

    return (
      <div>
        <h1>
          Game id: {this.props.gameKey}
        </h1>

        <p>
          You have {this.state.game.remainingTrial} chances
        </p>

        {
          this.state.game ?
            <div>
              <p>
                {this.state.game.masked}
              </p>
              <p>
                You have submitted: {
                  this.state.game.usedChars
                    .sort()
                    .map((ascii) => String.fromCharCode(ascii))
                }
              </p>
            </div>
            :
            null
        }

        <input type="text" maxLength={1} value={this.state.val} onChange={(e) => this.changeVal(e)} />
        <button onClick={() => this.submit()}>
          submit
        </button>
      </div>
    )
  }
}

class Menu extends Component {
  async startGame() {
    const gameKey = await startGame()
    this.props.setGame(gameKey)
  }

  render() {
    return (
      <div>
        <a href="#" onClick={() => {this.startGame()}}>
          ゲームをはじめる
        </a>
      </div>
    )
  }
}

class App extends Component {

  constructor(props) {
    super(props)
    this.state = { currentGameId: null}
  }

  render() {
    return (
      <div>
        {
          this.state.currentGameId ?
            <Game 
              gameKey={this.state.currentGameId}
            /> :
            <Menu
              setGame={(gameKey) => { this.setState({ currentGameId: gameKey }) }}
            />
        }
      </div>
    );
  }
}

export default App;