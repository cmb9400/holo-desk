until python3 ~/Development/holo-desk/tablepi/input.py ;
do
    echo "input.py crashed with exit code $?.  Respawning.." >&2
    sleep 1
done
