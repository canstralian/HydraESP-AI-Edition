
#include <Arduino.h>
#include <unity.h>
#include "../src/ai_states.cpp"

// Test helper to create sensor data
sensor_data_t create_test_sensor_data(uint32_t wifi_count, uint32_t ble_count, 
                                     uint32_t free_memory, bool user_interaction) {
    sensor_data_t data = {0};
    data.wifi_networks_count = wifi_count;
    data.ble_devices_count = ble_count;
    data.free_memory = free_memory;
    data.user_interaction = user_interaction;
    data.wifi_signal_strength = -50; // Default signal strength
    data.ble_signal_strength = -50;  // Default signal strength
    data.uptime_seconds = 100;       // Default uptime
    data.sd_card_present = true;     // Default SD card present
    return data;
}

void test_ai_state_idle() {
    sensor_data_t data = create_test_sensor_data(0, 0, 50000, false);
    ai_state_t state = analyze_behavior(&data);
    TEST_ASSERT_EQUAL(AI_STATE_IDLE, state);
}

void test_ai_state_sniffing() {
    sensor_data_t data = create_test_sensor_data(15, 0, 50000, false);
    ai_state_t state = analyze_behavior(&data);
    TEST_ASSERT_EQUAL(AI_STATE_SNIFFING, state);
}

void test_ai_state_tracking() {
    sensor_data_t data = create_test_sensor_data(0, 6, 50000, false);
    data.ble_signal_strength = -40; // Strong signal
    ai_state_t state = analyze_behavior(&data);
    TEST_ASSERT_EQUAL(AI_STATE_TRACKING, state);
}

void test_ai_state_error_low_memory() {
    sensor_data_t data = create_test_sensor_data(0, 0, 5000, false); // Low memory
    ai_state_t state = analyze_behavior(&data);
    TEST_ASSERT_EQUAL(AI_STATE_ERROR, state);
}

void test_ai_state_string_conversion() {
    TEST_ASSERT_EQUAL_STRING("Idle", ai_state_to_string(AI_STATE_IDLE));
    TEST_ASSERT_EQUAL_STRING("Sniffing", ai_state_to_string(AI_STATE_SNIFFING));
    TEST_ASSERT_EQUAL_STRING("Error", ai_state_to_string(AI_STATE_ERROR));
}

void test_ai_state_emoji_conversion() {
    TEST_ASSERT_EQUAL_STRING("😊", ai_state_to_emoji(AI_STATE_IDLE));
    TEST_ASSERT_EQUAL_STRING("👃", ai_state_to_emoji(AI_STATE_SNIFFING));
    TEST_ASSERT_EQUAL_STRING("💀", ai_state_to_emoji(AI_STATE_ERROR));
}

void setup() {
    UNITY_BEGIN();
    
    RUN_TEST(test_ai_state_idle);
    RUN_TEST(test_ai_state_sniffing);
    RUN_TEST(test_ai_state_tracking);
    RUN_TEST(test_ai_state_error_low_memory);
    RUN_TEST(test_ai_state_string_conversion);
    RUN_TEST(test_ai_state_emoji_conversion);
    
    UNITY_END();
}

void loop() {
    // Tests run once in setup()
}
