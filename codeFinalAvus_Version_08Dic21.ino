#include <pcmConfig.h> //Lib audio configuracion basica
#include <pcmRF.h> //Lib audio 
#include <TMRpcm.h> //Lib audio para reproduccion WAV

#include <Wire.h> //Lib comunicacion I2C
#include <RTClib.h> //Lib reloj

#define SD_ChipSelectPin 53 //Pin de seleccion de SD

//Configuracion SPI
/*
  CS --> D9
  Mosi --> D11
  SCK ---> D13
  Miso --> D12
  GND --> GND
  LED --> D7
  RX -->Debe ir al D6
  TX -->Debe ir al D5
*/



int led1 = 32; 
int led2 = 33;
int led3 = 35;

//Creacion pulsadores buscaobjeto y boton ganador
int pulsador1 = digitalRead(3);
int pulsador2 = digitalRead(4);
int pulsador3 = digitalRead(5);
int pulsador4 = digitalRead(6);
int pulsador5 = digitalRead(7);
int pulsador6 = digitalRead(8);
int pulsadorW = digitalRead(9);

//Variables bool para alarmas
bool recordatorio1 = true;
bool recordatorio2 = true;
bool recordatorio3 = true;
bool recordatorio4 = true;
bool recordatorio5 = true;
bool recordatorio6 = true;

int ledDelay = 400;

TMRpcm Audio; //crea objeto para control de audio
File logFile; //crea archivo .txt para la SD
RTC_DS3231 rtc;     // crea objeto del reloj tipo RTC_DS3231

void setup()
{
  Serial.begin(9600);//inicio de comunicacion serial

//Puertos de los pulsadores busca objetos y boton ganador
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);

//config inicial para la deteccion y lectura de SD
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println(F("Error al iniciar!!"));
    return;
  }
  Serial.println(F("SD iniciada correctamente"));

//config inicial para reloj
  if (! rtc.begin()) {
    Serial.println("RTC no encontrado!!");
    while (1);
  }
  Serial.println(F("RTC iniciado correctamente"));

//ajuste de fecha y hora actual
  rtc.adjust(DateTime(__DATE__, __TIME__ ));

//ajuste para audio (puerto, calidad, volumen)  
  Audio.speakerPin = 11; //Salida para el audio
  Audio.quality(1);
  Audio.setVolume(5);

}

void loop()
{
  DateTime fecha = rtc.now(); //Actualizacion hora y fecha cuando se programe el reloj

  int  valor1 = !digitalRead(3); //lectura de puerto 
  if (digitalRead(3) == HIGH) //condicional 
  {
    dates(); //Funcion de hora y fecha
    boton1(); //Funcion de escritura en SD
    boton11(); //Funcion de activacion de audio
  }

  if (digitalRead(4) == HIGH)
  {
    dates();
    boton2();
    boton22();
  }

  if (digitalRead(5) == HIGH)
  {
    dates();
    boton3();
    boton33();
  }

  if (digitalRead(6) == HIGH)
  {
    dates();
    boton4();
    boton44();
  }

  if (digitalRead(7) == HIGH)
  {
    dates();
    boton5();
    boton55();
  }

  if (digitalRead(8) == HIGH)
  {
    dates();
    boton6();
    boton66();
  }

  if (digitalRead(9) == HIGH)
  {
    dates();
    botonW();
    botonWW();
    botonWL();
  }

//Led parpadeo
  //  digitalWrite(led, HIGH);
  //  delay(500);
  //  digitalWrite(led, LOW);
  //  delay(500);

//Alarmas para los eventos. Segun la hora se activará escritura en SD y Audio correspondiente
  if (fecha.hour() == 8 && fecha.minute() == 0 && fecha.second() == 0) {
    if (recordatorio1 == true) {
      Serial.println("Pastillas AM"); logFile.print("Pastillas AM");
      Audio.play("8.wav");
      delay(6000);
      recordatorio1 = false;  //Desactivacion del evento
    }
    recordatorio1 = true; //Activacion de la alarma nuevamente para el siguiente dia
  }

  if (fecha.hour() == 19 && fecha.minute() == 0 && fecha.second() == 0) {
    if (recordatorio2 == true) {
      Serial.println("Pastillas PM"); logFile.print("Pastillas PM");
      recordatorio2 = false;
      Audio.play("8.wav");
      delay(6000);
    }
    recordatorio2 = true;
  }

  if (fecha.hour() == 9 && fecha.minute() == 0 && fecha.second() == 0) {
    if (recordatorio3 == true) {
      Serial.println("Ejercicio AM"); logFile.print("Ejercicio AM");
      recordatorio3 = false;
      Audio.play("9.wav");
      delay(6000);
    }
    recordatorio3 = true;
  }

  if (fecha.hour() == 14 && fecha.minute() == 0 && fecha.second() == 0) {
    if (recordatorio4 == true) {
      Serial.println("Ejercicio PM"); logFile.print("Ejercicio PM");
      recordatorio4 = false;
      Audio.play("9.wav");
      delay(6000);
    }
    recordatorio4 = true;
  }

  if (fecha.hour() == 11 && fecha.minute() == 0 && fecha.second() == 0) {
    if (recordatorio3 == true) {
      Serial.println("Actividad intelectual AM"); logFile.print("Actividad intelectual AM");
      recordatorio3 = false;
      Audio.play("10.wav");
      delay(6000);
    }
    recordatorio3 = true;
  }

  if (fecha.hour() == 16 && fecha.minute() == 0 && fecha.second() == 0) {
    if (recordatorio4 == true) {
      Serial.println("Actividad intelectual PM"); logFile.print("Actividad intelectual PM");
      recordatorio4 = false;
      Audio.play("10.wav");
      delay(6000);
    }
    recordatorio4 = true;
  }
}

