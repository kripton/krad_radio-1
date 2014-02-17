#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <math.h>
#include <asm/types.h>
#include <poll.h>
#include <dirent.h>
#include <linux/videodev2.h>
#include <linux/uvcvideo.h>
#include "krad_v4l2.h"
#include "krad_image_pool.h"
#include "krad_convert.h"

#define KR_V4L2_NIMAGES 12

typedef struct kr_v4l2 kr_v4l2;

typedef struct {
  struct v4l2_buffer buf;
  kr_image image;
  kr_v4l2 *kv;
} kv_image;

struct kr_v4l2 {
  int fd;
  kr_v4l2_info *info;
  kr_v4l2_path_info *path_info;
  kr_adapter *adapter;
  kr_adapter_path *adapter_path;
  uint32_t nimages;
  kv_image images[KR_V4L2_NIMAGES];
};

static int read_image(kr_v4l2 *kv, kr_image *image);
static int xioctl(int fd, int request, void *arg);
static int image_release(void *ptr);
static int capture_off(kr_v4l2 *kv);
static int capture_on(kr_v4l2 *kv);
static void unmap_images(kr_v4l2 *kv);
static int map_images(kr_v4l2 *kv);
static int set_params(kr_v4l2 *kv);
static void teardown(kr_v4l2 *kv);
static int kv_init(kr_v4l2 *kv);

static int xioctl(int fd, int request, void *arg) {
  int r;
  do r = ioctl(fd, request, arg);
  while (-1 == r && EINTR == errno);
  return r;
}

static int image_release(void *ptr) {
  kv_image *kvimage;
  if (ptr == NULL) return -1;
  kvimage = (kv_image *)ptr;
  if (-1 == xioctl(kvimage->kv->fd, VIDIOC_QBUF, &kvimage->buf)) {
    printke("V4L2: VIDIOC_QBUF");
    return -1;
  }
  return 0;
}

static int read_image(kr_v4l2 *kv, kr_image *image) {
  struct v4l2_buffer buf;
  if (kv == NULL) return -1;
  if (image == NULL) return -1;
  memset(&buf, 0, sizeof(buf));
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  if (-1 == xioctl(kv->fd, VIDIOC_DQBUF, &buf)) {
    switch (errno) {
      case EAGAIN:
        return 0;
      default:
        printke("Krad V4L2: VIDIOC_DQBUF");
        return -1;
    }
  }
  *image = kv->images[buf.index].image;
  image->ts = buf.timestamp;
  return 1;
}

static int capture_off(kr_v4l2 *kv) {
  enum v4l2_buf_type type;
  if (kv == NULL) return -1;
  if (kv->fd == -1) return -1;
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (kv->info->state != KR_V4L2_CAPTURE) {
    return 0;
  }
  if (-1 == xioctl(kv->fd, VIDIOC_STREAMOFF, &type)) {
    printke("V4L2: VIDIOC_STREAMOFF");
    kv->info->state = KR_V4L2_VOID;
    return -1;
  }
  kv->info->state = KR_V4L2_VOID;
  return 0;
}

static int capture_on(kr_v4l2 *kv) {
  uint32_t i;
  struct v4l2_buffer buf;
  enum v4l2_buf_type type;
  if (kv == NULL) return -1;
  if (kv->fd == -1) return -1;
  if (kv->nimages == 0) return -1;
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  for (i = 0; i < kv->nimages; i++) {
    memset(&buf, 0, sizeof(buf));
    buf.type = type;
    buf.index = i;
    buf.memory = V4L2_MEMORY_MMAP;
    if (-1 == xioctl(kv->fd, VIDIOC_QBUF, &buf)) {
      printke("V4L2: VIDIOC_QBUF");
      return -1;
    }
  }
  if (-1 == xioctl(kv->fd, VIDIOC_STREAMON, &type)) {
    printke("V4L2: VIDIOC_STREAMON");
    return -1;
  }
  kv->info->state = KR_V4L2_CAPTURE;
  return 0;
}

