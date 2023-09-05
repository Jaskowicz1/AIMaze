# AI Maze

This is my AI Maze project! I've always been fascinated by AI (only in the form of good use) since I was ~14 so making this definitely made younger me happy.

The code is pretty messy, I won't lie. The only part that isn't is NdArray. The reason it's pretty messy is because I'm still learning C++ so some questionable choices were made whilst writing this.

## How does this work?

This project uses the Q-Learning algorithm to navigate through the grid. This works by assigning each tile as a possible state to the AI and then telling the AI the moves it can perform (generating our Q-Table). From here, our AI has a big choice to make, exploit or explore? With a random number generator and our epsilon, we can pick to either exploit or explore. Exploiting means it will just pick the best action depending on it's state (exploiting the best choice) whereas explore means it will just pick a random action. Then we tell it to do that quite a few times until we're happy and it'll eventually have the best route. Now, how do we get it to pick the best route every time? Well, once it's actually figured out the best route, we can tell it to just pick the numbers from its Q-Table that have the highest value based on its current state (highest value will be the best action from the current state), then it does that action and we move on.

## How does Q-Learning work and what is it?

I wont be documenting that here but if you would like to know, I go more in depth on [My Site](https://jaskowicz.xyz/Projects/AIMaze) about how Q-Learning works and what it is.

## Libraries Used

- [SFML](https://www.sfml-dev.org/index.php)

## Preview

![image](https://github.com/Jaskowicz1/AIMaze/assets/46899449/7f50b915-f7f5-42cf-bbf3-d3a9876aded0)

