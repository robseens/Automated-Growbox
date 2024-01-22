#include <Arduino.h>
#include <DHT.h>

// Definējam komponentes
const int ventilatorsPin = 14;  
const int gaismaPin = 12;       
const int pumpisPin = 32;        

// Definējam sensorus
const int dhtPin = 18;           
const int moistureSensorPin = 19; 

// Izveidojam DHT11 sensoru
DHT dht(dhtPin, DHT11);

// Mainīgie gaismas kotnrolei 
unsigned long previousMillisGaisma = 0;
const long onDurationGaisma = 16 * 3600000; // 16 stundas
const long offDurationGaisma = 8 * 3600000; // 8 stundas

void setup() {
  // Komponentes izveidojam kā outputus
  pinMode(ventilatorsPin, OUTPUT); 
  pinMode(gaismaPin, OUTPUT);       
  pinMode(pumpisPin, OUTPUT);       

  // DHT11 sensora sākums
  dht.begin();
}

void loop() {
  // Nolasām temperatūru un gaisa mitrumu
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Pārbaudām vai temperatūra ir lielāka par 25 grādiem Celsija
  if (temperature > 25) {
    // Ja, jā, ieslēdzam ventilatoru uz 30 sekundēm un tad izslēdzam
    digitalWrite(ventilatorsPin, HIGH);  
    delay(30000);
    digitalWrite(ventilatorsPin, LOW);   
  }

  // Pārbadām vai gaisa mitrums ir virs 70%
  if (humidity > 70) {
    // Ja, jā, ieslēdzam ventilatoru uz 30 sekundēm un tad izslēdzam
    digitalWrite(ventilatorsPin, HIGH);
    delay(30000);  
    digitalWrite(ventilatorsPin, LOW);  
  }

  delay(1000);

  // Gaismas kontrole, lai tā darbotos 16 stundu ieslēgta un 8 stundu izslēgta ciklā
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisGaisma < onDurationGaisma) {
    // Ieslēdzam gaismu
    digitalWrite(gaismaPin, HIGH);
  } else if (currentMillis - previousMillisGaisma < onDurationGaisma + offDurationGaisma) {
    // Izslēdzam gaismu
    digitalWrite(gaismaPin, LOW);
  } else {
    // Restartējam laika uzskaiti
    previousMillisGaisma = currentMillis;
  }

  
  delay(1000);

  // Nolasām augsnes mitruma vērtību
  int soilMoisture = analogRead(moistureSensorPin);

  // Pārbadām vai augsnes mitrums nepārsniedz 380
  if (soilMoisture > 380) {
    // Ja, jā, ieslēdzam ūdens pumpi, kas aplaista augu uz divām sekundēm
    digitalWrite(pumpisPin, HIGH);
    delay(2000);
    digitalWrite(pumpisPin, LOW);
  }

  // Pēc 30 sekundēm atkārtojam ciklu
  delay(30000);
}
