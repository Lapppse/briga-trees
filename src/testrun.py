import shutil
from os import getenv
from pathlib import Path

import sh

test_dir = Path.cwd() / "test"
result_dir = Path.cwd() / "result"
build_dir = Path.cwd() / "build"
input_file = Path.cwd() / "input.txt"
output_file = Path.cwd() / "output.txt"
measure_dir = Path.cwd() / "measure"
subdirs: list[str] = str(getenv("TEST_SUBDIR_NAMES")).split(";")
trees = str(getenv("TEST_TREE_NAMES")).split(";")
# subdirs: list[str] = ["1_random", "2_small_k_random/mid", "3_eff_random", "4_only_inserts"]
# trees = ["avl", "rb", "treap", "splay", "set"]

test_priority = {"small": 0, "medium": 1, "large": 2}

for subdir_name in subdirs:
    print(subdir_name)
    for tree in trees:
        for i in sorted((test_dir / subdir_name).iterdir(), key=lambda x: test_priority[x.name]):
            Path(result_dir / subdir_name / tree / i.name).mkdir(parents=True, exist_ok=True)
            Path(measure_dir / subdir_name / tree / i.name).mkdir(parents=True, exist_ok=True)
            for j in sorted(i.iterdir(), key=lambda x: int(x.name[:-4])):
                measure_file = measure_dir / subdir_name / tree / i.name / j.name
                shutil.copy(j, input_file)
                sh.time(["-f", "%S %e %U %M", str(build_dir/ tree)], _err=str(measure_file))
                input_file.unlink()
                try:
                    shutil.copy(output_file, result_dir / subdir_name / tree / i.name / j.name)
                    output_file.unlink()
                    print(f"done {tree} {i.name}/{j.name}")
                except FileNotFoundError:
                    print("no output")
            sum: float = 0
        print()
    print()
