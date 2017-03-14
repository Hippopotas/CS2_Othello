Team Best Group Team's CS2 project.

Richard: implemented original AI and added mobility to AI

Charles: created base for making moves with Player class, optimized the specific numbers used for the AI, debugged

Strategies:
We divided time remaining into  (spaces_remaining / 2) (for each move), and then took log10(time) for the number of layers to use minimax on. This allowed us to use a deeper search if we were allowed more time. If we went over the alloted time, the later turn calculations would use a more shallow search.
Also we changed the heuristic function to implement mobility on early stages of the game since the number of spaces taken does not matter at all until the game is over, so there is no reason to use that as the optimization statistic. Mobility allows the AI to take get more possible moves, which gives more options for the end of the game.
