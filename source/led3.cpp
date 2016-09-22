#include "fun_bag.h"
#include "uvisor-lib/uvisor-lib.h"
#include "mbed.h"
#include "rtos.h"
#include "main-hw.h"

struct box_context {
    uint32_t heartbeat;
    int toggle;
};

static const UvisorBoxAclItem acl[] = {
};

static void led3_main(const void *);

UVISOR_BOX_NAMESPACE(NULL);
UVISOR_BOX_HEAPSIZE(3000);
UVISOR_BOX_MAIN(led3_main, osPriorityNormal, UVISOR_BOX_STACK_SIZE);
UVISOR_BOX_CONFIG(box_led3, acl, UVISOR_BOX_STACK_SIZE, box_context);

static void run_3(void)
{
    while (1) {
        const int toggle = uvisor_ctx->toggle;
        uint16_t size = toggle ? 150 : 99;
        uint16_t seed = (size << 8) | (uvisor_ctx->heartbeat & 0xFF);

        uvisor_ctx->toggle = !uvisor_ctx->toggle;
        alloc_fill_wait_verify_free(size, seed, 0);
    }
}

static void my_box_switch_irq0(void)
{
    uint32_t ii = 0;
    while(ii++ < 100000) ;
    (void) ii;
    // while(1) ;
}

static void my_box_switch_irq1(void)
{
    uint32_t ii = 0;
    while(ii++ < 100000) ;
    (void) ii;
    // while(1) ;
}

static void led3_main(const void *)
{
    us_ticker_read();
    osStatus status;
    DigitalOut led3(LED1);
    DigitalOut pin(PB11);
    led3 = LED_OFF;

    InterruptIn button0(SW0);
    button0.mode(PullUp);
    button0.fall(my_box_switch_irq0);
    // NVIC_SetVector(GPIO_ODD_IRQn, (uint32_t) my_box_switch_irq0);

    InterruptIn button1(SW1);
    button1.mode(PullUp);
    button1.fall(my_box_switch_irq1);
    // NVIC_SetVector(GPIO_EVEN_IRQn, (uint32_t) my_box_switch_irq1);

    // Thread * thread1 = new Thread();
    // status = thread1->start(run_3);
    // if (status != osOK) {
    //     printf("Could not start box_led3 thread1.\r\n");
    //     uvisor_error(USER_NOT_ALLOWED);
    // }

    // Thread * thread2 = new Thread();
    // status = thread2->start(run_3);
    // if (status != osOK) {
    //     printf("Could not start box_led3 thread2.\r\n");
    //     uvisor_error(USER_NOT_ALLOWED);
    // }

    // /* Create page-backed allocator. */
    // const uint32_t kB = 1024;
    // SecureAllocator alloc = secure_allocator_create_with_pages(4*kB, 1*kB);
    // if (alloc == NULL) uvisor_error(USER_NOT_ALLOWED);
    // /* Prepare the thread definition structure. */
    // osThreadDef_t thread_def;
    // thread_def.stacksize = DEFAULT_STACK_SIZE;
    // /* Allocate the stack inside the page allocator! */
    // thread_def.stack_pointer = (uint32_t *) secure_malloc(alloc, DEFAULT_STACK_SIZE);
    // thread_def.tpriority = osPriorityNormal;
    // thread_def.pthread = (void (*)(const void *)) &run_3;
    // /* Create a thread with the page allocator as heap. */
    // osThreadContextCreate(&thread_def, NULL, alloc);


    while (1) {
        // static const size_t size = 20;
        // uint16_t seed = (size << 8) | (uvisor_ctx->heartbeat & 0xFF);

        led3 = !led3;

        // register uint32_t asm("sp") sp;
        // bool is_stacked = !(sp & 0x04);
        // if (is_stacked) asm volatile ("push r1");
        pin = !pin;
        // if (is_stacked) asm volatile ("pop r1");
        Thread::wait(1);
        // ++uvisor_ctx->heartbeat;
        // alloc_fill_wait_verify_free(size, seed, 0);
    }
}
