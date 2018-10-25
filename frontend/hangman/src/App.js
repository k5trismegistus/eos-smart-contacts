// React core
import React, { Component } from 'react';
import { getGame, startGame } from './api'

class Game extends Component {
  render() {
    return (
      <div>
        Game
      </div>
    )
  }
}

class Menu extends Component {
  async startGame() {
    const game = await startGame()
    console.log(game)
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
            <Game /> :
            <Menu />
        }
      </div>
    );
  }
}

export default App;