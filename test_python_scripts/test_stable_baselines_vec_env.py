import gym
from stable_baselines.common.vec_env import SubprocVecEnv
from stable_baselines.common import set_global_seeds


def make_env(env_id, rank, seed=0):
   def _init():
      env = gym.make(env_id); env.seed(seed + rank); return env
   set_global_seeds(seed); return _init


if __name__ == '__main__':


   seed = 42
   n_copies = 3
   #env = SubprocVecEnv([make_env(" + env_id + " , rank=i, seed=seed) for i in range(n_copies)], start_method="fork")

   #env.reset()
   #env.close()