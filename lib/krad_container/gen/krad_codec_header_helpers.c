#include "krad_codec_header_helpers.h"

int krad_codec_t_to_index(int val) {
  switch (val) {
    case NOCODEC:
      return 0;
    case OPUS:
      return 1;
    case FLAC:
      return 2;
    case VP8:
      return 3;
    case THEORA:
      return 4;
    case MJPEG:
      return 5;
    case PNG:
      return 6;
    case CODEC2:
      return 7;
    case SKELETON:
      return 8;
    case Y4M:
      return 9;
    case DAALA:
      return 10;
    case KVHS:
      return 11;
    case H264:
      return 12;
    case VORBIS:
      return 13;
  }
  return -1;
}

char *kr_strfr_krad_codec_t(int val) {
  switch (val) {
    case NOCODEC:
      return "nocodec";
    case OPUS:
      return "opus";
    case FLAC:
      return "flac";
    case VP8:
      return "vp8";
    case THEORA:
      return "theora";
    case MJPEG:
      return "mjpeg";
    case PNG:
      return "png";
    case CODEC2:
      return "codec2";
    case SKELETON:
      return "skeleton";
    case Y4M:
      return "y4m";
    case DAALA:
      return "daala";
    case KVHS:
      return "kvhs";
    case H264:
      return "h264";
    case VORBIS:
      return "vorbis";
  }
  return NULL;
}

int kr_strto_krad_codec_t(char *string) {
  if (!strcmp(string,"nocodec")) {
    return NOCODEC;
  }
  if (!strcmp(string,"opus")) {
    return OPUS;
  }
  if (!strcmp(string,"flac")) {
    return FLAC;
  }
  if (!strcmp(string,"vp8")) {
    return VP8;
  }
  if (!strcmp(string,"theora")) {
    return THEORA;
  }
  if (!strcmp(string,"mjpeg")) {
    return MJPEG;
  }
  if (!strcmp(string,"png")) {
    return PNG;
  }
  if (!strcmp(string,"codec2")) {
    return CODEC2;
  }
  if (!strcmp(string,"skeleton")) {
    return SKELETON;
  }
  if (!strcmp(string,"y4m")) {
    return Y4M;
  }
  if (!strcmp(string,"daala")) {
    return DAALA;
  }
  if (!strcmp(string,"kvhs")) {
    return KVHS;
  }
  if (!strcmp(string,"h264")) {
    return H264;
  }
  if (!strcmp(string,"vorbis")) {
    return VORBIS;
  }

  return -1;
}

int krad_container_type_t_to_index(int val) {
  switch (val) {
    case MKV:
      return 0;
    case OGG:
      return 1;
    case TOGG:
      return 2;
    case NATIVEFLAC:
      return 3;
    case Y4MFILE:
      return 4;
    case KUDP:
      return 5;
  }
  return -1;
}

char *kr_strfr_krad_container_type_t(int val) {
  switch (val) {
    case MKV:
      return "mkv";
    case OGG:
      return "ogg";
    case TOGG:
      return "togg";
    case NATIVEFLAC:
      return "nativeflac";
    case Y4MFILE:
      return "y4mfile";
    case KUDP:
      return "kudp";
  }
  return NULL;
}

int kr_strto_krad_container_type_t(char *string) {
  if (!strcmp(string,"mkv")) {
    return MKV;
  }
  if (!strcmp(string,"ogg")) {
    return OGG;
  }
  if (!strcmp(string,"togg")) {
    return TOGG;
  }
  if (!strcmp(string,"nativeflac")) {
    return NATIVEFLAC;
  }
  if (!strcmp(string,"y4mfile")) {
    return Y4MFILE;
  }
  if (!strcmp(string,"kudp")) {
    return KUDP;
  }

  return -1;
}

