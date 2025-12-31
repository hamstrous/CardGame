# Python 2 compatible
# Writes all filenames from a directory to a text file

import os

TARGET_DIR = "ascension"        # folder containing the files
OUTPUT_FILE = "filenames.txt"
id = 1
with open(OUTPUT_FILE, "w") as out:
    for name in sorted(os.listdir(TARGET_DIR)):
        path = os.path.join(TARGET_DIR, name)
        if os.path.isfile(path):
            out.write(str(id) + "	1630	540 cards/ascension/" + name + "	cards/ascension_card_back.png	100	170	0	1" + "\n")
            id += 1

print("Wrote filenames to {}".format(OUTPUT_FILE))
