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

#define 