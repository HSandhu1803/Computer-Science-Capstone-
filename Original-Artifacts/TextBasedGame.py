# A dictionary for the Deadly Zombie Text Adventure Game
# The dictionary links a room to other rooms.
rooms = {
        'Reception': {'East': 'Pharmacy', 'North': 'Library', 'South': 'Cafeteria', 'item': None},
        'Pharmacy': {'North': 'Nursing_Station', 'East': 'Doctor_Room', 'South': 'X-ray_Room','West': 'Reception', 'item': 'Health-Aid-Kit'},
        'Nursing_Station': {'East': 'Store_Room', 'South': 'Pharmacy', 'item': 'Injection'},
        'Store_Room': {'West': 'Nursing_Station', 'item': 'Gun'},
        'Doctor_Room': {'North': 'Surgical_Room', 'West': 'Pharmacy', 'item': 'Armour'},
        'Surgical_Room': {'South': 'Doctor_Room', 'item': 'Knife'},
        'X-ray_Room': {'East': 'Basement', 'North': 'Pharmacy', 'item': 'Flash-Light'},
        'Basement': {'West': 'X-ray_Room', 'item': 'Zombie'},
        'Library': {'South': 'Reception', 'West': 'Emergency_Exit', 'item': 'Magic-Potion'},
        'Cafeteria': {'North': 'Reception', 'item': 'Keycard'},
        'Emergency_Exit': {'East': 'Library', 'item': 'Night-Vision-Glass'}

}

# Puzzles that need to be solved to access certain rooms or items
puzzles = {
        'Pharmacy': 'What is 99 - 34?',
        'Surgical_Room': 'What is the capital of France?',
        'Emergency_Exit': 'What is the square root of 16?'
}

# Correct answers for the puzzles
puzzle_answers = {
        'Pharmacy': '65',
        'Surgical_Room': 'Paris',
        'Emergency_Exit': '4'
}

# User-defined function to display basic instructions
def show_instructions():
    print("\n--Deadly Zombie Text Adventure Game--\n")
    print("Collect 8 items to win the game, or be chewed by the zombie.")
    print("Move commands: go South, go North, go East, go West")
    print("Add to Inventory: get 'item name'.")


# To Show Current status of player's room
def show_status(present_room, stock):
    print("\n--------------------------------------")
    print("You are in the", present_room)
    print("Inventory:", stock)
    if (rooms[present_room]['item'] is not None) and (rooms[present_room]['item'] not in stock):
        print('You see a', rooms[present_room]['item'])


#  To retrieve an item, seen in the current room.
def get_item(present_room, stock):
    if rooms[present_room]['item'] not in stock:
        outcome = False
        while not outcome:
            command = input('Enter your move:').split()
            move = command[0]
            if move != 'get':  # Check first word of command i.e. 'get' item
                print('Invalid Move')
                continue

            item = command[1]
            if item == rooms[present_room]['item']: # Check whether item is present in room
                outcome = True
                inventory.append(item)
                print(item, 'Retrieved!')
                show_status(present_room, stock)
            else:
                print('Can\'t get item!!')


# Checking whether script is running directly or it has been imported.
if __name__ == "__main__":

    inventory = []      # Create empty list

    # initialize starting room
    current_room = 'Reception'
    result = False

    show_instructions()                     # Function Call
    show_status(current_room, inventory)    # Function Call

    while not result:  # repeat until user's has either Won or lost

        # Exit Condition: Lost the game
        if rooms[current_room]['item'] == 'Zombie':
            result = True
            print("\nGAME OVER, You lost the game.")
            print("Thanks for playing the game. Hope you enjoyed it.")
            continue

        # Exit Condition: Won the game
        if len(inventory) == 8:
            result = True
            print("\nHURRAY! You have collected all the eight items.")
            print("You won the game!!")
            print("Thanks for playing the game. Hope you enjoyed it.")
            continue

        entry = input('Enter your move:').split()

        if len(entry) == 1:
        # No command is of single word, it is either 'go direction' or 'get item'
            print('Invalid Move')

        elif len(entry) == 2:
            move1 = entry[0]
            move2 = entry[1]

            # first word other than go is 'Invalid'
            # Nested IF statement
            if move1 != 'go':
                print('Invalid Move!')
                continue

            # Checking directions??
            # Nested if..else statement
            if move2 in rooms[current_room]:
                # Puzzle check
                if rooms[current_room][move2] in puzzles:
                    print(puzzles[rooms[current_room][move2]])
                    answer = input()
                    if answer != puzzle_answers[rooms[current_room][move2]]:
                        print('Wrong answer! You can\'t go that way.')
                        continue

                # Update Current Room
                current_room = rooms[current_room][move2]
                # Function Call
                show_status(current_room, inventory)

                if current_room == 'Basement' or current_room == 'Reception':      # Villain's Room or starting point
                    continue
                elif current_room == 'Emergency_Exit':
                    print('\nDanger!! Wear Night-vision glasses and run away....')
                    print('\nEmergency-Exit, Good Bye!!')
                    result = True
                else:
                    get_item(current_room, inventory)
            else:
                print("You can't go that way!")
        else:
            print('Invalid Move!')
