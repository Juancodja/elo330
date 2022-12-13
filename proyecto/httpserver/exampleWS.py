import asyncio
import websockets
import json
import threading

from random import randint
from time import sleep
from math import sqrt

data_lock = threading.Lock()

PORT = 47201
## bullet update time delta
delta = 1/50

## player respawn time
respawn = 5

## board
height = 1400
width = 800

## bullets
bullet_size = 5
player_size = 20

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
        if (gameData["Players"][playerInd]["posY"] + delta) < 0:
            gameData["Players"][playerInd]["posY"] = 0
        else:
            gameData["Players"][playerInd]["posY"] += delta
    if dir == "U":
        if (gameData["Players"][playerInd]["posY"] - delta) > height:
            gameData["Players"][playerInd]["posY"] = height
        else:
            gameData["Players"][playerInd]["posY"] -= delta
    if dir == "L":
        if (gameData["Players"][playerInd]["posX"] - delta) < 0:
            gameData["Players"][playerInd]["posX"] = 0
        else:
            gameData["Players"][playerInd]["posX"] -= delta
    if dir == "R":
        if (gameData["Players"][playerInd]["posX"] + delta) > width:
            gameData["Players"][playerInd]["posX"] = width
        else:
            gameData["Players"][playerInd]["posX"] += delta
    print(gameData)
    print(f"Move: {dir} ")

def getPlayerPos(playerName):
    for player in gameData["Players"]:
        if player["name"] == playerName:
            return (player["posX"], player["posY"])

def computeSpeed(xi, yi, xf, yf):
    x = xf-xi
    y = yf-yi
    h = sqrt(x**2 + y**2)

    velX = 10*x/h
    velY = 10*y/h
    print((velX, velY))
    print((int(velX), int(velY)))
    return (int(velX), int(velY))

def shoot(message):
    shoot = message["shoot"]
    xMouse = shoot["mouseX"]
    yMouse = shoot["mouseY"]
    xPlayer, yPlayer  = getPlayerPos(shoot["name"])

    velX, velY = computeSpeed(xPlayer, yPlayer, xMouse, yMouse)

    bullet = {"posX": xPlayer + 4*velX, "posY": yPlayer + 4*velY, "velX": velX ,"velY": velY}
    
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
    
##############
# GAME LOGIC #
##############

class Game(threading.Thread):
    def run(self):
        bulletHandler()


def bulletHandler():
    while(1):
        with data_lock:
            for i, bullet in enumerate(gameData["bullets"]):
                
                bullet["posX"] = bullet["posX"] + bullet["velX"]
                bullet["posY"] = bullet["posY"] + bullet["velY"]

                if bullet["posX"] not in range(0, height):
                    gameData["bullets"].pop(i)
                elif bullet["posY"] not in range(0, width):
                    gameData["bullets"].pop(i)

                for player in gameData["Players"]:
                    if (bullet["posX"] in range(player["posX"] - player_size, player["posX"] + player_size - 1) 
                        and bullet["posY"] in range(player["posY"] - player_size, player["posY"] + player_size - 1)):
                        if player["alive"] == 1:
                            player_kill(player)
                            gameData["bullets"].pop(i)
                            break
        sleep(delta)



class death_handler(threading.Thread):      ## debe aceptar argumento jugador del dict
    def __init__(self, player):
        super().__init__()
        self.player = player

    def run(self):
        sleep(respawn)
        with data_lock:
            try:
                self.player["alive"] = 1
            finally:
                pass
        return


def player_kill(player):
    player["alive"] = 0
    death_handler(player).start()
    return


### Main



async def main():
    game = Game()
    game.start()
    async with websockets.serve(handler, "localhost", PORT):
        await asyncio.Future()  # run forever

asyncio.run(main())


