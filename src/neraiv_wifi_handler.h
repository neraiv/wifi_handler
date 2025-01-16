#if !defined(NERAIV_WIFI_HANDLER_H)
#define NERAIV_WIFI_HANDLER_H

#include <vector>
#include <Arduino.h>
#include <string.h>

#if defined(ESP8266)
    #include <ESP8266WiFi.h>
#endif
#if defined(ESP32)
    #include <WiFi.h>
#endif

// Uncomment the following line to enable debug messages
#define WIFI_HANDLER_DEBUG

enum class WiFiError {
    SetupParametersDoesNotMatch,
    CantConnectWiFi,
    CantRetrieveIpAddress
};

enum class WiFiHandelerConnectionMethod {
    firstDiscovered,
    byStrongest,
    byPriority,
    byLatency
}

enum class WiFiHandelerSearchMethod{
    searchWhenDisconnected,
    searchEverySearchInterval,
    searchWhenBelowMinStrength,
    searchWhenBelowMinLatency
}

class WiFi_t {
public:
    WiFi_t(const String& ssid_, const String& password_, uint8_t priority_)
        : ssid(ssid_), password(password_), priority(priority_), strength(0) , latency(0) {}

    String ssid;
    String password;
    uint8_t priority;
    int strength;
    int latency;
};

class WiFiHandler {
public:
    WiFiHandler(const std::vector<WiFi_t*>& wifiList,
                WiFiHandelerConnectionMethod connectionMethod = WiFiHandelerConnectionMethod::firstDiscovered,
                WiFiHandelerSearchMethod searchMethod = WiFiHandelerSearchMethod::searchWhenDisconnected,
                unsigned long timeout = 0,
                uint searchInterval = 5000,
                int minStrength = 40,
                uint minLatency = 0)
        : wifiList(wifiList),
          connectionMethod(connectionMethod),
          searchMethod(searchMethod),
          timeout(timeout),
          searchInterval(searchInterval),
          minStrength(minStrength),
          minLatency(minLatency) {}

    std::vector<WiFi_t*> wifiList;
    
    bool connectWiFi();
    std::vector<int> searchWiFi();

    std::vector<int> orderBy(std::vector<WiFi_t*> list = wifiList, WiFiHandelerConnectionMethod method = connectionMethod);

    bool handleWiFi();

    WiFi_t* getCurrentWiFi();


private:
    int _selected_wifi_index = 0;
    bool _WiFiRequired;
    
    WiFiHandelerConnectionMethod connectionMethod;
    WiFiHandelerSearchMethod searchMethod;
    unsigned long timeout;
    unsigned long searchInterval;
    uint minStrength;
    uint minLatency;
};

#endif // NERAIV_WIFI_HANDLER_H