static void unmap_images(kr_v4l2 *kv) {
  int i;
  kv_image *kvimage;
  if (kv->nimages > 0) {
    capture_off(kv);
    for (i = 0; i < kv->nimages; i++) {
      kvimage = &kv->images[i];
      if (-1 == munmap(kvimage->image.px, kvimage->buf.length)) {
        printke("V4L2: munmap %d", i);
      }
    }
    kv->nimages = 0;
  }
}

static int map_images(kr_v4l2 *kv) {
  int i;
  struct v4l2_buffer buf;
  struct v4l2_requestbuffers req;
  kr_image *image;
  memset(&req, 0, sizeof(req));
  req.count = KR_V4L2_NIMAGES;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  if (-1 == xioctl(kv->fd, VIDIOC_REQBUFS, &req)) {
    if (EINVAL == errno) {
      printke("V4L2: device does not support memory mapping");
    } else {
      printke("V4L2: VIDIOC_REQBUFS");
    }
    return -1;
  }
  if (req.count < 2) {
    printke("V4L2: Insufficient buffer memory");
    return -1;
  }
  kv->nimages = req.count;
  printk("V4L2: %d buffer frames", kv->nimages);
  for (i = 0; i < kv->nimages; i++) {
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;
    if (-1 == xioctl(kv->fd, VIDIOC_QUERYBUF, &buf)) {
      printke("V4L2: VIDIOC_QUERYBUF");
      kv->nimages = 0;
      return -1;
    }
    kv->images[i].kv = kv;
    kv->images[i].buf = buf;
    kv->images[i].image.px = mmap(NULL, buf.length,
     PROT_READ | PROT_WRITE, MAP_SHARED, kv->fd, buf.m.offset);
    if (MAP_FAILED == kv->images[i].image.px) {
      printke("V4L2: mmap failure");
      kv->nimages = 0;
      return -1;
    }
    image = &kv->images[i].image;
    image->ppx[0] = image->px;
    image->ppx[1] = NULL;
    image->ppx[2] = NULL;
    image->ppx[3] = NULL;
    image->pps[0] = kv->path_info->width * 2;
    image->pps[1] = 0;  /* kv->path_info->width/2; */
    image->pps[2] = 0;  /* kv->path_info->width/2; */
    image->pps[3] = 0;
    image->w = kv->path_info->width;
    image->h = kv->path_info->height;
    image->fmt = PIX_FMT_YUYV422;
    image->release = image_release;
    image->owner = &kv->images[buf.index];
  }
  return 0;
}

static int set_params(kr_v4l2 *kv) {
  kr_v4l2_path_info *info;
  info = kv->path_info;
  struct v4l2_format format;
  struct v4l2_streamparm sps;
  memset(&sps, 0, sizeof(sps));
  memset(&format, 0, sizeof(format));
  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  format.fmt.pix.width = info->width;
  format.fmt.pix.height = info->height;
  format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  format.fmt.pix.field = V4L2_FIELD_ANY;
  if (-1 == xioctl(kv->fd, VIDIOC_S_FMT, &format)) {
    printke("V4L2: Unable to set desired video format");
    return -1;
  } else {
    printk("V4L2: Set %dx%d", info->width, info->height);
  }
  sps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (-1 == xioctl(kv->fd, VIDIOC_G_PARM, &sps)) {
    printke("V4L2: Unable to get capture parameters");
    return -1;
  }
  sps.parm.capture.timeperframe.numerator = info->den;
  sps.parm.capture.timeperframe.denominator = info->num;
  if (-1 == xioctl(kv->fd, VIDIOC_S_PARM, &sps)) {
    printke("V4L2: Unable to set desired capture parameters");
    printke("V4L2: error %d, %s", errno, strerror(errno));
    return -1;
  }
  return 0;
}

