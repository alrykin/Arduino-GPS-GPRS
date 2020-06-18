#include "TinyGPS.h"
TinyGPS gps;


const bool DEBUG = false;
const String Trigger = "trigger";

bool Param = false;
String Serial_get, Package = "no connection";

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, Param);

  Serial1.begin(115200);                 //gsm
  Serial2.begin(9600);                   //gps
  Serial.begin(115200);                  //комп

  Serial1.println("AT&F");               // Восстановление заводских настроек
  updateSerial();
  Serial1.println("AT+CNMI=3,2");        // Настройка обработки входящих сообщений
  updateSerial();
  Serial1.println("AT+CLIP=1");          // Включение автоопределения номера
  updateSerial();
  Serial1.println("AT+CMGF=1");          // Настраивает текстовый режим сообщения
  updateSerial();

}


void loop() {
  //PC_to_GSM();                         // Для ручного управления  
  GSM_lesten();
  GPS_listen();
}

void updateSerial() {
  delay(500);                            // Задержка для того что бы модуль успел обработать предыдущую команду
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}

void GPS_listen() {
  if (Serial2.available()) {
    while (Serial2.available())
    {
      char c = Serial2.read();
      if (gps.encode(c)) {                                                                    // Проверка есть ли во входящем пакете даных даные, интересующие нас
        float latitude, longitude;
        gps.f_get_position(&latitude, &longitude);                                            // Получение широты и долготы
        latitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : latitude, 6;                         // Проверка, является ли значение угла правильным. Если нет - присваевает 0. 6- количество знаков после запятой
        longitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : longitude, 6;
        Package = "Широта: " + String(latitude, 6) + " Долгота:" + String(longitude, 6);      // Формирует пакет даных на отправку
        if (DEBUG) {
          Serial.print("LAT=");
          Serial.print(latitude, 6);
          Serial.print(" LON=");
          Serial.println(longitude, 6);
        }
      }
      else Package = "no connection";
    }
  }
}


void GSM_lesten() {
  if (Serial1.available()) {
    delay(100);
    Serial_get = "";
    while (Serial1.available()) Serial_get += (char)Serial1.read();
    React();
  }
}


void React() {
  if (Serial_get.indexOf("RING") != -1) {                                                      // Обнаружено входящий вызов
    Serial1.println("ATH");                                                                    // Заканчивает звонок
    updateSerial();
    if (DEBUG) Serial.println("DEBUG1 " + Serial_get.substring(18, 30) + "\\DEBUG1");          // Номер телефона
    Serial1.println("AT+CMGS=\"+" + Serial_get.substring(18, 30) + "\"");                      // Отправляет посылку/
    updateSerial();
    Serial1.print(Package);                                                                    
    updateSerial();
    Serial1.print("");                                                                         // Отправляет посылку\    в кавычках символ "Ctrl+Z" 
  }
  else if (Serial_get.indexOf("+CMT") != -1) {                                                 // Обнаружено входящее сообщение
    if (DEBUG) Serial.println("DEBUG2/ " + Serial_get.substring(9, 21) + "\\DEBUG2");          // Номер телефона
    if (Serial_get.indexOf(Trigger) != -1) {
      Change_param();
    } else {
          Serial1.println("AT+CMGS=\"+" + Serial_get.substring(9, 21) + "\"");                 // Отправляет посылку
    updateSerial();
    Serial1.print(Package);
    updateSerial();
    Serial1.print("");                                                                         // Отправляет посылку\    в кавычках символ "Ctrl+Z" 
    }
  }
  else if (DEBUG) Serial.println("DEBUG0/" + Serial_get + "\\DEBUG0");                         //показывает ВСЕ что пришло в порт, если это не привело к какому-то из действий выше
}


void Change_param() {                                                                          // При вызове изменяет значение переменной и отправляет новое значение на 2 цифровой порт
  Param = !Param;
  digitalWrite(2, Param);
  if (DEBUG) Serial.println("Trigger = " + String(Param));
}


void PC_to_GSM() {                                                                             // Для отправки команды на модуль GSM через терминал компа.
  Serial_get = "";
  if (Serial.available()) {
    delay(10);
    while (Serial.available()) Serial_get += (char)Serial.read();
    Serial1.println(Serial_get);
  }
}
