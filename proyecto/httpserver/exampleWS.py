import asyncio
import websockets
import json

from random import randint
from time import sleep

gameData = {"players":[
                {
                    "name": "Player0",
                    "posX": 0,
                    "posY": 0,
                    "R": 255, 
                    "G": 255, 
                    "B": 255, 
                    "alive": 1
                }
            ],
            "bullets":[
                {
                    "posX": 0,
                    "posY": 0,
                    "velX": 0,
                    "velY": 0,
                }
            ]
}


def random_shiet(gameData):
    r = randint(0, 255)
    g = randint(0, 255)
    b = randint(0, 255)

    posX = randint(100, 500)
    posY = randint(100, 500)

    gameData["Players"][0]["posX"] = posX
    gameData["Players"][0]["posY"] = posY

    gameData["Players"][0]["R"] = r
    gameData["Players"][0]["G"] = g
    gameData["Players"][0]["B"] = b

    return gameData


def move(message, gameData):
    playerName = message["move"]["name"]
    dir = message["move"]["dir"]
    i = 0
    playerInd = 0

    for player in gameData["Players"]: 
        if player["name"] == playerName:
            playerInd = i
        i = i +1

    delta = 5
    if dir == "D":
        gameData["Players"][playerInd]["posY"] += delta
    if dir == "U":
        gameData["Players"][playerInd]["posY"] -= delta
    if dir == "L":
        gameData["Players"][playerInd]["posX"] -= delta
    if dir == "R":
        gameData["Players"][playerInd]["posX"] += delta

    print(gameData)
    print(f"Move: {dir} ")



def shoot(message, players):
    shoot = message["shoot"]
    print(shoot)

def saveState(gameData):
    with open('players.json', 'w') as outfile:
        json.dump(gameData, outfile)

def messageHandler(message):
    print(message)
    message = json.loads(message)
    with open('players.json') as user_file:
        gameData = json.load(user_file)

    if 'refresh' in message.keys():
        pass
    elif 'move' in message.keys():
        move(message, gameData)
    elif 'shoot' in message.keys():
        shoot(message, gameData)
    else:
        pass
    
    saveState(gameData)

    response = json.dumps(gameData)

    return response



async def handler(websocket):
    async for message in websocket:
        response = messageHandler(message)
        await websocket.send(response)
    



async def main():
    async with websockets.serve(handler, "localhost", 47201):
        await asyncio.Future()  # run forever

asyncio.run(main())



