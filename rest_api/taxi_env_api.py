import gymnasium as gym
from fastapi import APIRouter, Depends, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
taxi_router = APIRouter(prefix="/gym-taxi-env", tags=["gym-taxi-env"])

# the environment to create
env = None


@taxi_router.get("/is-alive")
async def get_is_alive():
    global env

    if env is None:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": False})
    else:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": True})


@taxi_router.post("/make")
async def make(version: str = Body(...)):
    global  env
    if env is not None:
        env.close()

    try:
        env = gym.make(f"Taxi-{version}")
    except Exception as e:
        raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                            detail=str(e))

    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True})