
Keep directory structure.
Bash:
    rm -rf Distances.so
    g++ -O3 -Wall -Werror -shared -std=c++11 -fPIC Distances.cpp -o Distances.so 

Use in Py:
    from dists.dists import getGEO, getMBD 

In dist/dists.py change getGEO and getMBD if needed, only those should be visible and usable from the outside. 