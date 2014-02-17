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

#define KR_V4L2_BUFS_DEF 12

typedef struct kr_v4l2 kr_v4l2;

typedef struct {
  void *start;
  struct v4l2_buffer buf;
  kr_v4l2 *v4l2;
} kr_v4l2_buf;

struct kr_v4l2 {
  int fd;
  kr_v4l2_info *info;
  kr_v4l2_path_info *path_info;
  kr_adapter *adapter;
  kr_adapter_path *adapter_path;
  kr_image_pool *pool;
  uint32_t nbufs;
  kr_v4l2_buf bufs[KR_V4L2_BUFS_DEF];
};

/*
int kr_v4l2_read(kr_v4l2 *v4l2, kr_image *image);
int kr_v4l2_poll(kr_v4l2 *v4l2, int ms);
int kr_v4l2_capture(kr_v4l2 *v4l2, int on);
int kr_v4l2_mode_set(kr_v4l2 *v4l2, kr_v4l2_mode *mode);
*/
static int xioctl(int fd, int request, void *arg);
/*
static int kr_v4l2_buf_release(void *ptr);
static void kr_v4l2_unmap(kr_v4l2 *v4l2);
static void kr_v4l2_map(kr_v4l2 *v4l2);
*/
static int kr_v4l2_mode_set(kr_v4l2 *kv);
static void v4l2_close(kr_v4l2 *kv);
static void v4l2_open(kr_v4l2 *kv);

static int xioctl(int fd, int request, void *arg) {
  int r;
  do r = ioctl(fd, request, arg);
  while (-1 == r && EINTR == errno);
  return r;
}
/*
int kr_v4l2_buf_release(void *ptr) {
  kr_v4l2_buf *buf;
  if (ptr == NULL) return -1;
  buf = (kr_v4l2_buf *)ptr;
  if (-1 == xioctl(buf->v4l2->fd, VIDIOC_QBUF, &buf->buf)) {
    printke("Krad V4L2: VIDIOC_QBUF");
    return -1;
  }
  return 0;
}

int kr_v4l2_poll(kr_v4l2 *v4l2, int ms) {

  struct pollfd fds[1];

  if (v4l2 == NULL) return -1;
  if (v4l2->fd == -1) return -1;
  if (v4l2->info.state != KR_V4L2_CAPTURE) return -1;
  fds[0].fd = v4l2->fd;
  fds[0].events = POLLIN;

  return poll(fds, 1, ms);
}

int kr_v4l2_read(kr_v4l2 *v4l2, kr_image *image) {

  struct v4l2_buffer buf;

  if (v4l2 == NULL) return -1;
  if (image == NULL) return -1;

  memset(&buf, 0, sizeof(buf));
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;

  if (-1 == xioctl(v4l2->fd, VIDIOC_DQBUF, &buf)) {
    switch (errno) {
      case EAGAIN:
        return 0;
      default:
        printke("Krad V4L2: VIDIOC_DQBUF");
        return -1;
    }
  }

  *//*  v4l2->timestamp = buf.timestamp; *//*
  image->px = v4l2->bufs[buf.index].start;
  image->ppx[0] = image->px;
  image->ppx[1] = NULL;
  image->ppx[2] = NULL;
  image->ppx[3] = NULL;
  image->pps[0] = v4l2->info.width * 2;
  image->pps[1] = 0; *//*v4l2->info.width/2;*//*
  image->pps[2] = 0; *//*v4l2->info.width/2;*//*
  image->pps[3] = 0;
  image->w = v4l2->info.width;
  image->h = v4l2->info.height;
  image->fmt = PIX_FMT_YUYV422;
  image->release_cb = kr_v4l2_buf_release;
  image->owner = &v4l2->bufs[buf.index];
  return 1;
}

int kr_v4l2_capture(kr_v4l2 *v4l2, int on) {

  uint32_t i;
  struct v4l2_buffer buf;
  enum v4l2_buf_type type;

  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if (v4l2 == NULL) return -1;
  if (v4l2->fd == -1) return -1;

  if (on == 0) {
    if (v4l2->info.state != KR_V4L2_CAPTURE) {
      return 0;
    }
    if (-1 == xioctl (v4l2->fd, VIDIOC_STREAMOFF, &type)) {
      printke("Krad V4L2: VIDIOC_STREAMOFF");
      v4l2->info.state = KR_V4L2_VOID;
      return -1;
    }
    v4l2->info.state = KR_V4L2_VOID;
    return 0;
  }

  if (v4l2->nbufs == 0) return -1;
  for (i = 0; i < v4l2->nbufs; i++) {
    memset(&buf, 0, sizeof(buf));
    buf.type = type;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;
    if (-1 == xioctl(v4l2->fd, VIDIOC_QBUF, &buf)) {
      printke("Krad V4L2: VIDIOC_QBUF");
      return -1;
    }
  }

  if (-1 == xioctl(v4l2->fd, VIDIOC_STREAMON, &type)) {
    printke("Krad V4L2: VIDIOC_STREAMON");
    return -1;
  }
  v4l2->info.state = KR_V4L2_CAPTURE;
  return 0;
}

static void kr_v4l2_unmap(kr_v4l2 *v4l2) {
  int i;
  if (v4l2->nbufs > 0) {
    kr_v4l2_capture(v4l2, 0);
    for (i = 0; i < v4l2->nbufs; i++) {
      if (-1 == munmap(v4l2->bufs[i].start, v4l2->bufs[i].buf.length)) {
        printke("Krad V4L2: munmap %d", i);
      }
    }
    v4l2->nbufs = 0;
  }
}

static void kr_v4l2_map(kr_v4l2 *v4l2) {
  int i;
  struct v4l2_buffer buf;
  struct v4l2_requestbuffers req;
  memset(&req, 0, sizeof(req));
  req.count = KR_V4L2_BUFS_DEF;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  if (-1 == xioctl(v4l2->fd, VIDIOC_REQBUFS, &req)) {
    if (EINVAL == errno) {
      printke("Krad V4L2: device does not support memory mapping");
    } else {
      printke("Krad V4L2: VIDIOC_REQBUFS");
    }
    return;
  }
  if (req.count < 2) {
    printke("Krad V4L2: Insufficient buffer memory");
    return;
  }
  v4l2->nbufs = req.count;
  printk("Krad V4L2: %d buffers", v4l2->nbufs);
  for (i = 0; i < v4l2->nbufs; i++) {
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;
    if (-1 == xioctl(v4l2->fd, VIDIOC_QUERYBUF, &buf)) {
      printke("Krad V4L2: VIDIOC_QUERYBUF");
      v4l2->nbufs = 0;
      return;
    }
    v4l2->bufs[i].v4l2 = v4l2;
    v4l2->bufs[i].buf = buf;
    v4l2->bufs[i].start = mmap(NULL, buf.length,
     PROT_READ | PROT_WRITE, MAP_SHARED, v4l2->fd, buf.m.offset);
    if (MAP_FAILED == v4l2->bufs[i].start) {
      printke("Krad V4L2: mmap");
      v4l2->nbufs = 0;
      return;
    }
  }
}
*/

