
#include "secure_config.h"

const char* SecureConfig::CONFIG_NAMESPACE = "hydra_secure";
SecureConfig secureConfig;

bool SecureConfig::init() {
    return prefs.begin(CONFIG_NAMESPACE, false);
}

bool SecureConfig::setWiFiCredentials(const String& ssid, const String& password) {
    if (!prefs.putString("wifi_ssid", ssid)) return false;
    if (!prefs.putString("wifi_pass", password)) return false;
    return true;
}

bool SecureConfig::getWiFiCredentials(String& ssid, String& password) {
    ssid = prefs.getString("wifi_ssid", "");
    password = prefs.getString("wifi_pass", "");
    return (ssid.length() > 0 && password.length() > 0);
}

bool SecureConfig::setAPIKey(const String& service, const String& key) {
    String keyName = "api_" + service;
    return prefs.putString(keyName.c_str(), key);
}

bool SecureConfig::getAPIKey(const String& service, String& key) {
    String keyName = "api_" + service;
    key = prefs.getString(keyName.c_str(), "");
    return (key.length() > 0);
}

void SecureConfig::clearAll() {
    prefs.clear();
}
