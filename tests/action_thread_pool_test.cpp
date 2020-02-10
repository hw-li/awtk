#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tkc/platform.h"
#include "tkc/action_thread_pool.h"

#define NR 10

static uint32_t exec_times = 0;

static ret_t qaction_dummy_on_event(qaction_t* action, event_t* e) {
  if (e->type == EVT_DONE) {
    log_debug("done\n");
  }

  return RET_OK;
}

static ret_t qaction_dummy_exec(qaction_t* action) {
  exec_times++;
  log_debug("exec: exec_times=%u\n", exec_times);
  return RET_OK;
}

void test() {
  uint32_t i = 0;
  qaction_t action;
  action_thread_pool_t* pool = NULL;

  qaction_t* a = qaction_init(&action, qaction_dummy_exec, NULL, 0);
  qaction_set_on_event(a, qaction_dummy_on_event, NULL);

  pool = action_thread_pool_create(10, 2);

  action_thread_pool_exec(pool, a);
  action_thread_pool_exec(pool, a);
  action_thread_pool_exec(pool, a);

  sleep_ms(2000);

  action_thread_pool_destroy(pool);
  log_debug("exec_times=%u\n", exec_times);
}

#include "tkc/platform.h"

int main(int argc, char* argv[]) {
  platform_prepare();

  test();

  return 0;
}
