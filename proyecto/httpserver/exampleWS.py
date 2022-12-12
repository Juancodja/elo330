import asyncio
import websockets
import json

from random import randint
from time import sleep

def random_shiet(players):
    r = randint(0, 255)
    g = randint(0, 255)
    b = randint(0, 255)

    posX = randint(100, 500)
    posY = randint(100, 500)

    players["Players"][0]["posX"] = posX
    players["Players"][0]["posY"] = posY

    players["Players"][0]["R"] = r
    players["Players"][0]["G"] = g
    players["Players"][0]["B"] = b

    return players


def move(message, players):
    dir = message["move"]["dir"]
    playerInd = 0
    delta = 5
    if dir == "D":
        players["Players"][playerInd]["posY"] += delta
    if dir == "U":
        players["Players"][playerInd]["posY"] -= delta
    if dir == "L":
        players["Players"][playerInd]["posX"] -= delta
    if dir == "R":
        players["Players"][playerInd]["posX"] += delta

    print(players)
    print(f"Move: {dir} ")



def shoot(message, players):
    shoot = message["shoot"]
    print(shoot)

def saveState(players):
    with open('players.json', 'w') as outfile:
        json.dump(players, outfile)

def messageHandler(message):
    print(message)
    message = json.loads(message)
    with open('players.json') as user_file:
        players = json.load(user_file)
    
    if 'move' in message.keys():
        move(message, players)
    elif 'shoot' in message.keys():
        shoot(message, players)
    else:
        pass
    
    saveState(players)

    response = json.dumps(players)


    return response



async def handler(websocket):
    async for message in websocket:
        response = messageHandler(message)
        await websocket.send(response)
    

async def echo(websocket):
    async for message in websocket:
        with open('players.json') as user_file:
            parsed_json = json.load(user_file)
        random_shiet(parsed_json)
        message = json.dumps(parsed_json)
        await websocket.send(message)
        print("asda")



async def main():
    async with websockets.serve(handler, "localhost", 47201):
        await asyncio.Future()  # run forever

asyncio.run(main())