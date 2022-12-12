# Proyecto ELO330

## Tablero

- Ancho pantalla 1200x700
- Ancho plataforma 1000x500


Enviar por socket TCP archivo json


Lo que envia TCP :

{   
    jugadores:{nombre_jugador: {
        pos x: int,
        pos y: int,
        color: [r,g,b]
    }
    {balas: [pos , pos...]}
}


Lo que recibe TCP:

bala desde hacia
jugador x up

{bala: (xf - xi, yf - yi)}


### Carateristicas Juego

1920x1080

pantalla 1400x800
tablero  1000x600

jugador 20x20
balas   10x10

Logica muerte, 
- cambiar el bit alive a 0
- 5s tiempo respawn