#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

struct list_node_s
{
    int data;
    struct list_node_s* next;
};

const int MAX_THREADS = 1024;

long thread_count;
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;
struct list_node_s* head = NULL;

int n;
int m;
float mMember;
float mInsert;
float mDelete;
int count_member=0;
int count_insert=0;
int count_delete=0;

int member( int value, struct  list_node_s* head_p );
int insert(int value, struct list_node_s** head_pp);
int m_delete(int value, struct list_node_s** head_pp);
int printList( struct  list_node_s* head_p );
void* thread_oparation_one_to_all(void* rank);
void* thread_oparation_read_write(void* rank);
void* thread_oparation_read_write_custom(void* rank);

typedef struct {
    int readers;
    int writer;
    pthread_cond_t readers_proceed;
    pthread_cond_t writer_proceed;
    int pending_writers;
    pthread_mutex_t read_write_lock;
} mylib_rwlock_t;

mylib_rwlock_t custom_rwlock;

void mylib_rwlock_init (mylib_rwlock_t *l) {
    l -> readers = l -> writer = l -> pending_writers = 0;
    pthread_mutex_init(&(l -> read_write_lock), NULL);
    pthread_cond_init(&(l -> readers_proceed), NULL);
    pthread_cond_init(&(l -> writer_proceed), NULL);
}

void mylib_rwlock_rlock(mylib_rwlock_t *l) {
    /* if there is a write lock or pending writers, perform condition
 wait.. else increment count of readers and grant read lock */

    pthread_mutex_lock(&(l -> read_write_lock));
    while ((l -> pending_writers > 0) || (l -> writer > 0))
        pthread_cond_wait(&(l -> readers_proceed),
                          &(l -> read_write_lock));
    l -> readers ++;
    pthread_mutex_unlock(&(l -> read_write_lock));
}


void mylib_rwlock_wlock(mylib_rwlock_t *l) {
    /* if there are readers or writers, increment pending writers
 count and wait. On being woken, decrement pending writers
 count and increment writer count */

    pthread_mutex_lock(&(l -> read_write_lock));
    while ((l -> writer > 0) || (l -> readers > 0)) {
        l -> pending_writers ++;
        pthread_cond_wait(&(l -> writer_proceed),
                          &(l -> read_write_lock));
    }
    l -> pending_writers --;
    l -> writer ++;
    pthread_mutex_unlock(&(l -> read_write_lock));
}


void mylib_rwlock_unlock(mylib_rwlock_t *l) {
    /* if there is a write lock then unlock, else if there are
 read locks, decrement count of read locks. If the count
 is 0 and there is a pending writer, let it through, else
 if there are pending readers, let them all go through */

    pthread_mutex_lock(&(l -> read_write_lock));
    if (l -> writer > 0)
        l -> writer = 0;
    else if (l -> readers > 0)
        l -> readers --;     pthread_mutex_unlock(&(l -> read_write_lock));
    if ((l -> readers == 0) && (l -> pending_writers > 0))
        pthread_cond_signal(&(l -> writer_proceed));
    else if (l -> readers > 0)
        pthread_cond_broadcast(&(l -> readers_proceed));
}


int main(int argc, char* argv[])
{
    //one_to_all(argc, argv);
    //one_to_node(argc, argv);
    one_to_node_custom(argc, argv);
    return 0;
}



void one_to_node(int argc, char* argv[]){
    int i=0;
    long       thread;  /* Use long in case of a 64-bit system */
    pthread_t* thread_handles;
        double start, finish, elapsed;

    Get_args(argc, argv);

    for(;i<n;i++)
    {
        int r = rand()%65536;
        if(!insert(r,&head))
        {
            i--;
        }
    }

    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));

    start = clock();
    pthread_rwlock_init(&rwlock, NULL);

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL,thread_oparation_read_write , (void*)thread);
    }

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    pthread_rwlock_destroy(&rwlock);
    finish = clock();
    elapsed = (finish - start)/CLOCKS_PER_SEC;

    printf("Elapsed time = %e seconds\n", elapsed);
    return 0;
}



void one_to_node_custom(int argc, char* argv[]){
    int i=0;
    long       thread;  /* Use long in case of a 64-bit system */
    pthread_t* thread_handles;
        double start, finish, elapsed;

    Get_args(argc, argv);

    for(;i<n;i++)
    {
        int r = rand()%65536;
        if(!insert(r,&head))
        {
            i--;
        }
    }

    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));

    start = clock();
    mylib_rwlock_init(&custom_rwlock);

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL,thread_oparation_read_write_custom , (void*)thread);
    }

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }
    finish = clock();
    elapsed = (finish - start)/CLOCKS_PER_SEC;

    printf("Elapsed time = %e seconds\n", elapsed);
    return 0;
}



