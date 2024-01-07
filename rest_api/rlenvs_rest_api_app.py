import os

from loguru import logger
from dotenv import load_dotenv
from fastapi import FastAPI
from fastapi import HTTPException
from fastapi import status
from fastapi.middleware.cors import CORSMiddleware
from taxi_env_api import taxi_router
from frozenlake_env_api import frozenlake_router


BASE_URL = "/api"

app = FastAPI(title="rlenvs-rest-api",
              debug="Debug")

# CORS handling: https://fastapi.tiangolo.com/tutorial/cors/
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(frozenlake_router, prefix=BASE_URL)
app.include_router(taxi_router, prefix=BASE_URL)


@app.on_event("startup")
def on_startup():
    """Loads the trained model

    Returns
    -------

    """

    # make checks

    logger.info("Starting API...")
