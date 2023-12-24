//D2 Led Kırmızı
//D4 Led Yeşil
//D6 Led mavi
//D8 Buzzer
//D1 Read Röle

#define BLYNK_TEMPLATE_ID "TMPL6GKcE0zol"
#define BLYNK_TEMPLATE_NAME "Theft Alarm"
#define BLYNK_AUTH_TOKEN "KZZPqKfrONN7JtUJmYFZjb7f2eMjuoFN"

//////////////////////////////////////blynk
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

////////////////////////////////////////////AdaFruit
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "YahyaLom"
#define AIO_KEY         "aio_LPak63xW0pJqPLT8vnM04xOukFtS" //Adafruit AIO Key
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);



/****************************** Feeds ***************************************/
//Adafruit_MQTT_Publish feed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sau.saumqtt");
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/onoff");

/************************* WiFi Access Point *********************************/
char WLAN_SSID[]="lom 2.4G";
char WLAN_PASSWORD[]="yahya1453";

//char WLAN_SSID[]="Yahya";
//char WLAN_PASSWORD[]="23456789";



///////////////////////////////////////////////// Bylnk Auth Token
char auth[]="KZZPqKfrONN7JtUJmYFZjb7f2eMjuoFN";

////////////////////////////////////////////////// Bylnk İlk Bağlantı ile Senkronizasyon
bool isFirstConnect=true;
BLYNK_CONNECTED()
{
  if(isFirstConnect)
  {
    Blynk.syncAll();
    isFirstConnect=false;
  }
};


void MQTT_connect();

////////////////////////////////////////////////////////Mqtt Bağlantı
void adaSetup()
{
  Serial.println(F("Adafruit MQTT demo"));
    // Connect to WiFi access point.
    Serial.println(); Serial.println();
    // Setup MQTT subscription for onoff feed.
    mqtt.subscribe(&onoffbutton);
}

//D3 Kapatma
//D2 Led Kırmızı
//D4 Led Yeşil
//D6 Led mavi
//D8 Buzzer
//D1 Read Röle
int maviLed = 12; //GPIO04  -> D6 pini mavi ışık için
int kirmiziLed = 4; //GPIO4  -> D2 pini kırmızı ışık için
int yesilLed = 2; //GPIO2  -> D4 pini yeşil ışık için
int buzzer = 15; //GPI015  -> D8 pini buzzer için
int readRole = 5; //GPIO5 -> D1 pini read röle için




///////////////////////////////////////////////////////////Wi-Fi Bağlanması
void wifiSetup()
{
  delay(10);
  digitalWrite(maviLed, HIGH);//Mavi Yandı
  Serial.println();Serial.println(WLAN_SSID);
  Serial.println(F("Kablosuz agina baglaniyor..."));
  WiFi.begin(WLAN_SSID,WLAN_PASSWORD);//wifi giriş bilgieri
  while(WiFi.status()!=WL_CONNECTED)//wifi bağlanma işlemi olana kadar bağlanmayı deniyor
  {
    delay(500);
    Serial.print(".");
    Serial.print("baglanamadi..");
  }
  Serial.println();
  Serial.print(WLAN_SSID);Serial.println(" Kablosuz agına baglandi");
  digitalWrite(D6, LOW);//Mavi Söndü
  Serial.print("IP adresi:");Serial.println(WiFi.localIP());
};
BlynkTimer timer;
void event()
{

};

void setup() 
{
  Serial.begin(115200);
  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D3, OUTPUT);
  wifiSetup();
  adaSetup();
  Blynk.begin(auth,  WLAN_SSID,  WLAN_PASSWORD);
  //timer.setInterval(5000L, event);
}

void loop() 
{
  MQTT_connect();
  Adafruit_MQTT_Subscribe* subscription;
  while ((subscription = mqtt.readSubscription(2000)))
  {    
    if (subscription == &onoffbutton)
    {
      Serial.print(F("Got: "));
      Serial.println((char*)onoffbutton.lastread);
      Serial.println("On/Off tuşuna basıldı,sistem devrede..."); 
    }
  }

  if (strcmp((char*)onoffbutton.lastread, "OFF") == 0) 
  {
    digitalWrite(yesilLed, LOW);
    Serial.println("Off tuşuna basıldı, sistem durdu");
    Serial.print("...");
    digitalWrite(D2, HIGH);//Kırmızı Yandı
    //digitalWrite(D8, HIGH);//Buzzer Çalıştı
    delay(500);
    digitalWrite(D2, LOW);//Kırmızı Söndü
    digitalWrite(D6, HIGH);//Mavi Yandı
    delay(500);
    digitalWrite(D6, LOW);//Mavi Söndü
    digitalWrite(D4, HIGH);//Yeşil Yandı                      
    delay(500);
    digitalWrite(D4, LOW);//Yeşil Söndü
    //digitalWrite(D8, LOW);//Buzzer Durdu 
    delay(1000);     
  }
  else
  {
    if(digitalRead(readRole)==1)
      {
        digitalWrite(yesilLed, HIGH);
        //Blynk.email("yahya61ts@gmail.com", "Alert", "Theft Alert in Home");
        //Blynk.notify("Alert : Theft Alert in Home");
        //delay(1000);
        //digitalWrite(yesilLed, LOW);

      }
      else
      {
        digitalWrite(yesilLed, LOW);
        digitalWrite(kirmiziLed, HIGH);
        digitalWrite(D8, HIGH);//Buzzer Çalıştı
        delay(1000);
        digitalWrite(kirmiziLed, LOW);
        Blynk.logEvent("theft_alert","Odanın kapısı açıldı!!!");
        digitalWrite(D8, LOW);//Buzzer Durdu
      }

  }

         
}

///////////////////////////////////////////////////////mqtt bağlanma
void MQTT_connect()
{
    int8_t ret;
    if (mqtt.connected())
    {
        return;
    }
    Serial.print("Connecting to MQTT... ");
    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0)
    { 
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(500);  
        retries--;
        if (retries == 0)
        {
            while (1) ;
        }
    }
    Serial.println("MQTT Connected!");
}
      /*digitalWrite(D2, HIGH);//Kırmızı Yandı
      digitalWrite(D8, HIGH);//Buzzer Çalıştı
      delay(500);
      digitalWrite(D2, LOW);//Kırmızı Söndü
      //digitalWrite(D6, HIGH);//Mavi Yandı
      delay(500);
      //digitalWrite(D6, LOW);//Mavi Söndü
      digitalWrite(D4, HIGH);//Yeşil Yandı                      
      delay(500);
      digitalWrite(D4, LOW);//Yeşil Söndü
      digitalWrite(D8, LOW);//Buzzer Durdu 
      delay(1000);     */
