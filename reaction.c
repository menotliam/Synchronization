#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
	// FILL ME IN
       struct lock lock;
       struct condition cv_h;
       struct condition cv_o;
       struct condition cv_barrier;
       int h_count;
       int o_count;
       int group;
       int h_board;
       int o_board;
};

void
reaction_init(struct reaction *reaction)
{
	// FILL ME IN
       lock_init(&reaction->lock);
       cond_init(&reaction->cv_h);
       cond_init(&reaction->cv_o);
       cond_init(&reaction->cv_barrier);
       reaction->h_count = 0;
       reaction->o_count = 0;
       reaction->group = 0;
       reaction->h_board = 0;
       reaction->o_board = 0;
}

void
reaction_h(struct reaction *reaction)
{
	// FILL ME IN
       lock_acquire(&reaction->lock);
       int is_leader = 0;
       reaction->h_count++;
       if(reaction->h_count >= 2 && reaction->o_count >= 1 && reaction->group == 0){
              reaction->h_count -= 2;
              reaction->o_count -= 1;
              reaction->h_board = 2;
              reaction->o_board = 1;
              reaction->group = 3;
              is_leader = 1;
              cond_signal(&reaction->cv_h, &reaction->lock);
              cond_signal(&reaction->cv_h, &reaction->lock);
              cond_signal(&reaction->cv_o, &reaction->lock);
       } else{
             while(reaction->h_board == 0) cond_wait(&reaction->cv_h, &reaction->lock);
         }
       reaction->h_board--;
       barrier_wait(reaction);
       lock_release(&reaction->lock);
       if(is_leader) make_water();
}

void
reaction_o(struct reaction *reaction)
{
	// FILL ME IN
       lock_acquire(&reaction->lock);
       int is_leader = 0;
       reaction->o_count++;
       if(reaction->h_count >= 2 && reaction->o_count >= 1 && reaction->group == 0){
              reaction->h_count -= 2;
              reaction->o_count -= 1;
              reaction->group = 3;
              reaction->h_board = 2;
              reaction->o_board = 1;
              is_leader = 1;
              cond_signal(&reaction->cv_h, &reaction->lock);
              cond_signal(&reaction->cv_h, &reaction->lock);
              cond_signal(&reaction->cv_o, &reaction->lock);
       } else{
             while(reaction->o_board == 0) cond_wait(&reaction->cv_o, &reaction->lock);
         }
       reaction->o_board--;
       barrier_wait(reaction);
       lock_release(&reaction->lock);
       if(is_leader) make_water();
}

void
barrier_wait(struct reaction *reaction)
{
       reaction->group--;
       if(reaction->group > 0){
          while(reaction->group > 0) cond_wait(&reaction->cv_barrier, &reaction->lock);
       } else{
             cond_broadcast(&reaction->cv_barrier, &reaction->lock);
         }
}
