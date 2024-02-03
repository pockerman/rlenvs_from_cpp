import os

from loguru import logger
from dotenv import load_dotenv
from fastapi import FastAPI
from fastapi import HTTPException
from fastapi import status
from fastapi.middleware.cors import CORSMiddleware

from api_info import api_info_router
from frozenlake_env_api import frozenlake_router
from taxi_env_api import taxi_router
from cliffwalking_env_api import cliffwalking_router
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
app.include_router(cliffwalking_router, prefix=BASE_URL)


@app.on_event("startup")
def on_startup():
    """Loads the trained model

    Returns
    -------

    """

    # make checks

    logger.info("Starting API...")
