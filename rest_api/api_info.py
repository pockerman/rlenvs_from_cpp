from fastapi import APIRouter, Depends, Body, status
from fastapi.responses import JSONResponse

api_info_router = APIRouter(prefix="/api-info", tags=["api-info"])


@api_info_router.get("/gymnasium")
async def get_has_gymnasium() -> JSONResponse:
    try:
        import gymnasium as gym
        return JSONResponse(status_code=status.HTTP_200_OK, content={"message": "Gymnasium found on server"})
    except ModuleNotFoundError as e:
        return JSONResponse(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                            content={"message": "Gymnasium is not installed on the server"})


@api_info_router.get("/gymnasium/envs")
async def get_gymnasium_envs() -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_200_OK,
                        content={"envs": ["FrozenLake-4x4", "FrozenLake-8x8", "Taxi-v3",
                                          "CliffWalking-v0", "Pendulum-v0", "Pendulum-v1"]})
