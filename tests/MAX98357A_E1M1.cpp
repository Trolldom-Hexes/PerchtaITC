#include <driver/i2s.h>

const int pinBCLK = 27;
const int pinLRC  = 26;
const int pinDOUT = 25;

const int sampleRate = 16000;
const int amplitude = 6000;

// Notes and frequencies
const int NOTE_E3  = 165;
const int NOTE_F3  = 175;
const int NOTE_FS3 = 185;
const int NOTE_G3  = 196;
const int NOTE_A3  = 220;
const int NOTE_C4  = 261;
const int NOTE_D4  = 294;
const int NOTE_DS4 = 311;

int melody[] = {
  // Riff Part 1
  NOTE_E3, NOTE_E3, NOTE_G3, NOTE_E3, NOTE_FS3, NOTE_E3, NOTE_F3, NOTE_E3,
  NOTE_E3, NOTE_G3, NOTE_E3, NOTE_FS3, NOTE_F3, NOTE_FS3,
  NOTE_E3, NOTE_E3, NOTE_G3, NOTE_E3, NOTE_FS3, NOTE_E3, NOTE_F3, NOTE_E3,
  NOTE_E3, NOTE_A3, NOTE_G3, NOTE_E3,
  // Riff Part 2 (Variation)
  NOTE_E3, NOTE_E3, NOTE_G3, NOTE_E3, NOTE_FS3, NOTE_E3, NOTE_F3, NOTE_E3,
  NOTE_E3, NOTE_G3, NOTE_E3, NOTE_FS3, NOTE_F3, NOTE_FS3,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_A3, NOTE_G3, NOTE_E3,
  // Melody build-up
  NOTE_E3, NOTE_G3, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_A3, NOTE_G3,
  NOTE_E3, NOTE_F3, NOTE_G3, NOTE_F3, NOTE_E3
};

int durations[] = {
  110, 110, 110, 110, 110, 110, 110, 110,
  110, 110, 110, 110, 110, 220,
  110, 110, 110, 110, 110, 110, 110, 110,
  130, 130, 130, 260,
  110, 110, 110, 110, 110, 110, 110, 110,
  110, 110, 110, 110, 110, 220,
  130, 130, 130, 130, 130, 130, 260,
  130, 130, 130, 130, 130, 130, 130, 130,
  130, 130, 130, 130, 300
};

const int noteCount = sizeof(melody) / sizeof(melody[0]);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = sampleRate,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = pinBCLK,
    .ws_io_num = pinLRC,
    .data_out_num = pinDOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_zero_dma_buffer(I2S_NUM_0);
}

void playTone(int frequency, int durationMs) {
  int totalSamples = (sampleRate * durationMs) / 1000;
  int bufferSize = 256;
  int16_t samples[bufferSize];

  int samplesPlayed = 0;
  int rampSamples = sampleRate * 0.015; // 15ms Fade-In/Out to prevent clicking

  while (samplesPlayed < totalSamples) {
    int chunk = min(bufferSize / 2, totalSamples - samplesPlayed);
    for (int j = 0; j < chunk * 2; j += 2) {
      int currentSampleIndex = samplesPlayed + j / 2;
      float t = (float)currentSampleIndex / sampleRate;
      
      float env = 1.0;
      // Fade-In at the beginning
      if (currentSampleIndex < rampSamples) {
        env = (float)currentSampleIndex / rampSamples;
      }
      // Fade-Out at the end
      int remaining = totalSamples - currentSampleIndex;
      if (remaining < rampSamples) {
        env = (float)remaining / rampSamples;
      }

      int16_t sample = amplitude * env * sin(2.0 * PI * frequency * t);
      samples[j] = sample;
      samples[j + 1] = sample;
    }
    size_t bytesWritten;
    i2s_write(I2S_NUM_0, samples, chunk * 4, &bytesWritten, portMAX_DELAY);
    samplesPlayed += chunk;
  }
}

void loop() {
  for (int i = 0; i < noteCount; i++) {
    playTone(melody[i], durations[i]);
  }
  delay(3000); // Pause after playback before restarting loop
}