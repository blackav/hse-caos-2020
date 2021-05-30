/* -*- mode: c; c-basic-offset: 4 -*- */

#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>

enum { DEFAULT_MAIN_STACK = 8 * 1024 * 1024 };

typedef unsigned gthr_t;

struct GThrTask
{
    void *saved_ebx;
    void *saved_esi;
    void *saved_edi;
    void *saved_ebp;
    void *saved_esp;
    int is_main;
    int is_finished;
    int is_running;
    struct GThrTask *wait_thr;

    void *bot;
    size_t size;
    void (*func)(void *);
    void *user;

    struct GThrTask *prev;
    struct GThrTask *next;
};

struct GThrTask main_stack;

struct GThrTask *stacks_first;
struct GThrTask *stacks_last;

struct GThrTask *gthr_real_self()
{
    unsigned esp;
    asm volatile ("mov %%esp, %0" : "=g"(esp));
    for (struct GThrTask *p = stacks_first; p; p = p->next) {
        if (esp >= (unsigned) p->bot && esp <= (unsigned) p->bot + p->size)
            return p;
    }
    abort();
}

void gthr_switch(struct GThrTask *from, struct GThrTask *to);
void gthr_entry(struct GThrTask *from, struct GThrTask *to);
void gthr_schedule(struct GThrTask *self);

gthr_t gthr_self()
{
    return (gthr_t) gthr_real_self();
}

void gthr_trampoline()
{
    struct GThrTask *self = gthr_real_self();
    self->func(self->user);
    self->is_running = 0;
    self->is_finished = 1;
    for (struct GThrTask *p = stacks_first; p; p = p->next) {
        if (!p->is_running && p->wait_thr == self) {
            p->is_running = 1;
            p->wait_thr = 0;
        }
    }
    gthr_schedule(self);
}

void gthr_create(gthr_t *p_id, void (*func)(void *), void *user)
{
    struct GThrTask *self = gthr_real_self();

    size_t pagesize = sysconf(_SC_PAGESIZE);
    size_t stacksize = pagesize * 4;
    void *bot = mmap(NULL, stacksize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    struct GThrTask *top = (struct GThrTask *) (((unsigned char *) bot) + stacksize - sizeof(struct GThrTask));

    top->prev = stacks_last;
    if (stacks_last) {
        stacks_last->next = top;
    } else {
        stacks_first = top;
    }
    stacks_last = top;

    top->bot = bot;
    top->size = stacksize;
    top->func = func;
    top->user = user;
    unsigned esp = (unsigned) top - 32;
    esp &= -16;
    *p_id = (unsigned) top;
    top->saved_esp = (void*) esp;

    top->is_running = 1;
    gthr_entry(self, top);
}

void gthr_schedule(struct GThrTask *self)
{
    // find another ready stack
    struct GThrTask *p = self->next;
    while (p && !p->is_running) {
        p = p->next;
    }
    if (!p) {
        p = stacks_first;
        while (p != self && !p->is_running) {
            p = p->next;
        }
    }

    gthr_switch(self, p);
}

void gthr_yield()
{
    gthr_schedule(gthr_real_self());
}

void gthr_join(gthr_t id)
{
    struct GThrTask *self = gthr_real_self();
    struct GThrTask *other = (struct GThrTask *) id;

    while (!other->is_finished) {
        self->is_running = 0;
        self->wait_thr = other;
        gthr_schedule(self);
    }

    if (other->is_main) {
        abort();
    }

    // unlink from list
    if (other->prev) {
        other->prev->next = other->next;
    } else {
        stacks_first = other->next;
    }
    if (other->next) {
        other->next->prev = other->prev;
    } else {
        stacks_last = other->prev;
    }

    // destroy stack
    munmap(other->bot, other->size);
}

void testrun1(void *ptr)
{
    for (int i = 0; i < 10; ++i) {
        printf("%d\n", i);
        gthr_yield();
    }
}

void testrun2(void *ptr)
{
    for (int i = 0; i < 10; ++i) {
        printf("%d\n", i + 100);
        gthr_yield();
    }
}

int main()
{
    stacks_first = &main_stack;
    stacks_last = &main_stack;
    unsigned esp;
    asm volatile ("mov %%esp, %0" : "=g"(esp));
    size_t pagesize = sysconf(_SC_PAGESIZE);
    esp -= DEFAULT_MAIN_STACK;
    esp &= -pagesize;
    main_stack.is_main = 1;
    main_stack.is_running = 1;
    main_stack.bot = (void*) esp;
    main_stack.size = DEFAULT_MAIN_STACK + pagesize;

    gthr_t id1;
    gthr_t id2;
    gthr_create(&id1, testrun1, 0);
    gthr_create(&id2, testrun2, 0);
    gthr_join(id2);
    gthr_join(id1);
}
