#!/usr/bin/env bash

#  echo "begin"
NAME=rostest
# IMAGE=test=
 IMAGE=osrf/ros:galactic-desktop
# IMAGE=deb34ee00221 
XSOCK=/tmp/.X11-unix
XAUTH=$HOME/.Xauthority
XAUTH_DOCKER=$HOME/.Xauthority
SHARED_HOST_DIR=$PWD
SHARED_DOCKER_DIR=/home/ros2/rostest
PUBLISH=" --net=host "
CMD=""
# ACTION="start"
# echo $ACTION
VOLUMES="--volume=$XSOCK:$XSOCK:rw
--volume=$XAUTH:$XAUTH:rw
--volume=$SHARED_HOST_DIR:$SHARED_DOCKER_DIR:rw"

function usage(){
  echo "Usage: $0 [OPTIONS]"
  echo "    -h,--help                                   Display the usage and exit."
  echo "    -a,--action <action> Action [start          start mirror
                                         build        build code
					 ci           wait for update
					 devinto      get into a container
					 demo         demo display
					 run          wait for update
					 sim          wait for update
					 web          wait for update
					 ]"
}

while true; do
  case $1 in
  -a | --action)
    ACTION=$2
    shift 2
    break
    ;;
  -h | --help)
    usage
    exit 0
    ;;
  null)
    break
    ;;
  esac
done

if [ "$ACTION" == "test" ]; then
  echo "test begin"
  if [ ! -e "/.dockerenv" ]; then
    echo "please run in dockerenv"
    exit
  fi
  # source /home/ubuntu/.bashrc
  source ./install/setup.bash
  
 # ros2 launch scenario_test_runner scenario_test_runner.launch.py scenario:='./test/yaml/'$1 launch_autoware:=false launch_rviz:=true
  ros2 launch scenario_test_runner scenario_test_runner.launch.py scenario:='./src/scenario_simulator/test_runner/scenario_test_runner/test/scenario/'$1 launch_autoware:=false launch_rviz:=true
  exit
fi

if [ "$ACTION" == "devinto" ]; then
  docker exec -it -u root -w $SHARED_DOCKER_DIR $NAME bash --login
  exit
fi

if [ "$ACTION" == "demo" ]; then
  source ./install/setup.bash
  ros2 launch cpp_mock_scenarios mock_test.launch.py scenario:=traffic_simulation_demo launch_rviz:=true timeout:=60.0
  exit
fi

if [ "$ACTION" == "start" ]; then
  # echo "start"
  CMD="bash"
  fi
docker run \
   -it --rm --name $NAME $VOLUMES \
   --env="XAUTHORITY=${XAUTH}" \
   --env="DISPLAY=${DISPLAY}" \
   --shm-size 2G \
   --env="QT_X11_NO_MITSHM=1" \
   --privileged $PUBLISH \
   --workdir $SHARED_DOCKER_DIR \
   $IMAGE \
   $CMD

