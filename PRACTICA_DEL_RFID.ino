//Oscar Jose Barrios Cotom - 2022241


#include <SPI.h>			// incluye libreria bus SPI
#include <MFRC522.h>			// incluye libreria especifica para MFRC522
#include <Servo.h>//Librerias
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int Ledrojo = 7;
int Ledverde = 6;
const int servoPin1 = 8;
int buzzer = 5;
Servo servo;
#define RST_PIN  9			// constante para referenciar pin de reset
#define SS_PIN  10			// constante para referenciar pin de slave select

MFRC522 mfrc522(SS_PIN, RST_PIN);	// crea objeto mfrc522 enviando pines de slave select y reset
LiquidCrystal_I2C lcd(0x27, 16, 2);// Inicializa el objeto LiquidCrystal_I2C para la pantalla LCD

byte LecturaUID[4]; 				// crea array para almacenar el UID leido
byte Usuario1[4]= {0x69, 0x0D9, 0x12, 0xC3} ;    // UID de tarjeta leido en programa 1
byte Usuario2[4]= {0xB2, 0x054, 0x9E, 0x1C} ;    // UID de llavero leido en programa 1

void setup() {
  Serial.begin(9600);			// inicializa comunicacion por monitor serie a 9600 bps
  SPI.begin();				// inicializa bus SPI
  mfrc522.PCD_Init();			// inicializa modulo lector
  Serial.println("Listo");		// Muestra texto Listo
  servo.attach(servoPin1);
  servo.write(0);
  pinMode(Ledrojo, OUTPUT);
  pinMode(Ledverde, OUTPUT);
  pinMode(buzzer, OUTPUT);

   Wire.begin();// Inicializa la comunicación I2C
  lcd.backlight();// Encender la retroiluminación de la LCD

  lcd.begin(16, 2);
  lcd.print("Pon la tarjeta");//Imprime el texto en la LCD

}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent())		// si no hay una tarjeta presente
    return;						// retorna al loop esperando por una tarjeta
  
  if ( ! mfrc522.PICC_ReadCardSerial()) 		// si no puede obtener datos de la tarjeta
    return;						// retorna al loop esperando por otra tarjeta
    
    Serial.print("UID:");				// muestra texto UID:
    for (byte i = 0; i < mfrc522.uid.size; i++) {	// bucle recorre de a un byte por vez el UID
      if (mfrc522.uid.uidByte[i] < 0x10){		// si el byte leido es menor a 0x10
        Serial.print(" 0");				// imprime espacio en blanco y numero cero
        }
        else{						// sino
          Serial.print(" ");				// imprime un espacio en blanco
          }
          Serial.print(mfrc522.uid.uidByte[i], HEX);   	// imprime el byte del UID leido en hexadecimal
          LecturaUID[i]=mfrc522.uid.uidByte[i];   	// almacena en array el byte del UID leido      
          }
          
          Serial.print("\t");   			// imprime un espacio de tabulacion             
                    
          if(comparaUID(LecturaUID, Usuario1)){//llama a funcion comparaUID con Usuario1
            lcd.clear();
            Serial.println("Bienvenido Usuario 1");	// si retorna verdadero muestra texto bienvenida
            servo.write(90);//Se mueve a 90 grados si cumple el if
            delay(20);
            lcd.print("Hola Usuario1");
            digitalWrite(buzzer, HIGH);
            digitalWrite(Ledverde, HIGH);
            digitalWrite(Ledrojo, LOW);
            delay(3000);
            digitalWrite(Ledverde, LOW);
            digitalWrite(Ledrojo, HIGH);
            digitalWrite(buzzer, LOW);
            lcd.clear();
            lcd.print("Pon la tarjeta");//Imprime el texto en la LCD
            servo.write(0);
          }
          else if(comparaUID(LecturaUID, Usuario2)){// llama a funcion comparaUID con Usuario2
            lcd.clear();
            Serial.println("Bienvenido Usuario2");	// si retorna verdadero muestra texto bienvenida
            servo.write(90);
            delay(20);
            lcd.print("Hola Profe");
            digitalWrite(buzzer, HIGH);
            digitalWrite(Ledverde, HIGH);
            digitalWrite(Ledrojo, LOW);
            delay(3000);
            digitalWrite(Ledverde, LOW);
            digitalWrite(Ledrojo, HIGH);
            digitalWrite(buzzer, LOW);
            lcd.clear();
            lcd.print("Pon la tarjeta");//Imprime el texto en la LCD
            servo.write(0);
          }	 
          else{						// si retorna falso
            Serial.println("No te conozco");	// muestra texto equivalente a acceso denegado          
            lcd.clear(); 
            servo.write(0);
            lcd.print("Quien sos?");
            digitalWrite(Ledrojo, HIGH);   
            delay(3000);
            lcd.clear();
            lcd.print("Pon la tarjeta");//Imprime el texto en la LCD
          }
                  mfrc522.PICC_HaltA();  		// detiene comunicacion con tarjeta                
}

boolean comparaUID(byte lectura[],byte usuario[])	// funcion comparaUID
{
  for (byte i=0; i < mfrc522.uid.size; i++){		// bucle recorre de a un byte por vez el UID
  if(lectura[i] != usuario[i])				// si byte de UID leido es distinto a usuario
    return(false);					// retorna falso
  }
  return(true);						// si los 4 bytes coinciden retorna verdadero
}
