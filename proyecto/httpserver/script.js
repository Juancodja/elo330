let circle = document.querySelector('.circle');
let moveBy = 10;

window.addEventListener('load', ()=>{
    circle.style.position = 'absolute';
    circle.style.left = 0;
    circle.style.top = 0; 

})

window.addEventListener('keydown', (e)=>{
    switch(e.key){
        case 'ArrowLeft':
            circle.style.left = parseInt(circle.style.left) - moveBy +'px';
            break;
        case 'ArrowRight':
            circle.style.left = parseInt(circle.style.left) + moveBy +'px';
            break;
        case 'ArrowUp':
            circle.style.top = (parseInt(circle.style.top) - moveBy) +'px';
            break;
        case 'ArrowDown':
            circle.style.top = parseInt(circle.style.top) + moveBy +'px';            
            break;
    }
    console.log(circle.style.top, circle.style.left);

})

funtion vuelta(objt){
    var x = parseInt(objt.style.left);
    var y = parseInt(objt.style.top);

}