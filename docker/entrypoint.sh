#! /bin/bash

set -e
USERNAME="ros2"
DEFAULT_USER_ID=1000
if [ -v USER_ID ] && [ "$USER_ID" != "$DEFAULT_USER_ID" ]; then
  echo "Changing witpilot user ID to match your host's user ID ($USER_ID)."
  echo "This operation can take a while..."
  usermod --uid $USER_ID $USERNAME
  find /home/rostest -user $DEFAULT_USER_ID -exec chown -h $USER_ID {} \;
fi
export HOME="/home/$USERNAME"
# `cd $HOME/roslearning
if [ -z "$1" ]; then
  set - "bin/bash" -l
fi

# Run the provided command using user 'witpilot'
# chroot --userspec=$USERNAME:$USERNAME --skip-chdir / "$@"
source /opt/ros/$ROS_DISTRO/setup.bash
