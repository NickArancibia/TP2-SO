// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/tests.h"
#include "./include/lib.h"
typedef struct MM_rq
{
    void *address;
    uint32_t size;
} mm_rq;

typedef struct P_rq
{
    int32_t pid;
    ProcessState state;
} p_rq;

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

int checkMemExit();

uint64_t test_mm(uint64_t argc, char *argv[])
{
    uint8_t rq;
    mm_rq mm_rqs[MAX_BLOCKS];

    uint32_t total;
    uint64_t max_memory;
    if ((max_memory = satoi(argv[0])) <= 0)
        return -1;

    while (1)
    {

        rq = 0;
        total = 0;

        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < max_memory)
        {
            mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
            mm_rqs[rq].address = malloc(mm_rqs[rq].size);
            if (mm_rqs[rq].address)
            {
                total += mm_rqs[rq].size;
                rq++;
            }
        }

        // Set
        uint32_t i;
        for (i = 0; i < rq; i++)
        {
            if (mm_rqs[i].address)
                memset(mm_rqs[i].address, i, mm_rqs[i].size);
        }

        // Check
        for (i = 0; i < rq; i++)
        {
            if (mm_rqs[i].address)
                if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
                {
                    printColor("test_mm ERROR\n", RED);
                    return -1;
                }
        }

        // Free
        for (i = 0; i < rq; i++)
        {
            if (mm_rqs[i].address)
            {
                free(mm_rqs[i].address);
                mm_rqs[i].address = 0;
            }
        }
    }
}

int64_t test_processes(uint64_t argc, char *argv[])
{
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;
    uint64_t max_processes;
    char *argvAux[] = {0};
    creationParameters params;
    params.name = "endless_loop";
    params.argc = 0;
    params.argv = argvAux;
    params.priority = 1;
    params.entryPoint = (entryPoint)endless_loop;
    params.foreground = 0;

    if (argc != 1)
        return -1;

    if ((max_processes = satoi(argv[0])) <= 0)
        return -1;

    p_rq p_rqs[max_processes];
    while (1)
    {
        // Create max_processes processes
        for (rq = 0; rq < max_processes; rq++)
        {
            p_rqs[rq].pid = createProcess(&params);

            if (p_rqs[rq].pid == -1)
            {
                printf("test_processes: ERROR creating process\n");
                return -1;
            }
            else
            {
                p_rqs[rq].state = RUNNING;
                alive++;
            }
        }

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0)
        {

            for (rq = 0; rq < max_processes; rq++)
            {
                action = GetUniform(100) % 2;

                switch (action)
                {
                case 0:
                    if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED)
                    {
                        if (sysKill(p_rqs[rq].pid) == -1)
                        {
                            printf("test_processes: ERROR killing process\n");
                            return -1;
                        }
                        p_rqs[rq].state = DEAD;
                        alive--;
                    }
                    break;
                case 1:
                    if (p_rqs[rq].state == RUNNING)
                    {
                        if (sysSuspendProcess(p_rqs[rq].pid) == -1)
                        {
                            printf("test_processes: ERROR blocking process\n");
                            return -1;
                        }
                        p_rqs[rq].state = BLOCKED;
                    }
                    break;
                }
            }

            // Randomly unblocks processes
            for (rq = 0; rq < max_processes; rq++)
                if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2)
                {
                    if (sysResumeProcess(p_rqs[rq].pid) == -1)
                    {
                        printf("test_processes: ERROR unblocking process\n");
                        return -1;
                    }
                    p_rqs[rq].state = RUNNING;
                }
        }
    }
}

void test_prio()
{
    int64_t pids[TOTAL_PROCESSES];
    char *argv[] = {0};
    uint64_t i;
    creationParameters params;
    params.name = "endless_loop_print";
    params.argc = 0;
    params.argv = argv;
    params.priority = 1;
    params.entryPoint = (entryPoint)endless_loop_print;
    params.foreground = 0;

    for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = createProcess(&params);

    bussy_wait(WAIT);
    printf("\nCHANGING PRIORITIES...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sysNice(pids[i], prio[i]);

    bussy_wait(WAIT);
    printf("\nBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sysSuspendProcess(pids[i]);

    printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sysNice(pids[i], MEDIUM);

    printf("UNBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sysResumeProcess(pids[i]);

    bussy_wait(WAIT);
    printf("\nKILLING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sysKill(pids[i]);
}

int checkProcsExit()
{
    if (getchar() == 'q')
    {
        return 1;
    }
    return 0;
}

void slowInc(int64_t *p, int64_t inc)
{
    uint64_t aux = *p;
    sysYield(); // This makes the race condition highly probable
    aux += inc;
    *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[])
{

    uint64_t n;
    int8_t inc;
    int8_t use_sem;
    int64_t *global;
    if (argc != 4)
    {
        printf("termine %d\n", (int32_t)argc);
        return -1;
    }
    if ((n = satoi(argv[0])) <= 0)
        return -1;
    if ((inc = satoi(argv[1])) == 0)
        return -1;
    if ((use_sem = satoi(argv[2])) < 0)
        return -1;

    global = (int64_t *)satoi(argv[3]);
    if (use_sem)
        if (sysSemOpen(SEM_ID, 1))
        {
            printf("test_sync: ERROR opening semaphore\n");
            return -1;
        }

    uint64_t i;
    for (i = 0; i < n; i++)
    {
        if (use_sem)
            sysSemWait(SEM_ID);
        slowInc(global, inc);
        if (use_sem)
            sysSemPost(SEM_ID);
    }
    if (use_sem)
        sysSemClose(SEM_ID);

    return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[])
{

    int64_t global; // shared memory
    uint64_t pids[2 * TOTAL_PAIR_PROCESSES];
    global = 0;
    if (argc != 2)
        return -1;

    int64_t *p = &global;

    char buff[20] = {0};
    intToString((int64_t)p, buff, 1);
    char *argvDec[] = {argv[0], "-1", argv[1], buff, NULL};
    char *argvInc[] = {argv[0], "1", argv[1], buff, NULL};

    creationParameters paramsDec;
    paramsDec.name = "my_process_dec";
    paramsDec.argc = 4;
    paramsDec.argv = argvDec;
    paramsDec.priority = 1;
    paramsDec.entryPoint = (entryPoint)my_process_inc;
    paramsDec.foreground = 1;

    creationParameters paramsInc;
    paramsInc.name = "my_process_inc";
    paramsInc.argc = 4;
    paramsInc.argv = argvInc;
    paramsInc.priority = 1;
    paramsInc.entryPoint = (entryPoint)my_process_inc;
    paramsInc.foreground = 1;

    uint64_t i, j;
    for (i = 0, j = 0; j < TOTAL_PAIR_PROCESSES; j++, i += 2)
    {
        pids[i] = sysCreateProcess(&paramsDec);
        pids[i + 1] = sysCreateProcess(&paramsInc);
    }

    for (i = 0, j = 0; j < TOTAL_PAIR_PROCESSES; j++, i += 2)
    {
        sysWait(pids[i], NULL);
        sysWait(pids[i + 1], NULL);
    }

    printf("Final value: %d\n", (int32_t)global);
    return 0;
}
