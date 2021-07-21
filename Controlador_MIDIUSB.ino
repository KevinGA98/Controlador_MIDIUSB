#include <Keypad.h>               //Se carga la libreria "Keypad".
#include "MIDIUSB.h"

const byte ROWS = 6;              //Se crea una constante para las 6 filas de la matriz
const byte COLS = 9;              //Se crea una constante para las 9 columnas

byte keys[ROWS][COLS];            //Se crea un arreglo matricial con las filas y las columnas

byte rowPins[ROWS] = {A0, A1, A2, A3, A4, A5};                     //Se indican los valores de los pines de cada fila
byte colPins[COLS] = {2, 3, 4, 5, 6, 7, 8, 9, 10}; //Se indican los valores de los pines de cada columna

byte tecla_presionada;                                             //Se crea una variable que almacena si la tecla está presionada
byte estado;

int nota = 31;                                                     //Se crea una constante para definir la nota inicial

long millis_anteriores = 0;        //  Almacenara la ultima vez que el LED fue actualizado
long intervalo = 500;              // intervaloo en el cual parpadea (millisegundos)

String cadena;                    //Variable tipo string que se llama cadena               
char c;                           //varaible tipo caracter, guarda cada caracter que llega desde el bluetooth                 


Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);  //Se crea el objeto tipo Keypad que es la matriz del teclado

String msg = "";                                                      //Se crea una variable msg de tipo String

void setup() 
{
  Serial.begin(115200);                                             //Se inicia la comunicación Serial
    for(int p=2; p<=19; p++){
      pinMode(p, INPUT_PULLUP);
    }
    
    for (int c=0; c<COLS; c++){                                       //Se almacenan los valores de las notas
      for (int r=0; r<ROWS; r++){                                     //en cada espacio de la matriz
        keys[r][c]=nota;
        nota++;
      }
    }
}

void loop() 
{
  if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)                                //Se escanea toda la Matriz.
        {
            if ( kpd.key[i].stateChanged )                            //Si cambia de estado una tecla.
            {
                switch (kpd.key[i].kstate) {                          //Evalua el estado de la tecla
                    case PRESSED:                                     //Si la tecla está presionada
                    msg = " PRESSED.";                                //Se almacena el mensaje "PRESSED"
                    tecla_presionada=kpd.key[i].kchar;                //Se almacena la tecla en la variable tecla_presionada
                    noteOn(0, tecla_presionada, 64);         //Se envía el mensaje MIDI para activar la tecla presionada
                    MidiUSB.flush();
                break;
                    case RELEASED:                                    //Si la tecla está liberada
                    msg = " RELEASED.";                               //Se almacena el mensaje "RELEASED"
                    tecla_presionada=kpd.key[i].kchar;                //Se almacena la tecla en la variable tecla_presionada
                    noteOff(0, tecla_presionada, 64);        //Se envía el mensaje MIDI para desactivar la tecla liberada
                    MidiUSB.flush();
                break;
                }
            }
        }
    }
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
