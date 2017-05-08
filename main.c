#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

static bool has_latency_check = false;
static LPALGETSOURCEDVSOFT alGetSourcedvSOFT;

int main() {

  ALCdevice *device;
  ALCcontext *context;
  const ALCchar *desired_device = NULL;
  int found_device_number = 0;

  printf("Enumerating OpenAL audio devices:\n");

  if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE) {
      // Enumeration is supported
      const ALCchar *devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
      const ALCchar *device = devices, *next = devices + 1;
      size_t len = 0;

      // list and select the first device as the desired device
      printf("---------------------------------\n");
      while (device && *device != '\0' && next && *next != '\0') {
          printf("%d: %s\n", found_device_number, device);
          if (found_device_number == 0) {
              desired_device = device;
          }
          found_device_number++;
          len = strlen(device);
          device += (len + 1);
          next += (len + 2);
      }
      printf("---------------------------------\n");
  } else {
      printf("Could not enumerate OpenAL devices.\n");
  }

  printf("Opening OpenAL audio device \"%s\":\n", desired_device);

  /* Open an audio device */
  device = alcOpenDevice(desired_device);
  if (!device) {
      fprintf(stderr, "OpenAL: could not open device - exiting\n");
      return 1;
  } else {
      printf("OpenAL: Could successfully open a device\n");
  }

  context = alcCreateContext(device, NULL);
  if (!context) {
    fprintf(stderr, "OpenAL: could not create context - exiting\n");
    return 1;
  }

  if (alcMakeContextCurrent(context) == ALC_FALSE) {
    fprintf(stderr, "OpenAL: could not make context current - exiting\n");
    return 1;
  }

  if (!alIsExtensionPresent("AL_SOFT_source_length")) {
    fprintf(stderr, "Required AL_SOFT_source_length not supported - exiting\n");
    return 1;
  }

  if (!alIsExtensionPresent("AL_SOFT_source_latency")) {
    fprintf(stderr, "AL_SOFT_source_latency not supported, audio may be a bit laggy.\n");
  } else {
    alGetSourcedvSOFT = alGetProcAddress("alGetSourcedvSOFT");
    has_latency_check = true;
  }

  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);

  printf("Done.\n");

  return 0;

}
