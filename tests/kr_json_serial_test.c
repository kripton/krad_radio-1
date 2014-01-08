#include <stdio.h>
#include "../lib/krad_radio/kr_client.h"
//#include "../lib/krad_transponder/gen/krad_transponder_common_helpers.h"

int main(int argc, char const *argv[]) {
  char json[4096];
  char text[4096];
  int res;
  kr_transponder_path_info info1;
  kr_transponder_path_info info2;

  memset(json,0,sizeof(json));
  memset(text,0,sizeof(text));
  memset(&info1,0,sizeof(info1));

  info1.input.type = KR_XPDR_COMPOSITOR;
  info1.input.info.compositor_path_info.width = 1280;
  info1.input.info.compositor_path_info.height = 720;
  info1.output.type = KR_XPDR_MIXER;
  info1.output.info.mixer_path_info.volume[0] = 75.00f;
  info1.output.info.mixer_path_info.volume[1] = 75.00f;

  res = kr_transponder_path_info_to_json(json,&info1,sizeof(json));

  if (res > 0) {
    printf("kr_transponder_path_info serialized correctly!\n");
    printf("%s\n",json);
  }

  res = kr_transponder_path_info_fr_json(json,&info2);

  if (res > 0) {
    printf("kr_transponder_path_info deserialized correctly!\n");
    res = kr_transponder_path_info_to_text(text,&info2,sizeof(text));
    if (res > 0) 
      printf("%s\n",text);
  } else {
    printf("failed at member %d!\n",-res);
  }

  return 0;
}
