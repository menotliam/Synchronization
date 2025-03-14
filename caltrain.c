#include "pintos_thread.h"

struct station {
       struct lock lock;
       struct condition cond_train;
       struct condition cond_passenger;
       int waiting;
       int available;
       int boarding;
};

void
station_init(struct station *station)
{
       lock_init(&station->lock);
       cond_init(&station->cond_train);
       cond_init(&station->cond_passenger);
       station->waiting = 0;
       station->available = 0;
       station->boarding = 0;
}

void
station_load_train(struct station *station, int count)
{
       lock_acquire(&station->lock);
       station->available = count;
       cond_broadcast(&station->cond_train, &station->lock);
       while(station->boarding > 0 || (station->available > 0 && station->waiting > 0)){
            cond_wait(&station->cond_passenger, &station->lock);
       }
       station->available = 0;
       lock_release(&station->lock);
}

void
station_wait_for_train(struct station *station)
{
       lock_acquire(&station->lock);
       station->waiting++;
       while(station->available == 0){
            cond_wait(&station->cond_train, &station->lock);
       }
       station->available--;
       station->waiting--;
       station->boarding++;
       lock_release(&station->lock);
}

void
station_on_board(struct station *station)
{
       lock_acquire(&station->lock);
       station->boarding--;
       if(station->boarding == 0 && (station->available == 0 || station->waiting == 0)){
             cond_signal(&station->cond_passenger, &station->lock);
       }
       lock_release(&station->lock);
}
