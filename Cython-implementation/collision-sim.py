import collision_funcs
from sys import argv

if __name__ == "__main__":

    try:
        n = int(argv[1])
        v_init = float(argv[2])
    except Exception as e:
        print(e)

    print(collision_funcs.main(n, v_init))
