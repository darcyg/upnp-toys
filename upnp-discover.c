#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <upnp/upnp.h>

#define MEDIA_RENDERER_TYPE "urn:schemas-upnp-org:device:MediaRenderer:1"
#define IS_UPNP_FAIL(code) ((code) != UPNP_E_SUCCESS && (code) != UPNP_E_INIT)

#define END_OF_OPTIONS {0, 0, 0, 0}
#define OPTION_INTERFACE 'i'

const struct option options[] = {
  {"interface", required_argument, NULL, OPTION_INTERFACE},
  END_OF_OPTIONS,
};

const int ASYNC_SEARCH_CALLBACK = 1;
int on_client_event(Upnp_EventType event_type, void *event, void *cookie) {
  if (cookie != &ASYNC_SEARCH_CALLBACK) return 0;
  switch (event_type) {
    case UPNP_DISCOVERY_SEARCH_TIMEOUT:
      printf("Search timeout\n");
    break;
    case UPNP_DISCOVERY_SEARCH_RESULT:
      printf("Discovered\n");
    break;
    default: break;
  }
  return 0;
}

int main(int argc, char **argv) {
  int option_index, option_value = 0;
  const char *interface = NULL;
  while (option_value != -1) {
    option_value = getopt_long(argc, argv, "", options, &option_index);
    if (option_value == OPTION_INTERFACE) {
        interface = optarg;
    }
  }

  int upnp_result;
  UpnpClient_Handle client;

  upnp_result = UpnpInit2(interface, 0);
  if (IS_UPNP_FAIL(upnp_result)) return upnp_result;

  upnp_result = UpnpRegisterClient(on_client_event, NULL, &client);
  if (IS_UPNP_FAIL(upnp_result)) return upnp_result;

  UpnpSearchAsync(client, 10, MEDIA_RENDERER_TYPE, &ASYNC_SEARCH_CALLBACK);
  while (1);
}
