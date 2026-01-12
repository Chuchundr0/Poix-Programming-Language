import time
import sys
import os

def main():
    print("Start build POIX...\n")
    time.sleep(1)

    PathToFiles = [
        "../src/lexer/lexer.cpp",
        "../src/lexer/tokens.cpp",
        "../src/main.cpp"
    ]
    HaveAllFiles = False

    for file in PathToFiles:
        print(f"CHECK: {file} ", end="")

        if os.path.exists(file):
            print("\033[32m[FOUND]\033[0m")
            HaveAllFiles = True
        else:
            print("\033[31m[NOT FOUND]\033[0m")
            HaveAllFiles = False

        time.sleep(0.1)

    if(HaveAllFiles == False):
        print("\033[33m\nERROR:\033[0m missing some files. POIX did not compile.")
        return
    else:
        print("\nStart compile...")
        time.sleep(1)
        try:
            os.system("g++ ../src/main.cpp -o ../bin/POIX")
        except:
            print("\033[33mERROR:\033[0m error with POIX compile. POIX did not compile.")
            return

        print("\033[32mEnd of POIX compilation.\033[0m")


if __name__ == "__main__":
    main()
