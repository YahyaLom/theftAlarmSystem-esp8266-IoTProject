//D2 Led Kırmızı
//D4 Led Yeşil
//D6 Led mavi
//D8 Buzzer
//D1 Read Röle

#define BLYNK_TEMPLATE_ID "TMPL6GKcE0zol"
#define BLYNK_TEMPLATE_NAME "Theft Alarm"
#define BLYNK_AUTH_TOKEN "KZZPqKfrONN7JtUJmYFZjb7f2eMjuoFN"


#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>



///////////////////////////////////////////////// Bylnk Auth Token
char auth[]="KZZPqKfrONN7JtUJmYFZjb7f2eMjuoFN";


////////////////////////////////////////////////// Wi-Fi Bağlanma Anahtarları
char WLAN_SSID[]="Yahya";
char WLAN_PASSWORD[]="23456789";

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
  wifiSetup();
  Blynk.begin(auth,  WLAN_SSID,  WLAN_PASSWORD);

  //timer.setInterval(5000L, event);
}


void loop() 
{

  if(digitalRead(readRole)==1)
  {
    digitalWrite(yesilLed, HIGH);
    //Blynk.email("yahya61ts@gmail.com", "Alert", "Theft Alert in Home");
    //Blynk.notify("Alert : Theft Alert in Home");
    
    delay(1000);
    digitalWrite(yesilLed, LOW);

  }
  else
  {
    digitalWrite(kirmiziLed, HIGH);
    digitalWrite(D8, HIGH);//Buzzer Çalıştı
    delay(1000);
    digitalWrite(kirmiziLed, LOW);
    Blynk.logEvent("theft_alert","Odanın kapısı açıldı!!!");
    digitalWrite(D8, LOW);//Buzzer Durdu
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
}