//Funcion hora y fecha
void dates()
{
  logFile = SD.open("datalog.txt", FILE_WRITE); // Apertura de archivo .txt en modo escritura

  DateTime fecha = rtc.now();

  Serial.print("Fecha ");

  logFile.print(fecha.day()); logFile.print("/");
  logFile.print(fecha.month()); logFile.print("/");
  logFile.print(fecha.year()); logFile.print(" ");
  logFile.print(fecha.hour()); logFile.print(":");
  logFile.print(fecha.minute()); logFile.print(":");
  logFile.print(fecha.second());

  Serial.print(fecha.day()); Serial.print("/");
  Serial.print(fecha.month()); Serial.print("/");
  Serial.print(fecha.year()); Serial.print(" ");
  Serial.print(fecha.hour()); Serial.print(":");
  Serial.print(fecha.minute()); Serial.print(":");
  Serial.print(fecha.second());

  logFile.close(); //Cierre y guardado en el archivo .txt 
}

//Pulsador predefinido para objeto a buscar
void boton1() {
  logFile = SD.open("datalog.txt", FILE_WRITE);// Abrir archivo y escribir valor

  logFile.print(", Boton1 "); logFile.println(pulsador1);
  Serial.print(", Boton1 "); Serial.println(pulsador1) + "\n";

  pulsador1++; //Contador de uso del pulsador 
  logFile.close();
}

//Activacion de audio correspondiente 
void boton11() {
  Audio.play("1.wav");
  delay(6000);
}

void boton2() {
  logFile = SD.open("datalog.txt", FILE_WRITE);// Abrir archivo y escribir valor

  logFile.print(", Boton2 "); logFile.println(pulsador2);
  Serial.print(", Boton2 "); Serial.println(pulsador2) + "\n";

  pulsador2++;
  logFile.close();
}

void boton22() {
  Audio.play("2.wav");
  delay(6000);
}

void boton3() {
  logFile = SD.open("datalog.txt", FILE_WRITE);// Abrir archivo y escribir valor

  logFile.print(", Boton3 "); logFile.println(pulsador3);
  Serial.print(", Boton3 "); Serial.println(pulsador3) + "\n";

  pulsador3++;
  logFile.close();
}

void boton33() {
  Audio.play("3.wav");
  delay(6000);
}

void boton4() {
  logFile = SD.open("datalog.txt", FILE_WRITE);// Abrir archivo y escribir valor

  logFile.print(", Boton4 "); logFile.println(pulsador4);
  Serial.print(", Boton4 "); Serial.println(pulsador4) + "\n";

  pulsador4++;
  logFile.close();
}

void boton44() {
  Audio.play("4.wav");
  delay(6000);
}


void boton5() {
  logFile = SD.open("datalog.txt", FILE_WRITE);// Abrir archivo y escribir valor

  logFile.print(", Boton5 "); logFile.println(pulsador5);
  Serial.print(", Boton5 "); Serial.println(pulsador5) + "\n";

  pulsador5++;
  logFile.close();
}

void boton55() {
  Audio.play("5.wav");
  delay(6000);
}


void boton6() {
  logFile = SD.open("datalog.txt", FILE_WRITE);// Abrir archivo y escribir valor

  logFile.print(", Boton6 "); logFile.println(pulsador6);
  Serial.print(", Boton6 "); Serial.println(pulsador6) + "\n";

  pulsador6++;
  logFile.close();
}

void boton66() {
  Audio.play("6.wav");
  delay(6000);
}

//Pulsador predefinido para boton ganador
void botonW() {
  logFile = SD.open("datalog.txt", FILE_WRITE);// Abrir archivo y escribir valor

  logFile.print(", GANADOR! "); logFile.println(pulsadorW);
  Serial.print(", GANADOR! "); Serial.println(pulsadorW) + "\n";
  digitalWrite(led1, HIGH);
  delay(500);
  digitalWrite(led1, LOW);
  delay(500);

  pulsadorW++;
  logFile.close();
}

void botonWW() {
  Audio.play("7.wav");
  delay(6000);
}

//Leds parpadeo para boton ganador
void botonWL() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  delay(500);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  delay(500);
}
