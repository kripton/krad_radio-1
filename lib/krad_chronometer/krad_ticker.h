typedef struct kr_ticker kr_ticker;

struct timespec kr_ticker_add_ms(struct timespec ts, uint64_t ms);
struct timespec kr_ticker_add_ns(struct timespec ts, uint64_t ns);
int kr_ticker_destroy(kr_ticker *ticker);
kr_ticker *kr_ticker_create(int num, int den);
//krad_ticker_t *krad_ticker_throttle_create();
int kr_ticker_start_at(kr_ticker *ticker, struct timespec start_time);
int kr_ticker_start(kr_ticker *ticker);
int kr_ticker_wait(kr_ticker *ticker);
//void krad_ticker_throttle(kr_ticker *ticker, uint64_t ms);
