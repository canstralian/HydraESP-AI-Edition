
#ifndef SECURE_CONFIG_H
#define SECURE_CONFIG_H

#include <Arduino.h>
#include <Preferences.h>

/**
 * @brief Secure configuration manager for sensitive data
 */
class SecureConfig {
private:
    Preferences prefs;
    static const char* CONFIG_NAMESPACE;

public:
    bool init();
    bool setWiFiCredentials(const String& ssid, const String& password);
    bool getWiFiCredentials(String& ssid, String& password);
    bool setAPIKey(const String& service, const String& key);
    bool getAPIKey(const String& service, String& key);
    void clearAll();
};

extern SecureConfig secureConfig;

#endif // SECURE_CONFIG_H
