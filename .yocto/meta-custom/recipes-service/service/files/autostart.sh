SCRIPT="/usr/bin/script.sh"

USER="root"

case "$1" in
  start)
    echo "Starting TST Weather UI Service"
    DISPLAY=":0" /bin/bash -c "$SCRIPT" $USER
    ;;
  stop)
    echo "Stopping TST Weather UI Service"
    ;;
  restart)
    echo "Restarting TST Weather UI Service"
    $0 stop
    sleep 2
    $0 start
    ;;
  *)
    echo "Usage: $0 {start|stop|restart}"
    exit 1
    ;;
esac

exit 0
