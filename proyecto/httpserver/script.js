const socket = new WebSocket('ws://localhost:47201');

const PLAYERSIZE  = 40;
const BULLETSIZE  = 15; 

let canvas = document.getElementById("mainCanvas");
var context = canvas.getContext("2d");

let playerName = prompt("Ingrese player Name")



window.addEventListener('load', ()=>{
})

socket.addEventListener("open", e => {
    let message = {"mimama": "la tuya"};
    socket.send(JSON.stringify(message));
});

socket.onmessage = ({ data }) => {
    console.debug('Message from server', data);
    let gameData = JSON.parse(data);

    draw(gameData);

};

var intervalId = setInterval(function() {
    socket.send("{}")
  }, 10);

window.addEventListener('keydown', move);

window.addEventListener('click', (e)=>{
    let shoot = {"shoot":{
        "name": playerName,
        "mouseX": 0,
        "mouseY": 0
        }
    }
    shoot.shoot.mouseX = e.clientX;
    shoot.shoot.mouseY = e.clientY;
    console.log(JSON.stringify(shoot))
    socket.send(JSON.stringify(shoot))
})



function move(e){
    let message = {"move":{
        "name": playerName,
        "dir": ''
        }
    }
    let dir = 'N'
    switch(e.key){
        case 'ArrowLeft':
            dir = "L"
            break;
        case 'ArrowRight':
            dir = "R"
            break;
        case 'ArrowUp':
            dir = "U"
            break;
        case 'ArrowDown':
            dir = "D"
            break;

        case 'a':
            dir = "L"
            break;
        case 'd':
            dir = "R"
            break;
        case 'w':
            dir = "U"
            break;
        case 's':
            dir = "D"
            break;

    }
    message.move.dir = dir;
    socket.send(JSON.stringify(message));
}


function draw(gameData){
    let players = gameData["Players"];
    let bullets = gameData["bullets"];
    context.clearRect(0, 0, canvas.width, canvas.height);
    for(let i in players){ 
        if(players[i].alive == 1){
            let R = players[i].R;
            let G = players[i].G; 
            let B = players[i].B;
    
            let color = '#' + R.toString(16) + G.toString(16) + B.toString(16);
            drawPoint(context, players[i].posX, players[i].posY, players[i].name, color, PLAYERSIZE);         
        }
       
    }
    for(let i in bullets){
        drawPoint(context, bullets[i].posX, bullets[i].posY, null, null, BULLETSIZE);
    }
}


function drawPoint(context, x, y, label, color, size) {
    if (color == null) {
        color = '#000'; 
    }
    if (size == null) {
        size = 5;
    }

    var radius = 0.5 * size;

    // to increase smoothing for numbers with decimal part
    var pointX = Math.round(x - radius);
    var pointY = Math.round(y - radius);

    context.beginPath();
    context.fillStyle = color;
    context.fillRect(pointX, pointY, size, size);
    context.fill();

    if (label) {
        var textX = Math.round(x);
        var textY = Math.round(pointY - 5);

        context.font = 'Italic 14px Arial';
        context.fillStyle = color;
        context.textAlign = 'center';
        context.fillText(label, textX, textY);
    }
}