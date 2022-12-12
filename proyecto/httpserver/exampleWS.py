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

with open('players.json') as user_file:
    gameData = json.load(user_file)

CLIENTS = set()




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
            break
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

def getPlayerPos(playerName):
    for player in gameData["Players"]:
        if player["name"] == playerName:
            return player["posX"], player["posY"] 

def computeSpeed(xi, xf, yi, yf):
    x = xf-xi
    y = yf-yi
    h = (x**2 + y**2+2)**(1/2)

    velX = 40*x/h
    velY = 40*y/h
    
    return int(velX), int(velY)

def shoot(message):
    shoot = message["shoot"]
    xMouse = shoot["mouseX"]
    yMouse = shoot["mouseY"]
    xPlayer, yPlayer  = getPlayerPos(shoot["name"])

    velX, velY = computeSpeed(xPlayer, xPlayer, xMouse , yMouse)

    bullet = {"posX": xPlayer + velX, "posY": yPlayer + velY, "velX": velX ,"velY": velY}
    
    gameData["bullets"].append(bullet)
    print(bullet)

def messageHandler(message):
    message = json.loads(message)

    if 'move' in message.keys():
        move(message, gameData)
    elif 'shoot' in message.keys():
        shoot(message)
    else:
        pass

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



