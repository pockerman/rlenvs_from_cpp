export CPLUS_INCLUDE_PATH="$CPLUS_INCLUDE_PATH:/usr/include/python3.8/"
cd ./build/tests/

echo "Running EnvBase tests"
cd test_env_base/ && ./test_env_base
cd ..
echo "Running BlackJack tests"
cd test_black_jack/ && ./test_black_jack
cd ..
echo "Running CartPole tests"
cd test_cart_pole/ && ./test_cart_pole
cd ..
echo "Running CliffWorld tests"
cd test_cliff_world/ && ./test_cliff_world
cd ..
echo "Running ForzenLake tests"
cd test_frozen_lake/ && ./test_frozen_lake
cd ..
echo "Running MountainCar tests"
cd test_mountain_car/ && ./test_mountain_car
cd ..
echo "Running Taxi tests"
cd test_taxi/ && ./test_taxi
cd ..
echo "Running TiledCartPole environment tests"
cd test_tiled_cart_pole/ && ./test_tiled_cart_pole
cd ..
