#include "krad_eq.h"

typedef struct {
  biquad filter;
  float db;
  float bw;
  float hz;
  kr_easer db_easer;
  kr_easer bw_easer;
  kr_easer hz_easer;
} kr_eq_band;

struct kr_eq {
  float new_sample_rate;
  float sample_rate;
  kr_eq_band band[KR_EQ_MAX_BANDS];
};

void kr_eq_band_set_db(kr_eq *eq, int band_num, float db, int duration,
 kr_easing easing, void *user) {
  if (band_num >= KR_EQ_MAX_BANDS) return;
  db = LIMIT(db, KR_EQ_DB_MIN, KR_EQ_DB_MAX);
  kr_easer_set(&eq->band[band_num].db_easer, db, duration, easing, user);
}

void kr_eq_band_set_bw(kr_eq *eq, int band_num, float bw, int duration,
 kr_easing easing, void *user) {
  if (band_num >= KR_EQ_MAX_BANDS) return;
  bw = LIMIT(bw, KR_EQ_BW_MIN, KR_EQ_BW_MAX);
  kr_easer_set(&eq->band[band_num].bw_easer, bw, duration, easing, user);
}

void kr_eq_band_set_hz(kr_eq *eq, int band_num, float hz, int duration,
 kr_easing easing, void *user) {
  if (band_num >= KR_EQ_MAX_BANDS) return;
  hz = LIMIT(hz, KR_EQ_HZ_MIN, KR_EQ_HZ_MAX);
  kr_easer_set(&eq->band[band_num].hz_easer, hz, duration, easing, user);
}

//void kr_eq_process (kr_eq_t *kr_eq, float *input, float *output, int num_samples) {

void kr_eq_process2(kr_eq *eq, float *input, float *output, int num_samples,
 int broadcast) {
  int b, s;
  int recompute;
  int recompute_default;
  void *ptr;
  ptr = NULL;
  if (eq->new_sample_rate != eq->sample_rate) {
    eq->sample_rate = eq->new_sample_rate;
    recompute_default = 1;
  } else {
    recompute_default = 0;
  }
  for (b = 0; b < KR_EQ_MAX_BANDS; b++) {
    //if ((kr_eq->band[b].db == 0.0f) && (!kr_eq->band[b].krad_easing_db.active)) {
    //  continue;
    //}
   // eq->address.sub_id2 = b;
    recompute = recompute_default;
    if (kr_easer_active(&eq->band[b].hz_easer)) {
      eq->band[b].hz = kr_easer_process(&eq->band[b].hz_easer, eq->band[b].hz, &ptr);
      recompute = 1;
      if (broadcast == 1) {
//        krad_radio_broadcast_subunit_control(eq->mixer->broadcaster,
//         &eq->address, HZ, eq->band[b].hz, ptr);
      }
    }
    if (kr_easer_active(&eq->band[b].db_easer)) {
      eq->band[b].db = kr_easer_process(&eq->band[b].db_easer, eq->band[b].db, &ptr);
      recompute = 1;
      if (broadcast == 1) {
//        krad_radio_broadcast_subunit_control(eq->mixer->broadcaster, &eq->address, DB, eq->band[b].db, ptr);
      }
    }
    if (kr_easer_active(&eq->band[b].bw_easer)) {
      eq->band[b].bw = kr_easer_process(&eq->band[b].bw_easer, eq->band[b].bw, &ptr);
      recompute = 1;
      if (broadcast == 1) {
//        krad_radio_broadcast_subunit_control(eq->mixer->broadcaster, &eq->address, BW, eq->band[b].bw, ptr);
      }
    }
    if (recompute == 1) {
      eq_set_params(&eq->band[b].filter, eq->band[b].hz, eq->band[b].db,
       eq->band[b].bw, eq->sample_rate);
    }
    if (eq->band[b].db != 0.0f) {
      for (s = 0; s < num_samples; s++) {
        output[s] = biquad_run(&eq->band[b].filter, input[s]);
      }
    }
  }
}

void kr_eq_set_sample_rate(kr_eq *eq, int sample_rate) {
  eq->new_sample_rate = sample_rate;
}

kr_eq *kr_eq_create(int sample_rate) {
  int b;
  float hz;
  kr_eq *eq;
  eq = kr_allocz(1, sizeof(kr_eq));
  eq->new_sample_rate = sample_rate;
  eq->sample_rate = eq->new_sample_rate;
  hz = 30.0;
  for (b = 0; b < KR_EQ_MAX_BANDS; b++) {
    eq->band[b].db = 0.0f,
    eq->band[b].bw = 1.0f;
    eq->band[b].hz = floor(hz);
    if (hz < 1000.0f) {
      if (hz < 150.0f) {
        hz = hz + 15.0f;
      } else {
        if (hz < 600.0f) {
          hz = hz + 50.0f;
        } else {
          hz = hz + 200.0f;
        }
      }
    } else {
      hz = hz + 1500.0f;
    }
  }
  return eq;
}

void kr_eq_destroy(kr_eq *eq) {
  free(eq);
}

int kr_eq_info_get(kr_eq *eq, kr_eq_info *info) {
  int i;
  if ((eq == NULL) || (info == NULL)) return -1;
  for (i = 0; i < KR_EQ_MAX_BANDS; i++) {
    info->band[i].db = eq->band[i].db;
    info->band[i].bw = eq->band[i].bw;
    info->band[i].hz = eq->band[i].hz;
  }
  return 0;
}
