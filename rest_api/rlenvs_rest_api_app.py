from loguru import logger
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

from api_info import api_info_router
from gymnasium_envs.toy_text.frozenlake_env_api import frozenlake_router
from gymnasium_envs.toy_text.taxi_env_api import taxi_router
from gymnasium_envs.toy_text.cliffwalking_env_api import cliff_walking_router
from gymnasium_envs.toy_text.black_jack_env_api import black_jack_router
from gymnasium_envs.classic_control.cart_pole_env_api import cart_pole_router
from gymnasium_envs.classic_control.mountain_car_env_api import mountain_car_router
from gymnasium_envs.classic_control.pendulum_env_api import pendulum_router
from gymnasium_envs.classic_control.acrobot_env_api import acrobot_router
from gdrl.gym_walk_env.gym_walk_env_api import gym_walk_env_router
from gym_pybullet_drones_env.quadrotor_sim_env_api import quadrotor_sim_router

from api_config import get_api_config


BASE_URL = "/api"

CONFIG = get_api_config()

app = FastAPI(title=CONFIG.API_TITLE,
              debug=CONFIG.DEBUG)

# CORS handling: https://fastapi.tiangolo.com/tutorial/cors/
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(api_info_router, prefix=BASE_URL)
app.include_router(taxi_router, prefix=BASE_URL)
app.include_router(frozenlake_router, prefix=BASE_URL)
app.include_router(cliff_walking_router, prefix=BASE_URL)
app.include_router(black_jack_router, prefix=BASE_URL)
app.include_router(cart_pole_router, prefix=BASE_URL)
app.include_router(mountain_car_router, prefix=BASE_URL)
app.include_router(gym_walk_env_router, prefix=BASE_URL)
app.include_router(pendulum_router, prefix=BASE_URL)
app.include_router(acrobot_router, prefix=BASE_URL)
app.include_router(quadrotor_sim_router, prefix=BASE_URL)


@app.on_event("startup")
def on_startup():
    """Loads the trained model

    Returns
    -------

    """

    # make checks

    logger.info("Starting API...")
