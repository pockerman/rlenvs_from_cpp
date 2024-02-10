from loguru import logger
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

from api_info import api_info_router
from toy_text.frozenlake_env_api import frozenlake_router
from toy_text.taxi_env_api import taxi_router
from toy_text.cliffwalking_env_api import cliff_walking_router
from toy_text.black_jack_env_api import black_jack_router
from classic_control.cart_pole_env_api import cart_pole_router
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


@app.on_event("startup")
def on_startup():
    """Loads the trained model

    Returns
    -------

    """

    # make checks

    logger.info("Starting API...")
