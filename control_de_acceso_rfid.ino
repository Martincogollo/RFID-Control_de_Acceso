const int verde =  5;
const int rojo =  4;
const int amarillo =  3;


#include <SPI.h> //Librería para comunicar por el bus SPI
#include <MFRC522.h>

#define RST_PIN 9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SLAVE SELECT (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522 y mandamos las constantes creadas antes

byte LecturaUID[4]; //Creamos un array de 4 bits para guardar los valores de los UID
byte Usuario1[4] = {0x69, 0x5C, 0x87, 0x6D}; //0x Es para decirle que es hexaedimal. ES ID DEL  llavero
byte Usuario2[4] = {0xA3, 0x90, 0xEB, 0x20}; //  Trajeta


void setup() {
  Serial.begin(9600); //Iniciamos el puerto serie
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos  el MFRC522. PCD es el módulo lector(proximity coupling devide)
  Serial.println("Vamos, pasa tarjeta o llavero anda");

  pinMode(verde, OUTPUT);
  pinMode(rojo, OUTPUT);
  pinMode(amarillo, OUTPUT);
}

void loop() {
  digitalWrite(verde, LOW);
  digitalWrite(rojo, LOW);
  digitalWrite(amarillo, HIGH);
  //PICC es la tarjeta o llavero (Proximity integrated circuit chip) 
  if ( ! mfrc522.PICC_IsNewCardPresent())    // si no hay una tarjeta presente
    return;           // retorna al loop esperando por una tarjeta

  if ( ! mfrc522.PICC_ReadCardSerial())     // si no puede obtener datos de la tarjeta
    return;           // retorna al loop esperando por otra tarjeta

    //Enviamos serialemente su UID (Unique Identification)
    //UID es en hexadecimal y es unico para cada tarjeta
    Serial.print("UID:");       // muestra texto UID:
    //Para que nos saque el UID de dos en dos digitos
    for (byte i = 0; i < mfrc522.uid.size; i++) { // bucle recorre de a un byte por vez el UID
        if (mfrc522.uid.uidByte[i] < 0x10) {  // si el byte leido es menor a 0x10
          Serial.print(" 0");       // imprime espacio en blanco y numero cero
        }
        else {          // sino
          Serial.print(" ");        // imprime un espacio en blanco
        }
          Serial.print(mfrc522.uid.uidByte[i], HEX);    // imprime el byte del UID leido en hexadecimal
          LecturaUID[i] = mfrc522.uid.uidByte[i];   // almacena en array el byte del UID leido
    }

          Serial.print("\t");         // imprime un espacio de tabulacion

        if (comparaUID(LecturaUID, Usuario1))   // llama a funcion comparaUID con Usuario1
          {Serial.println("¡¡¡Acceso Concedido!!!"); // si retorna verdadero muestra texto bienvenida
          digitalWrite(verde, HIGH);
          digitalWrite(rojo, LOW);
          digitalWrite(amarillo, LOW);
          delay (2000);
        }
        else if (comparaUID(LecturaUID, Usuario2)) // llama a funcion comparaUID con Usuario2
          {Serial.println("¡¡¡Acceso Concedido!!!"); // si retorna verdadero muestra texto bienvenida
          digitalWrite(verde, HIGH);
          digitalWrite(rojo, LOW);
          digitalWrite(amarillo, LOW);
          delay (2000);
          }          
          else          // si retorna falso
          {Serial.println("¡¡¡Acceso Denegado!!!");    // muestra texto equivalente a acceso denegado
          digitalWrite(verde, LOW);
          digitalWrite(rojo, HIGH);
          digitalWrite(amarillo, LOW);
          delay (2000);
          }
          // Terminamos la lectura de la tarjeta  actual
               mfrc522.PICC_HaltA();     // detiene comunicacion con tarjeta
               //Así le decimos a la tarjeta que no necesitamos mas info
               //así el lector para y no consume, mandando la señal
}


boolean comparaUID(byte lectura[], byte usuario[]) // funcion comparaUID
{
  for (byte i = 0; i < mfrc522.uid.size; i++) { // bucle recorre de a un byte por vez el UID
    if (lectura[i] != usuario[i])       // si byte de UID leido es distinto a usuario
      return (false);         // retorna falso
  }
  return (true);          // si los 4 bytes coinciden retorna verdadero
}