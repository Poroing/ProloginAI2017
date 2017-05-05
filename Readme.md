# A artificial intelligence

## The game

The rule of the game is have as much gold as you can.

### The Board
[2]

The board is a six by six square of slot in where to put [element][1].
A slot can only contain one [element][1].

### Item in the game

- [1]Element: This is either one of Mercure, Sulfur, Lead, Iron and Copper,
you don't manipulate element directly in the game but [sample][3].
- [9]Gold: This is just a counter that count the amount of gold you own.
- [5]Catalyzers: This is a counter that count how many time you can change an
[element][1] on the [board][2] into another [element][1].
- [3]Sample: This is juste a pair of [element][1].
- [4]Cluster: a cluster is a group of element of the same type adjacent
to each other. (It can be thought of a conexe component of the board)

- [10]CatalyzerGiver: An [element][1] of the type Mercure or Sulfur.

### A Turn

At the begining of your turn, you receive a [sample][3] to place on
your board. And then, you can do any number of these actions in any order
you want:
- [Place][6] the [sample][3] received
- [Transmute][8] any [cluster][4] on the [board][2]
- Catalyze

And you must, during your turn, [give][7] a [sample][3] to your opponent

#### Place a sample
[6]
When placed the [sample][3] must verify some condition
- The two [element][1] of the [sample][3] must be adjacent to each other
- At least one of the [element][1] of the [sample][3] must be adjacent to
another [element][1] of the same type which where already present on the
[board][2], if there is no [element][1] of the same type as one of the
[element][1] of the sample the [sample][3] can be placed anywhere on the
[board][2]

If you don't place a [sample][3] before the end of your turn the whole
[board][6] will be emptied.

#### Transmute a cluster
[8]
When you transmute a [cluster][4], you are given a certain amount of [gold][9]
and [catalyzers][5] hich depend on the type of element transmuted and the
size of the [cluster][4]

- If you transmute a [cluster][4] that have a size of one you will receive
`-3` gold
- If the element is a [CatalyzerGiver][10] you will receive
`floor((size - 1) / 2` [catalyzer][5] and `size - 1` [gold][9]
- If the element is not a [CatalyzerGiver][10] you will receive `O(size^2)`
[gold][9] (I don't remember the number of gold but I remembered that it was
quadratic)

