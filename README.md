 # Backgammon
Backgammon game for console

> *Note that minimum required window size should be 90x24 symbols and 49x24 when game started*

The game has 3 options
- ###### Local 2 player
- ###### Network 2 player
- ###### Play with computer
![image](https://user-images.githubusercontent.com/39254771/197335237-634dd0e5-c491-401a-968f-e36a3162bbb5.png)


Input
- '<' to exit game
- 'a' to start local game
- 'b' to start network game
- 'c' to start game with computer


# Local
2 player mode in single computer.
![image](https://user-images.githubusercontent.com/39254771/197335222-abfca6e2-7a8d-4498-8ca2-98b2bd99affc.png)

After starting local game, dices will be thrown and random player will be selected. Current player is shown in top of the board.
- '⛂' symbol used for white player
- '⛀' symbol used for black player

In this case white player is selected('⛂'). It means that white player already can throw dice and start the game. To do that, white player should input any text.

Player threw 4-4.

![image](https://user-images.githubusercontent.com/39254771/197336823-3349bb40-a9e9-4ca8-9f77-8e4b4f2d4433.png)

'🞙' shows all possible moves.

Player should input the pip, from which wants to take a peace. In this case player can take only from pip 'a' and set on pips 'i' or 'e'.

After selecting a pip, '⮟' will show outgoing pip, and '⮝' will show all possible pips for selected('a') pip.

> *If you took the peace by mistake, you can input 0 to release and take another one*

![image](https://user-images.githubusercontent.com/39254771/197337193-b8239745-6272-4b4f-952b-692693f7a67b.png)

Now player can select the incoming pip(where to place selected peace).

Player chose the pip 'i'

![image](https://user-images.githubusercontent.com/39254771/197337644-5a7ea9bc-d085-4711-b1b3-924f1d987ad1.png)

Since player threw identical dice and can't play 4 times, player can take another peace from head(pip 'a').

And the player does that.

![image](https://user-images.githubusercontent.com/39254771/197337929-077769da-10dd-4e17-8610-0e8b42c96a5f.png)

Now, when there is no available moves, player can commit moves by inputting 1 or if player does mistake, can input 0 to cancel all moves.

After committing moves, current player will be changed, board will be rotated for black player, and the dice will be threw.

![image](https://user-images.githubusercontent.com/39254771/197338188-fa14eb67-3f1c-49e2-b0c0-b7ab9fe6fc04.png)

Now current player is black and dice are 6-3. So player can do moves "6 and 3"
, "3 and 6" or "9"

Player chose pip 'a' and can see all possible pips for selected peace ('d', 'g', 'j').

![image](https://user-images.githubusercontent.com/39254771/197338447-9b30790e-1331-49de-b378-ce8ce56bb2e3.png)

Suppose player set peace on pip 'd'

![image](https://user-images.githubusercontent.com/39254771/197338527-e3e781dc-2f3e-4190-ab61-7269fdab116e.png)

Now there is only one available pip, where player can set a peace, and it is pip 'j'. But player can't take another peace from head(pip 'a'), so player should play peace from pip 'd'.

![image](https://user-images.githubusercontent.com/39254771/197338658-fc266f0e-99b5-4258-927b-32c79b96b24d.png)

We can notice that selected peace is in pip 'd', and there is only one available pip, where selected peace can be set.

And the player does that.

![image](https://user-images.githubusercontent.com/39254771/197338795-676626c0-a23f-4c5f-b762-4cbe4c5782e5.png)

As we know, the player can commit or cancel all moves.


After committing moves, the cycle repeats.

Now there are much more possible moves ('🞙').

![image](https://user-images.githubusercontent.com/39254771/197338832-a2d7a42a-e057-45ea-a8fd-7020c23d2603.png)

Suppose player chose peace on pip 'i'

There are 3 available pips for selected peace ('j', 'k', 'l')

![image](https://user-images.githubusercontent.com/39254771/197338977-917aac1c-c7c7-47d9-9a8f-91298b501e8f.png)

Player did move from pip 'i' to 'l' (3 moves). And have to play 1 move too.

![image](https://user-images.githubusercontent.com/39254771/197339082-553ccedf-41ed-4ab4-ad6c-616a0739f22e.png)



> *Note that play can't block opponent by closing six or more contiguous pips, if opponent has not been in that field-quarter yet. So player should give an opportunity to opponent to enter that field-quarter at least once*

In this example black player has move from pip 'g' to 'l', but can't play, because player will block
opponent to enter top-left field-quarter. Black player gives opportunity to white player to move from pip 'f' to 'l'.

![image](https://user-images.githubusercontent.com/39254771/197339638-c595ac5f-fb61-4574-baa1-36ba26951327.png)

In this case white player can't play dice 1 moving from pip 'i' to 'j'.

![image](https://user-images.githubusercontent.com/39254771/197339792-a89ec279-28ee-40b7-8dfc-9ddae31a1393.png)

In this case black player can close the pip 'f' and block white player, because white player already been in top-right field-quarter

![image](https://user-images.githubusercontent.com/39254771/197339998-837e049f-5de8-4f10-b270-9bac3222c1bb.png)

Of course black player moves from pip 'a' to 'f' and blocks white player. This is a very good chance for black player to win by mars.

![image](https://user-images.githubusercontent.com/39254771/197340092-e76fd943-ad0b-4f7b-9aff-fa533bf491a0.png)



When player moved all pieces to home(bottom-right field-quarter), can start bearing off(removing) paces

![image](https://user-images.githubusercontent.com/39254771/197340902-bc99d383-254b-4722-908e-6c9c0dbde377.png)

Selected peace on pip 't', and that peace can be removed by inputting 'z'

![image](https://user-images.githubusercontent.com/39254771/197340945-21e95892-c091-48d4-ace9-ca7d44ece822.png)

After bearing off(removing) peace, it will appear in right column

![image](https://user-images.githubusercontent.com/39254771/197341023-81e8f6c7-096b-4652-90b3-135b88580885.png)


In this case player doesn't have peace in pip 's', so there is no move for 6 dice. For that player can bear off(remove) peace from pip 't', playing dice 6.
So if pip, that matching with one of dice, is empty, player can bear off(remove) from the highest pip, that is smaller than matching pip.

![image](https://user-images.githubusercontent.com/39254771/197341262-a2ea442a-2474-4812-8e63-c8691d1adeee.png)

Current player removed peaces appearing in right column, and opponent removed peaces in left column.

![image](https://user-images.githubusercontent.com/39254771/197341672-e43e272c-3640-473f-a792-a6720ae60cc7.png)

After game over, players can see the winner peace.

![image](https://user-images.githubusercontent.com/39254771/197343519-0b5e07de-f53a-4690-b905-9143f9287eb8.png)

---
# Network
## Server

To start a server run ./server script.

Select max number of rooms and server running port.

![image](https://user-images.githubusercontent.com/39254771/197347204-bbb014ce-f02a-46dd-a0af-8d76b7ee78ed.png)

Now server running on localhost. But we can use ngrok tool to play outside local network.

Run "ngrok tcp 5555" to tunnel localhost:5555

![image](https://user-images.githubusercontent.com/39254771/197348758-b317b3bb-1f46-4dbf-b123-3291a90bdebd.png)

Now we can use "0.tcp.ngrok.io" as host name and "16948" as port to connect to our local server from everywhere.

## Client 1

Select "Network 2 player" option in main menu to start client side.

![image](https://user-images.githubusercontent.com/39254771/197347405-9cec4e16-ed05-4061-9586-ec782d41c8ff.png)

At first, we should choose a host, so select option 'a' to input a host.

As mentioned above, input "0.tcp.ngrok.io" in host field and "16948" in port field.

![image](https://user-images.githubusercontent.com/39254771/197348977-e5b392b2-9fb2-4e8f-84b8-09865ac32c70.png)

Now selected host:port is appearing in top part of menu (0.tcp.ngrok.io:16948).

![image](https://user-images.githubusercontent.com/39254771/197349072-e0baf519-dd0e-49d3-bd0c-c88140268074.png)


After selecting host, there are 2 options.
- b) Create a room
- c) Connect to room

One of the players should create a room.

Input the room name and password.

![image](https://user-images.githubusercontent.com/39254771/197349349-db75aafa-72c8-4770-a841-dcf37a29860d.png)

Now we should wait for opponent to connect...

![image](https://user-images.githubusercontent.com/39254771/197349668-a7c84697-b6a6-4f31-a4e5-c251bca2e8d2.png)

## Client 2

As a client 2, select "Choose a server" option and input host data (Host: 0.tcp.ngrok.io - Port: 16948).
After, select "Connect to room" option, input room name and password provided by client 1.
And game starts. Server selects random player.

![image](https://user-images.githubusercontent.com/39254771/197349870-86b69178-770d-467b-8689-20014c8897f1.png)

Player can play like local game.

And move will appear in opponent board.

![image](https://user-images.githubusercontent.com/39254771/197350439-8fa2d33d-22e5-4b52-a313-07e9b32f6df7.png)

After committing moves. Player will be changed and dice will be threw automatically. Now second player can play.

![image](https://user-images.githubusercontent.com/39254771/197350497-cee7476c-4e0a-4e02-bc0c-95c586a10ea5.png)

After the few moves...

![image](https://user-images.githubusercontent.com/39254771/197351521-0af705bd-6a70-40a7-9e74-6a579fd1cd08.png)


---
# Play with computer
Not implemented yet


---
> # Thank you for playing this game