static void teardown(kr_v4l2 *kv) {
  if (kv->fd > -1) {
    capture_off(kv);
    unmap_images(kv);
    close(kv->fd);
    kv->fd = -1;
    kv->info->state = KR_V4L2_VOID;
  }
}

static int kv_init(kr_v4l2 *kv) {
  struct stat st;
  char device[128];
  struct v4l2_capability cap;
  snprintf(device, sizeof(device), "/dev/video%d", kv->info->dev);
  if (-1 == stat(device, &st)) {
    printke("V4L2: Can't reckon '%s': %d, %s", device, errno, strerror(errno));
    return -1;
  }
  if (!S_ISCHR(st.st_mode)) {
    printke("V4L2: %s is not a device", device);
    return -2;
  }
  kv->fd = open(device, O_RDWR | O_NONBLOCK, 0);
  if (-1 == kv->fd) {
    printke("V4L2: Access denied '%s': %d, %s", device, errno,
     strerror(errno));
    return -3;
  }
  if (-1 == xioctl(kv->fd, VIDIOC_QUERYCAP, &cap)) {
    printke("V4L2: VIDIOC_QUERYCAP");
    teardown(kv);
    return -4;
  } else {
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
      printke("V4L2: %s is not a video capture device", device);
      teardown(kv);
      return -5;
    }
    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
      printke("V4L2: %s no have streamin support we need", device);
      teardown(kv);
      return -6;
    }
  }
  kv->info->state = KR_V4L2_OPEN;
  return 0;
}

int kr_v4l2_lctl(kr_adapter_path *path, kr_patchset *patchset) {
  if (path == NULL) return -1;
  if (patchset == NULL) return -2;
  printk("V4L2: Adapter path control");
  printk("V4L2: Adapter path controlled");
  return 0;
}

int kr_v4l2_unlink(kr_adapter_path *path) {
  if (path == NULL) return -1;
  printk("V4L2: Adapter path remove");
  printk("V4L2: Adapter path remove");
  return 0;
}

int kr_v4l2_link(kr_adapter *adapter, kr_adapter_path *path) {
  int ret;
  kr_v4l2 *kv;
  if (adapter == NULL) return -1;
  kv = (kr_v4l2 *)adapter->handle;
  if (path == NULL) return -2;
  printk("V4L2: Adapter path create");
  kv->adapter_path = path;
  kv->path_info = &path->info->adp.v4l2_in;
  ret = set_params(kv);
  if (ret != 0) return ret;
  ret = map_images(kv);
  if (ret != 0) return ret;
  ret = capture_on(kv);
  if (ret != 0) return ret;
  printk("V4L2: Adapter path created");
  return ret;
}

int kr_v4l2_ctl(kr_adapter *adapter, kr_patchset *patchset) {
  if (adapter == NULL) return -1;
  if (patchset == NULL) return -2;
  printk("V4L2: Adapter control");
  printk("V4L2: Adapter controlled");
  return 0;
}

int kr_v4l2_close(kr_adapter *adapter) {
  kr_v4l2 *kv;
  if (adapter == NULL) return -1;
  kv = (kr_v4l2 *)adapter->handle;
  if (kv == NULL) return -1;
  printk("V4L2: Adapter closing");
  teardown(kv);
  free(kv);
  adapter->handle = NULL;
  printk("V4L2: adapter closed");
  return 0;
}

int kr_v4l2_open(kr_adapter *adapter) {
  int ret;
  kr_v4l2 *kv;
  if (adapter == NULL) return -1;
  printk("V4L2: Adapter opening");
  adapter->handle = kr_allocz(1, sizeof(kr_v4l2));
  kv = (kr_v4l2 *)adapter->handle;
  kv->adapter = adapter;
  kv->info = &adapter->info->adp.v4l2;
  ret = kv_init(kv);
  if (ret != 0) return ret;
  adapter->fd = kv->fd;
  printk("V4L2: adapter opened");
  return 0;
}