static int kr_v4l2_mode_set(kr_v4l2 *kv) {
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

static void v4l2_close(kr_v4l2 *v4l2) {
  if (v4l2->fd > -1) {
    //kr_v4l2_unmap(v4l2);
    close(v4l2->fd);
    v4l2->fd = -1;
    v4l2->info->state = KR_V4L2_VOID;
  }
}

static void v4l2_open(kr_v4l2 *v4l2) {
  struct stat st;
  char device[128];
  struct v4l2_capability cap;
  snprintf(device, sizeof(device), "/dev/video%d", v4l2->info->dev);
  if (-1 == stat(device, &st)) {
    printke("Krad V4L2: Cannot identify '%s': %d, %s", device, errno,
     strerror(errno));
    return;
  }
  if (!S_ISCHR(st.st_mode)) {
    printke("Krad V4L2: %s is no device", device);
    return;
  }
  v4l2->fd = open(device, O_RDWR | O_NONBLOCK, 0);
  if (-1 == v4l2->fd) {
    printke("Krad V4L2: Cannot open '%s': %d, %s", device, errno,
     strerror(errno));
    return;
  }
  if (-1 == xioctl(v4l2->fd, VIDIOC_QUERYCAP, &cap)) {
    printke("Krad V4L2: VIDIOC_QUERYCAP");
    v4l2_close(v4l2);
    return;
  } else {
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
      printke("Krad V4L2: %s is no video capture device", device);
      v4l2_close(v4l2);
      return;
    }
    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
      printke("Krad V4L2: %s does not support streaming i/o", device);
      v4l2_close(v4l2);
      return;
    }
  }
  v4l2->info->state = KR_V4L2_OPEN;
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
  ret = kr_v4l2_mode_set(kv);
  //kr_v4l2_map(v4l2);
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
  v4l2_close(kv);
  free(kv);
  adapter->handle = NULL;
  printk("V4L2: adapter closed");
  return 0;
}

int kr_v4l2_open(kr_adapter *adapter) {
  kr_v4l2 *kv;
  if (adapter == NULL) return -1;
  printk("V4L2: Adapter opening");
  adapter->handle = kr_allocz(1, sizeof(kr_v4l2));
  kv = (kr_v4l2 *)adapter->handle;
  kv->adapter = adapter;
  kv->info = &adapter->info->adp.v4l2;
  v4l2_open(kv);
  adapter->fd = kv->fd;
  printk("V4L2: adapter opened");
  return 0;
}
