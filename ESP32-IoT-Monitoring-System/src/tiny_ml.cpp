#include "tiny_ml.h"
#include "global.h"


namespace {
    tflite::ErrorReporter *error_reporter = nullptr;
    const tflite::Model *model = nullptr;
    tflite::MicroInterpreter *interpreter = nullptr;
    TfLiteTensor *input = nullptr;
    TfLiteTensor *output = nullptr;
    constexpr int kTensorArenaSize = 16 * 1024; 
    uint8_t tensor_arena[kTensorArenaSize];
} 

void setupTinyML()
{
    Serial.println("Info: [Tiny ML] Init....");
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    model = tflite::GetModel(dht_anomaly_model_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        error_reporter->Report("Info: [Tiny ML] Model schema version %d != %d", model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    static tflite::AllOpsResolver resolver;
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    if (interpreter->AllocateTensors() != kTfLiteOk)
    {
        error_reporter->Report("Error: AllocateTensors() failed!");
        return;
    }

    input = interpreter->input(0);
    output = interpreter->output(0);

    Serial.println("Info: [Tiny ML] Initialized.");
}

void tiny_ml_task(void *pvParameters)
{
    setupTinyML();

    auto getStatus = [](float value) -> const char* {
        if (value >= 0.7f) return "WARNING";
        if (value <= 0.3f) return "STABLE";
        return "MEDIUM";
    };

    while (1)
    {
        input->data.f[0] = g_temperature;
        input->data.f[1] = g_humidity;
        input->data.f[2] = g_no2;
        input->data.f[3] = g_pm25;
        input->data.f[4] = g_pm10;

        if (interpreter->Invoke() != kTfLiteOk)
        {
            Serial.println("Error: Invoke failed");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        float out_NO2      = output->data.f[0];
        float out_PM25     = output->data.f[1];
        float out_PM10     = output->data.f[2];
        float out_temp     = output->data.f[3];
        float out_humidity = output->data.f[4];

        Serial.println("===== TinyML Inference =====");
        Serial.printf("NO2        : %.2f %% -> %s\n", out_NO2, getStatus(out_NO2));
        Serial.printf("PM2.5      : %.2f %% -> %s\n", out_PM25, getStatus(out_PM25));
        Serial.printf("PM10       : %.2f %% -> %s\n", out_PM10, getStatus(out_PM10));
        Serial.printf("Temperature: %.2f %% -> %s\n", out_temp, getStatus(out_temp));
        Serial.printf("Humidity   : %.2f %% -> %s\n", out_humidity, getStatus(out_humidity));
        Serial.println("============================");

        vTaskDelay(5000 / portTICK_PERIOD_MS); 
    }
}