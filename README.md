# AI Maze

This is my AI Maze project, you can also play it by yourself instead of the AI if you wish!

## How does this work?

This project uses the Q-Learning algorithm to navigate through the grid. This works by assigning each tile as a possible state to the AI and then telling the AI the moves it can perform, this then generates our Q-Table. From here, our AI has a big choice to make, exploit or explore? With a random number generator and our epsilon, we can pick to either exploit or explore. Exploiting means it will just pick the best action depending on it's state whereas explore means it will just pick a random action. Then we tell it to do that quite a few times until we're happy and it'll eventually have the best route. Now, how do we get it to pick the best route every time? Well we just always exploit rather than explore.

## Libraries Used

- [SFML](https://www.sfml-dev.org/index.php)
- [NumCpp](https://github.com/dpilger26/NumCpp)
- [Boost](https://www.boost.org) | Required by NumCpp.

## Commands

- Coming soon...

![image](img)