void one_to_all(int argc, char* argv[]){
    int i=0;
    long       thread;
    pthread_t* thread_handles;
    double start, finish, elapsed;
    Get_args(argc, argv);
    for(;i<n;i++)
    {
        int r = rand()%65536;
        if(!insert(r,&head))
        {
            i--;
        }
    }
    thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));

    start = clock();
    pthread_mutex_init(&mutex, NULL);

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL,thread_oparation_one_to_all , (void*)thread);
    }

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    pthread_mutex_destroy(&mutex);
    finish = clock();
    elapsed = (finish - start)/CLOCKS_PER_SEC;

    printf("Elapsed time = %e seconds\n", elapsed);
}

int member( int value, struct  list_node_s* head_p )
{
    struct list_node_s* curr_p = head_p;

    while( curr_p != NULL && curr_p->data < value )
    {
        curr_p = curr_p->next;
    }

    if(curr_p == NULL || curr_p->data > value)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


int insert(int value, struct list_node_s** head_pp)
{
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p = NULL;

    while(curr_p !=NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if(curr_p == NULL || curr_p->data > value)
    {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;

        if(pred_p == NULL)
        {
            *head_pp = temp_p;
        }
        else
        {
            pred_p->next = temp_p;
        }
        return 1;

    }
    else
    {
        return 0;
    }
}


int m_delete(int value, struct list_node_s** head_pp)
{
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    while(curr_p != NULL && curr_p->data < value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if(curr_p != NULL && curr_p -> data < value)
    {
        if(pred_p == NULL){
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else
        {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;

    }
    else
    {
        return 0;
    }

}

int printList( struct  list_node_s* head_p )
{
    struct list_node_s* curr_p = head_p;

    while(curr_p != NULL)
    {
        printf("%d ",curr_p->data);
        curr_p = curr_p->next;
    }
    printf("\n");
}

void Get_args(int argc, char* argv[]) {

    thread_count = strtol(argv[1], NULL, 10);

    n = (int) strtol(argv[2], (char **)NULL, 10);
    m = (int) strtol(argv[3], (char **)NULL, 10);

    mMember = (float) atof(argv[4]);
    mInsert = (float) atof(argv[5]);
    mDelete = (float) atof(argv[6]);
}

void* thread_oparation_one_to_all(void* rank)
{
    long my_rank = (long) rank;
    double factor, my_sum = 0.0;
    long long i;
    long long my_m = m/thread_count;
    for( i=0; i < my_m; i++ )
    {
        float prob = (rand()%10000/10000.0);

        int r = rand()%65536;
        if(prob<mMember)
        {
            pthread_mutex_lock(&mutex);
            member(r,head);
            count_member++;
            pthread_mutex_unlock(&mutex);
        }
        else if(prob < mMember + mInsert )
        {
            pthread_mutex_lock(&mutex);
            insert(r,&head);
            count_insert++;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            pthread_mutex_lock(&mutex);
            m_delete(r,&head);
            count_delete++;
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}  /* Thread_sum */

void* thread_oparation_read_write(void* rank)
{
    long my_rank = (long) rank;
    double factor, my_sum = 0.0;
    long long i;
    long long my_m = m/thread_count;
    for( i=0; i< my_m; i++ )
    {

        float prob = (rand()%10000/10000.0);
        //printf("%f\n",prob);


        int r = rand()%65536;
        if(prob < mMember)
        {
            pthread_rwlock_rdlock(&rwlock);
            member(r,head);
            count_member++;
            pthread_rwlock_unlock(&rwlock);
        }
        else if(prob < mMember + mInsert )
        {
            pthread_rwlock_wrlock(&rwlock);
            insert(r,&head);
            count_insert++;
            pthread_rwlock_unlock(&rwlock);
        }
        else
        {
            pthread_rwlock_wrlock(&rwlock);
            m_delete(r,&head);
            count_delete++;
            pthread_rwlock_unlock(&rwlock);
        }

    }

    return NULL;
}  /* Thread_sum */


void* thread_oparation_read_write_custom(void* rank)
{
    long my_rank = (long) rank;
    double factor, my_sum = 0.0;
    long long i;
    long long my_m = m/thread_count;
    for( i=0; i< my_m; i++ )
    {

        float prob = (rand()%10000/10000.0);
        //printf("%f\n",prob);


        int r = rand()%65536;
        if(prob < mMember)
        {
            //pthread_rwlock_rdlock(&rwlock);
            mylib_rwlock_rlock(&custom_rwlock);
            member(r,head);
            count_member++;
            //pthread_rwlock_unlock(&rwlock);
            mylib_rwlock_unlock(&custom_rwlock);

        }
        else if(prob < mMember + mInsert )
        {
            //pthread_rwlock_wrlock(&rwlock);
            mylib_rwlock_wlock(&custom_rwlock);
            insert(r,&head);
            count_insert++;
            //pthread_rwlock_unlock(&rwlock);
            mylib_rwlock_unlock(&custom_rwlock);
        }
        else
        {
            //pthread_rwlock_wrlock(&rwlock);
            mylib_rwlock_wlock(&custom_rwlock);
            m_delete(r,&head);
            count_delete++;
            //pthread_rwlock_unlock(&rwlock);
            mylib_rwlock_unlock(&custom_rwlock);
        }

    }

    return NULL;
}
