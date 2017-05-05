# A artificial intelligence

## The game

The rule of the game is have as much gold as you can.

### The Board


The board is a six by six square of slot in where to put element.
A slot can only contain one element.

### Item in the game

- Element: This is either one of Mercure, Sulfur, Lead, Iron and Copper,
you don't manipulate element directly in the game but sample.
- Gold: This is just a counter that count the amount of gold you own.
- Catalyzers: This is a counter that count how many time you can change an
element on the board into another element.
- Sample: This is juste a pair of element.
- Cluster: a cluster is a group of element of the same type adjacent
to each other. (It can be thought of a conexe component of the board)

- CatalyzerGiver: An element of the type Mercure or Sulfur.

### A Turn

At the begining of your turn, you receive a sample to place on
your board. And then, you can do any number of these actions in any order
you want:
- Place the sample received
- Transmute any cluster on the board
- Catalyze

And you must, during your turn, give a sample to your opponent

#### Place a sample

When placed the sample must verify some condition
- The two element of the sample must be adjacent to each other
- At least one of the element of the sample must be adjacent to
another element of the same type which where already present on the
board, if there is no element of the same type as one of the
element of the sample the sample can be placed anywhere on the
board

If you don't place a sample before the end of your turn the whole
board will be emptied.

#### Transmute a cluster

When you transmute a cluster, you are given a certain amount of gold
and catalyzers hich depend on the type of element transmuted and the
size of the cluster

- If you transmute a cluster that have a size of one you will receive
`-3` gold
- If the element is a CatalyzerGiver you will receive
`floor((size - 1) / 2` catalyzer and `size - 1` gold
- If the element is not a CatalyzerGiver you will receive `O(size^2)`
gold (I don't remember the number of gold but I remembered that it was
quadratic)

