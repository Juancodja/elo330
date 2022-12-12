const socket = new WebSocket('ws://localhost:47201');

const circle = document.querySelector('.circle');
let moveBy = 10;
let playerName = "player"
window.addEventListener('load', ()=>{
    circle.style.position = 'absolute';
    circle.style.left = 0;
    circle.style.top = 0; 

})

socket.addEventListener("open", e => {
    let message = {"mimama": "la tuya"};
    socket.send(JSON.stringify(message));
});

socket.onmessage = ({ data }) => {
    console.log('Message from server', data);
    let myArr = JSON.parse(data);
    let players = myArr.Players

    for ( let i in players){
        circle.style.left = parseInt(players[i].posX) +'px';
        circle.style.top = parseInt(players[i].posY) +'px';
        console.log(circle.style.top, circle.style.left);
    }

};

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
    }
    message.move.dir = dir;
    socket.send(JSON.stringify(message));
}
